#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class IUsePlayerCollision;

class PlayerPushReceiver {
public:
    PlayerPushReceiver(const al::LiveActor* actor);

    void clear();
    bool receivePushMsg(const al::SensorMsg* msg, const al::HitSensor* self,
                        const al::HitSensor* other, f32 maxTrans);
    bool receivePushMsgHacker(const al::SensorMsg* msg, const al::HitSensor* self,
                              const al::HitSensor* other, f32 maxTrans, bool isPlayer);
    bool receivePushMsgYoshiNpc(const al::SensorMsg* msg, const al::HitSensor* self,
                                const al::HitSensor* other, f32 maxTrans);
    bool receivePushMsgCap(const al::SensorMsg* msg, const al::HitSensor* self,
                           const al::HitSensor* other, f32 maxTrans);
    bool receivePushMsgGrowPlant(const al::SensorMsg* msg, const al::HitSensor* self,
                                 const al::HitSensor* other, f32 maxTrans);
    bool receiveCollidePushMsg(const al::SensorMsg* msg);
    void receiveForceDirect(const sead::Vector3f& force);
    void cutPushVec(const sead::Vector3f& pushVec);
    void calcPushVec(sead::Vector3f* outPushVec) const;
    void calcOnlyCollidePushVec(sead::Vector3f* outCollidePushVec) const;
    void calcPushedVelocity(sead::Vector3f* outPushedVelocity,
                            const sead::Vector3f& velocity) const;
    void calcPushedVelocityCommon(sead::Vector3f* outPushedVelocity, const sead::Vector3f& velocity,
                                  const sead::Vector3f& pushVec) const;
    void calcPushedVelocityWithCollide(sead::Vector3f* outPushedVelocity,
                                       const sead::Vector3f& velocity,
                                       const IUsePlayerCollision* collision,
                                       f32 collisionRadius) const;

private:
    const al::LiveActor* mActor = nullptr;
    sead::Vector3f mPushMin = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mPushMax = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mCollidePushMin = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mCollidePushMax = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(PlayerPushReceiver) == 0x38);
