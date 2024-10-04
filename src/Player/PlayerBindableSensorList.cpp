#include "Player/PlayerBindableSensorList.h"

#include <container/seadPtrArray.h>

const s32 PoolSize = 24;

// Only used in the three below workarounds…
using SensorInfo = PlayerBindableSensorList::SensorInfo;

// Required for matching: don’t inline PtrArray::sort(CompareCallback)
template <>
__attribute__((noinline)) void sead::PtrArray<SensorInfo>::sort(CompareCallback cmp) {
    PtrArrayImpl::sort_<SensorInfo>(cmp);
}

// Workaround for possible bug in sead: compareT has the wrong signature
#define compareT _ZN4sead8PtrArrayIN24PlayerBindableSensorList10SensorInfoEE8compareTEPKS2_S5_
extern "C" int compareT(const SensorInfo* a, const SensorInfo* b) {
    if (*a < *b)
        return -1;
    if (*b < *a)
        return 1;
    return 0;
}
template <>
void sead::PtrArray<SensorInfo>::sort() {
    sort(compareT);
}
// End workaround

// Workaround for possible mismatch in sead: popBack doesn't seem to match
template <>
SensorInfo* sead::PtrArray<SensorInfo>::popBack() {
    if (size() < 1)
        return nullptr;
    else
        return static_cast<SensorInfo*>(PtrArrayImpl::popBack());
}
// End workaround

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
    dest->mBindSensor = bindSensor;
    dest->mType = type;
    dest->mDistance = distance;
    dest->mPriority = priority;

    mActiveSensors.pushBack(dest);
}

void PlayerBindableSensorList::remove(al::HitSensor* toRemove) {
    s32 found;

    s32 size = mActiveSensors.size();
    for (found = 0; found < size; found++) {
        const SensorInfo* item = mActiveSensors[found];
        if (item->mBindSensor == toRemove)
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
    return mActiveSensors.at(index)->mBindSensor;
}

u32 PlayerBindableSensorList::getType(u32 index) const {
    return mActiveSensors.at(index)->mType;
}
