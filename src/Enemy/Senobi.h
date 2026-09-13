#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class CapTargetInfo;
class EnemyCap;
class IUsePlayerHack;
class SenobiStateEnemy;
class SenobiStateHack;

namespace al {
struct ActorInitInfo;
class HitSensor;
class JointSpringControllerHolder;
class SensorMsg;
}  // namespace al

struct SenobiParam {
    const s32* leafNumOneFoot = nullptr;
    const f32* leafDegree = nullptr;
    const f32* leafDisableMargin = nullptr;
};

class Senobi : public al::LiveActor {
public:
    Senobi(const char* actorName) : al::LiveActor(actorName) {}

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void makeActorDead() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void movement() override;
    void calcAnim() override;
    void updateCollider() override;

    static const char* getStretchJointName();
    static s32 getStretchSensorNum();
    static const sead::Vector3f& getStretchJointLocalOffset();

    f32 getStretchRate() const;
    void rebirth(const sead::Vector3f& trans, const sead::Vector3f& front);
    void resetCounter();
    void exeEnemy();
    void exeReset();
    void exeHack();

    IUsePlayerHack* getPlayerHack() const { return mPlayerHack; }

    f32 getStretchLength() const { return mStretchLength; }

    void setStretchLength(f32 stretchLength) { mStretchLength = stretchLength; }

private:
    SenobiParam* mParam = nullptr;
    IUsePlayerHack* mPlayerHack = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    EnemyCap* mEnemyCap = nullptr;
    SenobiStateEnemy* mStateEnemy = nullptr;
    SenobiStateHack* mStateHack = nullptr;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    f32 mStretchLength = 0.0f;
    bool mIsResetEnabled = false;
    u8 _145[3] = {};
};

static_assert(sizeof(Senobi) == 0x148);
