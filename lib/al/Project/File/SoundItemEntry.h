#pragma once

#include <basis/seadTypes.h>
#include <container/seadBuffer.h>

#include "Project/FileEntryBase.h"

namespace sead {
class ArchiveRes;
class FileDevice;
}  // namespace sead

namespace al {
class IAudioResourceLoader {
public:
    // TODO: placeholder functions
    virtual void func_0() = 0;
    virtual void func_8() = 0;
    // TODO: unknown parameter name
    virtual bool tryLoad(u32 entryId, u32 unknown) = 0;
};

class SoundItemEntry : public FileEntryBase {
public:
    SoundItemEntry();
    void load() override;
    // TODO: unknown parameter name
    void setLoadRequestInfo(u32 itemId, u32 unknown, IAudioResourceLoader* resourceLoader);
    bool isLoadSuccess() const;
    u32 getSoundItemId() const;
    void clear();

    IAudioResourceLoader* getAudioResourceLoader() const { return mResourceLoader; }

private:
    s32 mItemId = -1;
    s32 _bc = -1;
    IAudioResourceLoader* mResourceLoader = nullptr;
    volatile bool mIsLoadSuccess = false;
};

}  // namespace al
