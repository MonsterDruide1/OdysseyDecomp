#include "Project/FileEntryBase.h"

namespace al {

FileEntryBase::FileEntryBase() {
    mMessageQueue.allocate(1, nullptr);
}

void FileEntryBase::setFileName(const sead::SafeStringBase<char>& fileName) {
    mFileName = fileName;
}

const sead::FixedSafeString<0x40>& FileEntryBase::getFileName() const {
    return mFileName;
}

void FileEntryBase::sendMessageDone() {
    mMessageQueue.push(1, sead::MessageQueue::BlockType::NonBlocking);
    mFileState = FileState::IsSendMessageDone;
}

void FileEntryBase::waitLoadDone() {
    mMessageQueue.pop(sead::MessageQueue::BlockType::Blocking);
    mFileState = FileState::IsLoadDone;
}

void FileEntryBase::clear() {
    mFileName.clear();
    mFileState = FileState::None;
    mMessageQueue.pop(sead::MessageQueue::BlockType::NonBlocking);
}

void FileEntryBase::setLoadStateRequested() {
    mFileState = FileState::IsLoadRequested;
}

}  // namespace al
