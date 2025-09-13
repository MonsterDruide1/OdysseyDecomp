#include "Project/File/SoundItemEntry.h"

namespace al {
SoundItemEntry::SoundItemEntry() = default;

void SoundItemEntry::load() {
    mIsLoadSuccess = mResourceLoader->tryLoad(mItemId, _6c);
    sendMessageDone();
}

void SoundItemEntry::setLoadRequestInfo(u32 itemId, u32 unknown,
                                        IAudioResourceLoader* resourceLoader) {
    mItemId = itemId;
    _6c = unknown;
    mResourceLoader = resourceLoader;
    setLoadStateRequested();
}

bool SoundItemEntry::isLoadSuccess() const {
    return mIsLoadSuccess;
}

u32 SoundItemEntry::getSoundItemId() const {
    return mItemId;
}

void SoundItemEntry::clear() {
    FileEntryBase::clear();
    mItemId = -1;
    _6c = -1;
    mResourceLoader = nullptr;
    mIsLoadSuccess = false;
}

}  // namespace al
