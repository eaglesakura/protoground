#pragma once

#include "es/protoground.hpp"
#include <es/system/Hash64.h>
#include <es/system/NanoTimer.h>

namespace es {

class TaskQueue {
public:
    TaskQueue();

    class Task;

    class TaskHandle {
    public:
        /**
         * タスクが完了していたらtrue
         */
        bool isFinished() const;

        /**
         * タスクが終了するまでスピンロックをかける。
         */
        void await();

        TaskHandle();

        ~TaskHandle() = default;

    private:
        friend class TaskQueue;

        class Impl;

        sp<Impl> impl;
    };

    /**
     * Threadに関連付けられたタスクを全て実行する
     */
    void execute();

    /**
     * idに関連付けられたタスクを実行する
     */
    void execute(const Hash64 &threadId);

    /**
     * idに関連付けられたタスクを削除する
     *
     * 強制的に削除するため、未実行のタスクは実行されない。
     */
    void clean(const Hash64 &threadId);

    typedef std::function<void(TaskQueue *, const Hash64 &)> TaskFunction;

    /**
     * ラムダ式を登録する
     */
    TaskHandle add(const Hash64 &threadId, const TaskFunction function);

    /**
     * 指定Threadに対してタスクを登録する
     *
     * 指定されたThreadが存在しない場合、タスクは迷子となることに注意。
     */
    TaskHandle add(const Hash64 &threadId, const selection_ptr<Task> &task);

    class Task {
    public:
        virtual void execute(TaskQueue *queue, const Hash64 &threadId) = 0;

        virtual ~Task() = default;
    };

    ~TaskQueue() = default;

private:

    class Impl;

    class ThreadTaskQueue;

    class TaskRef;

    sp<Impl> impl;
};

}


