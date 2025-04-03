#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadColor.h>
#include <math/seadQuat.h>

namespace al {
class LiveActor;
}

struct PlayerHackStartShaderParam {
    PlayerHackStartShaderParam(bool a1, f32 a2, s32 a3, s32 a4) : _0(a1), _4(a2), _8(a3), _c(a4) {}

    bool _0;
    f32 _4;
    s32 _8;
    s32 _c;
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
