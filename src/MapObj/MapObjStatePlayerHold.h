#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
class SensorMsg;
class HitSensor;
}  // namespace al

class MapObjStatePlayerHold : public al::ActorStateBase {
public:
    MapObjStatePlayerHold(al::LiveActor* actor, const sead::Vector3f& localOffset,
                          const sead::Vector3f& localRotate);

    void initUseColliderPush(f32 pushForce);
    void initShadowMaskDropLengthCtrl(const char* shadowMaskName);
    void appear() override;
    void kill() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsgNoRelease(const al::SensorMsg* message, al::HitSensor* other,
                             al::HitSensor* self);
    bool receiveMsgRelease(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void prepareThrowCollide();
    bool receiveMsgCancel(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool tryStartCarryFront(const al::SensorMsg* message, al::HitSensor* sensor);
    bool tryStartCarryFrontWallKeep(const al::SensorMsg* message, al::HitSensor* sensor);
    bool tryStartCarryUp(const al::SensorMsg* message, al::HitSensor* sensor);
    void tryCancelHold(al::HitSensor* sensor);
    void updateCollider(al::HitSensor* sensor);
    void exeHold();

private:
    al::HitSensor* mPlayerHitSensor = nullptr;
    sead::Vector3f mLocalOffset;
    sead::Vector3f mLocalRotate;
    bool mIsCarryUp = false;
    bool mIsUseColliderPush = false;
    s32 mColliderUpOffset = 0;
    f32 mColliderRadius = 0.0f;
    f32 mColliderPushForce = 1.0f;
    sead::Vector3f mTotalPush = {0.0f, 0.0f, 0.0f};
    bool mIsResetPositionNeeded = false;
    const char* mShadowMaskName = nullptr;
    f32 mShadowMaskDropLength = 0.0f;
};
