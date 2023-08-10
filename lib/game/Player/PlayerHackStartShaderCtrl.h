#pragma once

#include <gfx/seadColor.h>
#include <math/seadQuat.h>

namespace al {
class LiveActor;
}

struct PlayerHackStartShaderParam {};

class PlayerHackStartShaderCtrl {
public:
    PlayerHackStartShaderCtrl(al::LiveActor* host, PlayerHackStartShaderParam* param);
    void setHost(al::LiveActor* host);
    void start();
    void update();
    void end();

private:
    al::LiveActor* mParent;
    s32 mTime;
    bool mActive;
    sead::Color4f mColor;
    sead::Quatf mQuat;
    PlayerHackStartShaderParam* mParam;
};
