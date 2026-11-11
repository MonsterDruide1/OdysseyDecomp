#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

// Minimal partial reconstruction of the cap's ThrowStay nerve state.
// Only the constructor (signature from data/file_list.yml) and the small
// getters used by HackCap are declared so far; the rest of the class is
// still unknown. Layout from HackCapStateThrowStay.o (ctor 0x40b950):
// ActorStateBase (0x00-0x1f), then the ten ctor params at +0x20..+0x68,
// then zeroed state at +0x70..+0x10f. sizeof 0x110.
namespace al {
class LiveActor;
class HitSensor;
}  // namespace al
class PlayerColliderHackCap;
class PlayerSeparateCapFlag;
class PlayerInput;
class IUsePlayerCollision;
class PlayerEyeSensorHitHolder;
class HackCapTrigger;
class HackCapJointControlKeeper;
class HackCapJudgePreInputSeparateJump;
class HackCap;

class HackCapStateThrowStay : public al::ActorStateBase {
    friend class HackCap;

public:
    HackCapStateThrowStay(al::LiveActor*, const PlayerColliderHackCap*, const al::LiveActor*,
                          const PlayerSeparateCapFlag*, const PlayerInput*,
                          const IUsePlayerCollision*, const PlayerEyeSensorHitHolder*,
                          const HackCapTrigger*, HackCapJointControlKeeper*,
                          HackCapJudgePreInputSeparateJump*, const bool*);

    bool isHomingPlayerJump() const;
    bool isEnableAppendAttack() const;
    bool isEnableSendHipDropMsg() const;
    bool sendHipDropObjMsg(HackCapTrigger* trigger, al::HitSensor* sensor1,
                           al::HitSensor* sensor2);
    void sendHipDropCollideMsg(al::HitSensor* sensor);
    bool isEnableTouchJumpTransWarp() const;
    bool isEnableKeepStayTouchJump() const;

private:
    const PlayerColliderHackCap* mCollider;
    const al::LiveActor* mPlayerActor;
    const PlayerSeparateCapFlag* mSeparateCapFlag;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mPlayerCollision;
    const PlayerEyeSensorHitHolder* mCapEyeSensorHitHolder;
    const HackCapTrigger* mTrigger;
    HackCapJointControlKeeper* mJointControlKeeper;
    HackCapJudgePreInputSeparateJump* mJudgePreInputSeparateJump;
    const bool* _68;
    // Zeroed state at +0x70..+0x10f, written by HackCap::exeThrowStay.
    // (Offsets/types from HackCap.o exeThrowStay 0x405584.)
    bool _70;
    bool _71;  // set from HackCap::_29a
    bool _72;  // set from HackCap::_29b
    char _73;
    sead::Vector3f _74;  // target pos (CapTargetInfo lock-on or player trans)
    char _80[0x20];
    f32 _a0;  // throw height (mHackThrowHeight->value)
    f32 _a4;  // speed (60.0f / RollSpeed / WaterMaxSpeed / ContinuousThrowSpeed / MaxVel)
    f32 _a8;  // ret speed (WaterMaxRetSpeed / MaxRetSpeed)
    char _ac[4];
    const char* _b0;  // "StayL" / "StayR"
    char _b8[0x58];
};

static_assert(sizeof(HackCapStateThrowStay) == 0x110,
              "HackCapStateThrowStay size must stay 0x110");
