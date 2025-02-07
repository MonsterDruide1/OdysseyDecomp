#pragma once

#include <basis/seadTypes.h>

#include "Player/IUsePlayerHack.h"

namespace al {
class LiveActor;
}

class PlayerCollider;

class HackerDepthShadowMapCtrl {
public:
    HackerDepthShadowMapCtrl(al::LiveActor*, const char*, f32, f32, f32);
    void resetAndUpdate();
    void update(PlayerCollider*);

private:
    al::LiveActor* mActor;
    const char* _8;
    f32 _10;
    bool _14;
    f32 _18;
    s32 _1c;
    f32 _20;
    f32 _24;
};
