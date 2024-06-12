#pragma once

#include "Player/IUsePlayerCollision.h"
#include "Player/IUsePlayerHack.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"

class PlayerActionPivotTurnControl {
public:
    PlayerActionPivotTurnControl(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,float);
    void reset();
    void update();
    void calcMoveDirection(sead::Vector3<float> *,sead::Vector3<float> const&);

public:
    al::LiveActor *mPlayer;
    const PlayerConst *mConst;
    const PlayerInput *mInput;
    const IUsePlayerCollision *mCollider;
    const IUsePlayerHack **_20 = nullptr;
    sead::Vector3f _28 = {0.0f, 0.0f, 0.0f};
    float mGravity;
    sead::Vector3f _38 = {0.0f, 0.0f, 0.0f};
    bool _44 = false;
    bool _45 = false;
    int _48 = 0;
};
