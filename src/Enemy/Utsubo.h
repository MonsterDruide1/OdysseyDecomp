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

    void exeWaitForWatcher();
    void exeWait();
    void exeMove();
    void exeFollow();
    void exeRiseSign();
    void exeRise();
    void exeAttackSign();
    void exeAttack();
    void exeSink();
    void exeWaitForce();

private:
    sead::Vector3f mRiseStartTrans;
    f32 mRiseMax;
    sead::Vector3f mPrevPlayerPos;
    sead::Vector3f mUnusedSensorPos;
    sead::Vector3f mBodySensorPos;
    sead::Vector3f mBodyCapSensorPos;
    al::AreaObjGroup* mMoveAreaGroup;
    bool mIsWaitForWatcher;
    al::LiveActor* mLinkedShineActor;
    bool mIsCloudSeaPlacement;
    bool mIsOnDepthShadow;
    sead::Matrix34f mSurfaceMtx;
};

static_assert(sizeof(Utsubo) == 0x198);
