#include "Player/PlayerBindableSensorList.h"

#include <container/seadPtrArray.h>

constexpr s32 PoolSize = 24;

PlayerBindableSensorList::PlayerBindableSensorList() {
    mActiveSensors.allocBuffer(PoolSize, nullptr);

    mPool.allocBuffer(PoolSize, nullptr);
    for (s32 i = 0; i < PoolSize; i++)
        mPool.pushBack(new SensorInfo());
}

void PlayerBindableSensorList::clear() {
    while (!mActiveSensors.isEmpty()) {
        SensorInfo* item = mActiveSensors.popBack();
        mPool.pushBack(item);
    }
}

void PlayerBindableSensorList::append(al::HitSensor* bindSensor, u32 type, f32 distance,
                                      s32 priority) {
    if (mPool.isEmpty())
        return;

    SensorInfo* dest = mPool.popBack();
    dest->bindSensor = bindSensor;
    dest->type = type;
    dest->overlap = distance;
    dest->priority = priority;

    mActiveSensors.pushBack(dest);
}

void PlayerBindableSensorList::remove(al::HitSensor* toRemove) {
    s32 size = mActiveSensors.size();
    for (s32 i = 0; i < size; i++) {
        if (mActiveSensors[i]->bindSensor == toRemove) {
            mPool.pushBack(mActiveSensors[i]);
            mActiveSensors.erase(i);
            return;
        }
    }
}

void PlayerBindableSensorList::sort() {
    mActiveSensors.sort();
}

u32 PlayerBindableSensorList::getNum() const {
    return mActiveSensors.size();
}

al::HitSensor* PlayerBindableSensorList::get(u32 index) const {
    return mActiveSensors[index]->bindSensor;
}

u32 PlayerBindableSensorList::getType(u32 index) const {
    return mActiveSensors[index]->type;
}

bool PlayerBindableSensorList::SensorInfo::operator<(const SensorInfo& other) const {
    if (priority < other.priority)
        return true;
    else if (priority == other.priority)
        return overlap < other.overlap;
    else
        return false;
}
