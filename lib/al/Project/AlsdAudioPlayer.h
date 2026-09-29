#pragma once

#include <basis/seadTypes.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace alsd {
class SoundHandle;

class AudioPlayer {
    SEAD_RTTI_BASE(AudioPlayer)
public:
    virtual bool startSound(SoundHandle*, u32);
    virtual bool startSound(SoundHandle*, const char*);
    virtual bool holdSound(SoundHandle*, u32);
    virtual bool holdSound(SoundHandle*, const char*);

    virtual void initialize() {}

    virtual void finalize() {}

    virtual void calc() {}

    virtual s32 getSoundCount() const { return 0; };

    virtual const char* getSoundName(u32) const { return nullptr; };

    virtual s32 getSoundId(const char*) const { return -1; };
};

static_assert(sizeof(AudioPlayer) == 0x8);
}  // namespace alsd
