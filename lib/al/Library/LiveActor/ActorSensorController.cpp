#include "Library/LiveActor/ActorSensorController.h"

#include "Library/LiveActor/ActorSensorUtil.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {

ActorSensorController::ActorSensorController(LiveActor* sensorParent, const char* sensorName) {
    HitSensor* actorSensor = getHitSensor(sensorParent, sensorName);
    mHitSensor = actorSensor;
    mSensorRadius = actorSensor->getRadius();
    mSensorFollowPosOffset = actorSensor->getFollowPosOffset();
}

void ActorSensorController::setSensorScale(f32 scale) {
    mHitSensor->setRadius(mSensorRadius * scale);
    mHitSensor->setFollowPosOffset(mSensorFollowPosOffset * scale);
}

void ActorSensorController::setSensorRadius(f32 radius) {
    mHitSensor->setRadius(radius);
}

void ActorSensorController::setSensorFollowPosOffset(const sead::Vector3f& offset) {
    mHitSensor->setFollowPosOffset(offset);
}

void ActorSensorController::resetActorSensorController() {
    mHitSensor->setRadius(mSensorRadius);
    mHitSensor->setFollowPosOffset(mSensorFollowPosOffset);
}

ActorSensorControllerList::ActorSensorControllerList(s32 maxControllers)
    : mMaxControllers(maxControllers) {
    mControllers = new ActorSensorController*[maxControllers];
    for (s32 i = 0; i < mMaxControllers; i++)
        mControllers[i] = nullptr;
}

void ActorSensorControllerList::addSensor(LiveActor* actor, const char* sensorName) {
    mControllers[mControllerNum++] = new ActorSensorController(actor, sensorName);
}

void ActorSensorControllerList::setAllSensorScale(f32 scale) {
    for (s32 i = 0; i < mControllerNum; i++)
        mControllers[i]->setSensorScale(scale);
}

void ActorSensorControllerList::resetAllActorSensorController() {
    for (s32 i = 0; i < mControllerNum; i++)
        mControllers[i]->resetActorSensorController();
}

}  // namespace al
