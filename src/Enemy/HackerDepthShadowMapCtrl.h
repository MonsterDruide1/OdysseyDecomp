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
    float _18;
    s32 _1c;
    float _20;
    float _24;
};
