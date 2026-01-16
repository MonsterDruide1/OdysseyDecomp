#pragma once

#include <basis/seadTypes.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class AudioKeeper;
struct BgmUserInfo;

class ActionBgmCtrl : public HioNode, public IUseAudioKeeper {
public:
    static ActionBgmCtrl* tryCreate(AudioKeeper*);

    ActionBgmCtrl(AudioKeeper*, BgmUserInfo*);
    void startAction(const char*);
    void update(f32 frame, f32 frameRateMax, f32 frameRate, bool isStop);

    AudioKeeper* getAudioKeeper() const override;
};
}  // namespace al
