#pragma once

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class AudioKeeper;
class BgmUserInfo;

class ActionBgmCtrl : public HioNode, public IUseAudioKeeper {
public:
    static ActionBgmCtrl* tryCreate(AudioKeeper*);

    ActionBgmCtrl(AudioKeeper*, BgmUserInfo*);
    void startAction(const char*);
    void update(float, float, float, bool);

    AudioKeeper* getAudioKeeper() const override;
};
}  // namespace al
