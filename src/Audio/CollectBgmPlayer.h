#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

namespace al {
class IUseAudioKeeper;
}

class CollectBgmPlayer : public al::HioNode, public al::ISceneObj {
public:
    CollectBgmPlayer();
    void init(const al::IUseAudioKeeper* audio_keeper);
    void prepare();
    void start(const char* name, const char* situation_name);
    void stop(s32 fade_frames);
    void isPlaying(const char* name, const char* situation_name) const;

private:
    char filler[0x18];
};

static_assert(sizeof(CollectBgmPlayer) == 0x20);
