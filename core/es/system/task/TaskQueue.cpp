#include <queue>
#include <es/system/Thread.hpp>
#include "TaskQueue.h"
#include <boost/unordered_map.hpp>
#include <es/system/NanoTimer.h>

namespace es {

class TaskQueue::TaskRef {
public:
    TaskRef(const selection_ptr<Task> &task) : task(task) {

    }

    ~TaskRef() = default;

    void execute(TaskQueue *queue, const hash &theradId) {
        auto nTask = task.lock();
        if (nTask) {
            nTask->execute(queue, theradId);
        }
        task.reset();
        finished = true;
    }

    bool isFinished() const {
        return finished;
    }

private:
    selection_ptr<Task> task;
    bool finished = false;
};

class TaskQueue::TaskHandle::Impl {
public:
    sp<TaskQueue::TaskRef> ref;
};

class TaskQueue::ThreadTaskQueue {
public:
    ThreadTaskQueue() {
    }

    sp<TaskQueue::TaskRef> add(const selection_ptr<Task> &task) {
        es::mutex_lock lock(mtx);

        sp<TaskQueue::TaskRef> ref(new TaskQueue::TaskRef(task));
        tasks.push_back(ref);
        return ref;
    }

    void execute(TaskQueue *queue, const hash &threadId) {
        // 正しいThreadで動いていることを検証
        assert(hash::from(std::this_thread::get_id()) == threadId);
        std::vector<sp<TaskQueue::TaskRef> > cpyTasks;

        // ロック時間を最小限にするため、タスクをコピー実行
        {
            es::mutex_lock lock(mtx);
            cpyTasks = tasks;
            tasks.clear();
        }

        for (auto &ref : cpyTasks) {
            ref->execute(queue, threadId);
        }
    }

    ~ThreadTaskQueue() = default;

    /**
     * 保留タスク
     */
    struct PendingTask {
        /**
         * 実行を許可する時刻
         */
        nano_time executeTimeStamp;

        const selection_ptr<Task> &task;
    };

private:
    mutex mtx;
    std::vector<sp<TaskQueue::TaskRef> > tasks;
};

class TaskQueue::Impl {
public:
    mutex mtx;
    boost::unordered_map<hash, sp<TaskQueue::ThreadTaskQueue> > threadTasks;
};

TaskQueue::TaskQueue() {
    impl.reset(new TaskQueue::Impl());
}

TaskQueue::TaskHandle TaskQueue::add(const hash &threadId, const TaskQueue::TaskFunction function) {
    class TaskImpl : public Task {
    public:
        TaskQueue::TaskFunction func;

        virtual void execute(TaskQueue *queue, const hash &threadId) {
            func(queue, threadId);
        }

        virtual ~TaskImpl() {
        }
    };

    sp<TaskImpl> task(new TaskImpl());
    task->func = function;

    return add(threadId, task);
}

TaskQueue::TaskHandle TaskQueue::add(const hash &threadId, const selection_ptr<Task> &task) {
    sp<TaskQueue::ThreadTaskQueue> queue;

    {
        mutex_lock lock(impl->mtx);
        auto itr = impl->threadTasks.find(threadId);
        if (itr != impl->threadTasks.end()) {
            queue = itr->second;
        } else {
            queue.reset(new TaskQueue::ThreadTaskQueue());
            impl->threadTasks.insert(std::make_pair(threadId, queue));
        }
    }
    assert((bool) queue);

    auto ref = queue->add(task);
    assert((bool) ref);
    TaskHandle handle;
    handle.impl->ref = ref;

    return handle;
}

void TaskQueue::execute() {
    execute(Hash64::from(std::this_thread::get_id()));
}

void TaskQueue::execute(const hash &threadId) {
    sp<ThreadTaskQueue> queue;
    {
        mutex_lock lock(impl->mtx);
        auto itr = impl->threadTasks.find(threadId);
        if (itr == impl->threadTasks.end()) {
            return;
        }
        queue = itr->second;
    }
    queue->execute(this, threadId);
}

void TaskQueue::clean(const hash &threadId) {
    mutex_lock lock(impl->mtx);
    impl->threadTasks.erase(threadId);
}

TaskQueue::TaskHandle::TaskHandle() {
    impl.reset(new TaskQueue::TaskHandle::Impl());
}

bool TaskQueue::TaskHandle::isFinished() const {
    return impl->ref->isFinished();
}

void TaskQueue::TaskHandle::await() {
    while (!isFinished()) {
        NanoTimer::sleepMs(1);
    }
}


}