#pragma once

#include <math/seadVector.h>
#include "MapObj/AnagramAlphabetCharacter.h"

namespace al {
class LiveActor;
}

class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class PlayerActionTurnControl;

class PlayerActionAirMoveControl {
public:
    PlayerActionAirMoveControl(al::LiveActor*, const PlayerConst*, const PlayerInput*,
                               const IUsePlayerCollision*, bool);
    void setup(float, float, int, float, float, int, float);
    void setupTurn(float, float, float, float, int, int, int);
    void setExtendFrame(int);
    void setupCollideWallScaleVelocity(float, float, float);
    void verticalizeStartMoveDir(const sead::Vector3f&);
    void update();
    bool isHoldJumpExtend() const;
    void calcMoveInput(sead::Vector3f*, const sead::Vector3f&) const;

public:
    al::LiveActor* mLiveActor;
    const PlayerConst* mPlayerConst;
    const PlayerInput* mPlayerInput;
    const IUsePlayerCollision* mCollision;
    IUsePlayerHack* mIUsePlayerHack = nullptr;
    PlayerActionTurnControl* mTurnControl = nullptr;
    bool mIsDamageOrWallCatch;
    bool unk_flag2 = false;
    bool unk_flag3 = false;
    bool mIs2D = false;
    int mExtendFrame = 0;
    int someCounter = 0;
    int someCounterRunningBackwards = 0;
    bool unk = false;
    bool unk2 = false;
    bool unk3 = false;
    float val1 = 0.0f;
    float mJumpSpeedMax = 0.0f;
    sead::Vector3f anotherVec = {0.0f, 0.0f, 0.0f};
    sead::Vector3f someOtherVec = {0.0f, 0.0f, 0.0f};
    float somethingHere = 0.0f;
    float mJumpGravity = 0.0f;
    float mMaxFallSpeed = 0.0f;
    bool isSetCollideWallScaleVelocity = false;
    sead::Vector3f mCollideWallScaleVelocity = {0.0f, 0.0f, 0.0f};
    float something2 = 0.0f;
    sead::Vector3f someVec = {0.0f, 0.0f, 0.0f};
};
