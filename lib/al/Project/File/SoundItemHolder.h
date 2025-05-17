#pragma once

#include <basis/seadTypes.h>
#include <container/seadBuffer.h>

#include "Project/FileEntryBase.h"

namespace sead {
class ArchiveRes;
class FileDevice;
}  // namespace sead

namespace al {
class IAudioResourceLoader;
class SoundItemEntry;

class SoundItemHolder {
public:
    SoundItemHolder();
    SoundItemEntry* addNewLoadRequestEntry(u32 itemId, u32 unknown,
                                           IAudioResourceLoader* resourceLoader);
    SoundItemEntry* tryFindEntry(u32 itemId, IAudioResourceLoader* resourceLoader);
    void waitLoadDoneAll();
    void clearEntry();

private:
    sead::Buffer<SoundItemEntry> mSoundItemEntries;
    s32 mSize = 0;
};

}  // namespace al
