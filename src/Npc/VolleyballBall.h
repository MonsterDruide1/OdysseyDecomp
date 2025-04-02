#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {

struct ActorInitInfo;
class HitSensor;
class ParabolicPath;
class SensorMsg;
}  // namespace al

class VolleyballNpc;

class VolleyballBall : public al::LiveActor {
public:
    VolleyballBall(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    void attack(const sead::Vector3f& startPosition, const sead::Vector3f& endPosition,
                f32 attackSpeed);
    void toss(const sead::Vector3f& startPosition, const sead::Vector3f& endPosition);
    void reset();

    bool isMiss() const;
    bool isReturnEnd() const;
    bool isTossEnd() const;
    bool isActive() const;
    bool isRetry() const;
    bool isRetryEnd() const;

    void exeWait();
    void exeAttack();
    void exeOnGround();
    void endOnGround();
    void exeReturn();
    void exeReturnSmash();
    void exeReturnEnd();
    void exeMiss();
    void exeMissReaction();
    void exeToss();
    void exeTossEnd();
    void exeRetry();
    void exeRetryEnd();

private:
    sead::Vector3f mEndPosition = sead::Vector3f::zero;
    al::ParabolicPath* mAttackPath = nullptr;
    al::ParabolicPath* mReturnPath = nullptr;
    f32 mAttackSpeed = 0.0f;
    s32 mPathTime = 0;
    VolleyballNpc* mNpc = nullptr;
    bool _138 = false;
    sead::Quatf mOrientation = sead::Quatf::unit;
    s32 mLastSmashCoins = 0;
    s32 mMissReactionDelay = 0;
    s32 mMissDelay = 0;
};
