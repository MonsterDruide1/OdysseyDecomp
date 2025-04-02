#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class HitSensor;

class ActorSensorController {
public:
    ActorSensorController(LiveActor* sensorParent, const char* sensorName);
    void setSensorScale(f32 scale);
    void setSensorRadius(f32 radius);
    void setSensorFollowPosOffset(const sead::Vector3f& offset);
    void resetActorSensorController();

    f32 getSensorRadius() const { return mSensorRadius; };

    const sead::Vector3f& getSensorFollowPosOffset() const { return mSensorFollowPosOffset; };

private:
    HitSensor* mHitSensor = nullptr;
    f32 mSensorRadius = 0.0f;
    sead::Vector3f mSensorFollowPosOffset = {0.0f, 0.0f, 0.0f};
};

class ActorSensorControllerList {
public:
    ActorSensorControllerList(s32 maxControllers);
    void addSensor(LiveActor* actor, const char* sensorName);
    void setAllSensorScale(f32 scale);
    void resetAllActorSensorController();

private:
    ActorSensorController** mControllers;
    s32 mMaxControllers;
    s32 mControllerNum = 0;
};

}  // namespace al
