#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class HitSensor;

class ActorSensorController {
public:
    ActorSensorController(al::LiveActor* sensorParent, const char* sensorName);
    void setSensorScale(float scale);  // Should this be called multiplier?
    void setSensorRadius(float radius);
    void setSensorFollowPosOffset(const sead::Vector3f& offset);
    void resetActorSensorController();

    f32 getSensorRadius() const { return mSensorRadius; };

    const sead::Vector3f& getSensorOffset() const { return mSensorOffset; };

private:
    HitSensor* mHitSensor = nullptr;
    float mSensorRadius = 0.0f;
    sead::Vector3f mSensorOffset = {0.0f, .0f, 0.0f};
};

class ActorSensorControllerList {
public:
    ActorSensorControllerList(s32 maxControllers);
    void addSensor(al::LiveActor* actor, const char* sensorName);
    void setAllSensorScale(f32 scale);
    void resetAllActorSensorController();

private:
    ActorSensorController** mControllers;
    s32 mMaxControllers;
    s32 mControllerNum = 0;
};

}  // namespace al
