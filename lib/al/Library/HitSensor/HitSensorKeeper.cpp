#include "Library/HitSensor/HitSensorKeeper.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {

HitSensorKeeper::HitSensorKeeper(s32 maxSensors) : mMaxSensors(maxSensors) {
    mSensors = new HitSensor*[maxSensors];
    for (s32 i = 0; i < mMaxSensors; i++)
        mSensors[i] = nullptr;
}

HitSensor* HitSensorKeeper::addSensor(LiveActor* parentActor, const char* name, u32 hitSensorType,
                                      f32 radius, u16 maxSensorCount,
                                      const sead::Vector3f* followPos,
                                      const sead::Matrix34f* followMatrix,
                                      const sead::Vector3f& offset) {
    auto* newSensor = new HitSensor(parentActor, name, hitSensorType, radius, maxSensorCount,
                                    followPos, followMatrix, offset);
    mSensors[mSensorCount] = newSensor;
    mSensorCount++;
    newSensor->update();
    return newSensor;
}

void HitSensorKeeper::update() {
    for (s32 i = 0; i < mSensorCount; i++)
        mSensors[i]->update();
}

s32 HitSensorKeeper::getSensorNum() const {
    return mSensorCount;
}

HitSensor* HitSensorKeeper::getSensor(s32 index) const {
    return mSensors[index];
}

void HitSensorKeeper::attackSensor() {
    for (s32 i = 0; i < mSensorCount; i++) {
        auto* sensor = mSensors[i];
        sensor->trySensorSort();
        for (s32 j = 0; j < sensor->mSensorCount; j++) {
            auto* subSensor = sensor->mSensors[j];
            if (!isDead(subSensor->mParentActor))
                sensor->mParentActor->attackSensor(sensor, subSensor);
        }
    }
}

void HitSensorKeeper::clear() {
    for (s32 i = 0; i < mSensorCount; i++)
        mSensors[i]->mSensorCount = 0;
}

void HitSensorKeeper::validate() {
    for (s32 i = 0; i < mSensorCount; i++)
        mSensors[i]->validate();
}

void HitSensorKeeper::invalidate() {
    for (s32 i = 0; i < mSensorCount; i++)
        mSensors[i]->invalidate();
}

void HitSensorKeeper::validateBySystem() {
    for (s32 i = 0; i < mSensorCount; i++)
        mSensors[i]->validateBySystem();
}

void HitSensorKeeper::invalidateBySystem() {
    for (s32 i = 0; i < mSensorCount; i++)
        mSensors[i]->invalidateBySystem();
}

HitSensor* HitSensorKeeper::getSensor(const char* name) const {
    if (mSensorCount == 1)
        return mSensors[0];

    for (s32 i = 0; i < mSensorCount; i++)
        if (isEqualString(mSensors[i]->mName, name))
            return mSensors[i];

    return nullptr;
}

}  // namespace al
