#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class IUsePlayerCollision;
class IUsePlayerHack;
class HackerJudgeNormalFall;
class HackerJudgeStartRun;
class PlayerActionTurnControl;

class HackerStateNormalMove : public al::ActorStateBase {
public:
    HackerStateNormalMove(al::LiveActor*, IUsePlayerHack**, const char*, const char*, const char*);

    void appear() override;
    void setLandNerve();
    void setupFallTime(s32);
    void changeWaitAnim(const char*);
    void changeRunAnim(const char*);
    void changeFallAnim(const char*);
    void usePlayerTurnControl(bool);
    void usePlayerCollision(IUsePlayerCollision*);
    bool isWaiting() const;
    bool isFalling() const;
    void exeWait();
    void updateGroundWait();
    bool tryPivot();
    void exeMove();
    void calcGravityDirLocal(sead::Vector3f*) const;
    void updateGroundMove();
    void exeBrake();
    void exePivot();
    void exeFall();

    PlayerActionTurnControl* getTurnControl() const { return mPlayerActionTurnControl; }

    void set_49(bool isEnabled) { _49 = isEnabled; }

    void set_54(f32 value) { _54 = value; }

    void set_5c(f32 value) { _5c = value; }

    void setMoveVelocity(f32 velocity) {
        mHasMoveVelocity = true;
        mMoveVelocity = velocity;
    }

    void setTurnAngle(f32 turnAngle) {
        mHasTurnAngle = true;
        mTurnAngle = turnAngle;
    }

private:
    IUsePlayerHack** mHacker;
    HackerJudgeNormalFall* mHackerJudgeNormalFall;
    HackerJudgeStartRun* mHackerJudgeStartRun;
    PlayerActionTurnControl* mPlayerActionTurnControl;
    IUsePlayerCollision* mIUsePlayerCollision;
    bool mIsUsePlayerTurnControl;
    bool _49;
    f32 _4c;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5c;
    bool mHasMoveVelocity;
    f32 mMoveVelocity;
    bool mHasTurnAngle;
    f32 mTurnAngle;
    sead::Vector3f mMoveDirection;
    const char* mWaitAnim;
    const char* mRunAnim;
    const char* mFallAnim;
    bool _98;
    bool _99;
};
