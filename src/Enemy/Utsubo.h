#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class AreaObjGroup;
}  // namespace al

class Utsubo : public al::LiveActor {
public:
    Utsubo(const char* name, bool isWaitForWatcher);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    bool isAttack() const;
    bool isRiseReady() const;
    void setNerveRiseSign();

    void exeWait();
    void exeWaitForWatcher();
    void exeMove();
    void exeFollow();
    void exeRiseSign();
    void exeRise();
    void exeAttackSign();
    void exeAttack();
    void exeSink();
    void exeWaitForce();

private:
    bool isAttackSensorActive() const;
    void updateBodyAndCapSensorPos(f32 riseDistance);

    sead::Vector3f mRiseStartTrans = sead::Vector3f::zero;
    f32 mRiseMax = 2000.0f;
    sead::Vector3f mPrevPlayerPos = sead::Vector3f::zero;
    sead::Vector3f _124 = sead::Vector3f::zero;
    sead::Vector3f mBodySensorPos = sead::Vector3f::zero;
    sead::Vector3f mBodyCapSensorPos = sead::Vector3f::zero;
    al::AreaObjGroup* mMoveAreaGroup = nullptr;
    bool mIsWaitForWatcher;
    al::LiveActor* mLinkedShineActor = nullptr;
    bool mIsCloudSeaPlacement = false;
    bool mIsOnDepthShadow = true;
    sead::Matrix34f mSurfaceMtx = sead::Matrix34f::ident;
};

static_assert(sizeof(Utsubo) == 0x198);
