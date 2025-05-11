#include "Project/File/SoundItemHolder.h"

#include "Project/File/SoundItemEntry.h"

namespace al {
SoundItemHolder::SoundItemHolder() {
    mSoundItemEntries.allocBufferAssert(0x280, nullptr);
}

SoundItemEntry* SoundItemHolder::addNewLoadRequestEntry(u32 itemId, u32 unknown,
                                                        IAudioResourceLoader* resourceLoader) {
    SoundItemEntry* entry = mSoundItemEntries.get(mSize);
    entry->setLoadRequestInfo(itemId, unknown, resourceLoader);
    mSize++;
    return entry;
}

SoundItemEntry* SoundItemHolder::tryFindEntry(u32 itemId, IAudioResourceLoader* resourceLoader) {
    for (s32 i = 0; i < mSize; i++) {
        SoundItemEntry* entry = mSoundItemEntries.get(i);
        if (entry->getSoundItemId() == itemId && entry->getAudioResourceLoader() == resourceLoader)
            return entry;
    }
    return nullptr;
}

void SoundItemHolder::waitLoadDoneAll() {
    for (s32 i = 0; i < mSize; i++) {
        SoundItemEntry* entry = mSoundItemEntries.get(i);
        if (entry->mFileState != FileState::IsLoadDone)
            entry->waitLoadDone();
    }
}

void SoundItemHolder::clearEntry() {
    for (s32 i = 0; i < mSize; i++)
        mSoundItemEntries.get(i)->clear();
    mSize = 0;
}

}  // namespace al
