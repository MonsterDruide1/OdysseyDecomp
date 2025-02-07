#pragma once

#include <gfx/seadColor.h>
#include <math/seadQuat.h>

namespace al {
class LiveActor;
}

struct PlayerHackStartShaderParam {
    PlayerHackStartShaderParam(bool a1, float a2, int a3, int a4)
        : _0(a1), _4(a2), _8(a3), _c(a4) {}

    bool _0;
    float _4;
    int _8;
    int _c;
};

static_assert(sizeof(PlayerHackStartShaderParam) == 0x10, "PlayerHackStartShaderParam Size");

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
    bool mIsActive;
    sead::Color4f mColor;
    sead::Quatf mQuat;
    PlayerHackStartShaderParam* mParam;
};
