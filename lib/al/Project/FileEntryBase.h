#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>
#include <thread/seadMessageQueue.h>

namespace al {

enum class FileState : s32 {
    None,
    IsLoadRequested,
    IsSendMessageDone,
    IsLoadDone,
};

class FileEntryBase {
public:
    FileEntryBase();

    virtual void load() = 0;

    void setFileName(const sead::SafeString& fileName);
    const sead::SafeString& getFileName() const;
    void sendMessageDone();
    void waitLoadDone();
    void clear();
    void setLoadStateRequested();

    // TODO: Remove, mFileState optimizes differently using a getter
    friend class ArchiveHolder;

private:
    sead::FixedSafeString<0x40> mFileName;
    FileState mFileState = FileState::None;
    sead::MessageQueue mMessageQueue;
};

static_assert(sizeof(FileEntryBase) == 0xb8);
}  // namespace al
