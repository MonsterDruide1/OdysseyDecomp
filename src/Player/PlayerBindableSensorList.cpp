#include "Player/PlayerBindableSensorList.h"

#include <container/seadPtrArray.h>

const s32 PoolSize = 24;

// Workaround until https://github.com/open-ead/sead/pull/207 is merged.
// Reason: Current implementation is heavily inlined, which causes mismatches.
template <>
__attribute__((noinline)) void
sead::PtrArray<PlayerBindableSensorList::SensorInfo>::sort(CompareCallback cmp) {
    PtrArrayImpl::sort_<PlayerBindableSensorList::SensorInfo>(cmp);
}

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
    dest->distance = distance;
    dest->priority = priority;

    mActiveSensors.pushBack(dest);
}

void PlayerBindableSensorList::remove(al::HitSensor* toRemove) {
    s32 found;

    s32 size = mActiveSensors.size();
    for (found = 0; found < size; found++) {
        const SensorInfo* item = mActiveSensors[found];
        if (item->bindSensor == toRemove)
            break;
    }

    if (found >= size)
        return;

    SensorInfo* item = mActiveSensors.at(found);
    mPool.pushBack(item);
    mActiveSensors.erase(found);
}

void PlayerBindableSensorList::sort() {
    mActiveSensors.sort();
}

u32 PlayerBindableSensorList::getNum() const {
    return mActiveSensors.size();
}

al::HitSensor* PlayerBindableSensorList::get(u32 index) const {
    return mActiveSensors.at(index)->bindSensor;
}

u32 PlayerBindableSensorList::getType(u32 index) const {
    return mActiveSensors.at(index)->type;
}
