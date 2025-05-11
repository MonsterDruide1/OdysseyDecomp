#include "Project/ArchiveHolder.h"

#include <basis/seadNew.h>

#include "Library/Base/StringUtil.h"

namespace al {

ArchiveHolder::ArchiveHolder() {
    mArchiveEntries.allocBufferAssert(0x800, nullptr);
}

ArchiveEntry* ArchiveHolder::addNewLoadRequestEntry(const sead::SafeString& fileName,
                                                    sead::Heap* heap,
                                                    sead::FileDevice* fileDevice) {
    ArchiveEntry* entry = mArchiveEntries.get(mSize);
    entry->setLoadRequestInfo(fileName, heap, fileDevice);
    mSize++;
    return entry;
}

ArchiveEntry* ArchiveHolder::tryFindEntry(const sead::SafeString& fileName) {
    for (s32 i = 0; i < mSize; i++) {
        ArchiveEntry* entry = mArchiveEntries.get(i);
        if (isEqualString(entry->getFileName(), fileName))
            return entry;
    }
    return nullptr;
}

void ArchiveHolder::waitLoadDoneAll() {
    for (s32 i = 0; i < mSize; i++) {
        ArchiveEntry* entry = mArchiveEntries.get(i);
        if (entry->mFileState != FileState::IsLoadDone)
            entry->waitLoadDone();
    }
}

void ArchiveHolder::clearEntry() {
    for (s32 i = 0; i < mSize; i++)
        mArchiveEntries.get(i)->clear();
    mSize = 0;
}

}  // namespace al
