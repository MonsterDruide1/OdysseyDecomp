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
    PlayerPushReceiver(const al::LiveActor*);

    void clear();
    bool receivePushMsg(const al::SensorMsg*, const al::HitSensor*, const al::HitSensor*, f32);
    bool receivePushMsgHacker(const al::SensorMsg*, const al::HitSensor*, const al::HitSensor*, f32,
                              bool);
    bool receivePushMsgYoshiNpc(const al::SensorMsg*, const al::HitSensor*, const al::HitSensor*,
                                f32);
    bool receivePushMsgCap(const al::SensorMsg*, const al::HitSensor*, const al::HitSensor*, f32);
    bool receivePushMsgGrowPlant(const al::SensorMsg*, const al::HitSensor*, const al::HitSensor*,
                                 f32);
    bool receiveCollidePushMsg(const al::SensorMsg*);
    bool receiveForceDirect(const sead::Vector3f&);
    void cutPushVec(const sead::Vector3f&);
    void calcPushVec(sead::Vector3f*) const;
    void calcOnlyCollidePushVec(sead::Vector3f*) const;
    void calcPushedVelocity(sead::Vector3f*, const sead::Vector3f&) const;
    void calcPushedVelocityCommon(sead::Vector3f*, const sead::Vector3f&,
                                  const sead::Vector3f&) const;
    void calcPushedVelocityWithCollide(sead::Vector3f*, const sead::Vector3f&,
                                       const IUsePlayerCollision*, f32) const;

private:
    al::LiveActor* mActor;
    sead::Vector3f _8;
    sead::Vector3f _14;
    sead::Vector3f _20;
    sead::Vector3f _2c;
};

static_assert(sizeof(PlayerPushReceiver) == 0x38);
