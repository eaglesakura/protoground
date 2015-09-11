#pragma once

#include "es/protoground-fbx.hpp"
#include <es/asset/IAsset.hpp>

namespace es {
namespace internal {


class FbxInputStream : public FbxStream {
public:
    FbxInputStream(int64_t length) : length(length) {
        assert(length > 0);
    }

    /** Query the current state of the stream. */
    virtual EState GetState() {
        return head != nullptr ? eOpen : eClosed;
    }

    /** Open the stream.
    * \return True if successful.
    * \remark Each time the stream is open or closed, the stream position must be reset to zero. */
    virtual bool Open(void *pStreamData) {
        current = 0;
        head = (uint8_t*)pStreamData;
        return true;
    }

    /** Close the stream.
    * \return True if successful.
    * \remark Each time the stream is open or closed, the stream position must be reset to zero. */
    virtual bool Close() {
        head = nullptr;
        current = 0;
        return true;
    }

    /** Empties the internal data of the stream.
    * \return True if successful. */
    virtual bool Flush() {
        return true;
    }

    /** Writes a memory block.
    * \param pData Pointer to the memory block to write.
    * \param pSize Size (in bytes) of the memory block to write.
    * \return The number of bytes written in the stream. */
    virtual int Write(const void * /*pData*/, int /*pSize*/) {
        return 0;
    }

    /** Read bytes from the stream and store them in the memory block.
    * \param pData Pointer to the memory block where the read bytes are stored.
    * \param pSize Number of bytes read from the stream.
    * \return The actual number of bytes successfully read from the stream. */
    virtual int Read(void *pData, int size) const {
        memcpy(pData, head + current, size);
        current += size;
        return size;
    }

    /** If not specified by KFbxImporter::Initialize(), the importer will ask
    * the stream to select an appropriate reader ID to associate with the stream.
    * FbxIOPluginRegistry can be used to locate id by extension or description.
    * Return -1 to allow FBX to select an appropriate default. */
    virtual int GetReaderID() const {
        return 0;
    }

    /** If not specified by KFbxExporter::Initialize(), the exporter will ask
    * the stream to select an appropriate writer ID to associate with the stream.
    * KFbxIOPluginRegistry can be used to locate id by extension or description.
    * Return -1 to allow FBX to select an appropriate default. */
    virtual int GetWriterID() const {
        return -1;
    }

    /** Adjust the current stream position.
    * \param pSeekPos Pre-defined position where offset is added (FbxFile::eBegin, FbxFile::eCurrent:, FbxFile::eEnd)
    * \param pOffset Number of bytes to offset from pSeekPos. */
    virtual void Seek(const FbxInt64 &pOffset, const FbxFile::ESeekPos &pSeekPos) {
        switch (pSeekPos) {
            case FbxFile::ESeekPos::eBegin:
                current = pOffset;
                break;
            case FbxFile::ESeekPos::eCurrent:
                current += pOffset;
                break;
            case FbxFile::ESeekPos::eEnd:
                assert(length > 0);
                assert(pOffset <= 0);
                current = length + pOffset;
                break;
            default:
                assert(false);
        }
    }

    /** Get the current stream position.
    * \return Current number of bytes from the beginning of the stream. */
    virtual long GetPosition() const {
        return current;
    }

    /** Set the current stream position.
    * \param pPosition Number of bytes from the beginning of the stream to seek to. */
    virtual void SetPosition(long pPosition) {
        current = pPosition;
    }

    /** Return 0 if no errors occurred. Otherwise, return 1 to indicate
    * an error. This method will be invoked whenever FBX needs to verify
    * that the last operation succeeded. */
    virtual int GetError() const {
        return 0;
    }

    /** Clear current error condition by setting the current error value to 0. */
    virtual void ClearError() {

    }

    virtual ~FbxInputStream() = default;

private:
    mutable uint8_t *head = nullptr;
    mutable int64_t current = 0;
    mutable int64_t length = 0;
};

}
}

