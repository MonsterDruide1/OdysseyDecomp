#pragma once

#include <container/seadPtrArray.h>

namespace al {
class HitSensor;
}

class PlayerBindableSensorList {
public:
    PlayerBindableSensorList();

    void clear();
    void append(al::HitSensor* bindSensor, u32 type, f32 distance, s32 priority);
    void remove(al::HitSensor* bindSensor);
    void sort();

    u32 getNum() const;
    al::HitSensor* get(u32 index) const;
    u32 getType(u32 index) const;

    struct SensorInfo;

private:
    sead::PtrArray<SensorInfo> mActiveSensors;
    sead::PtrArray<SensorInfo> mPool;
};

static_assert(sizeof(PlayerBindableSensorList) == 0x20);

struct PlayerBindableSensorList::SensorInfo {
    al::HitSensor* mBindSensor = nullptr;
    u32 mType = 0;
    f32 mDistance = 0;
    s32 mPriority = 0;

    bool operator<(const SensorInfo& other) const {
        if (mPriority < other.mPriority)
            return true;
        else if (mPriority == other.mPriority)
            return mDistance < other.mDistance;
        else
            return false;
    }
};

static_assert(sizeof(PlayerBindableSensorList::SensorInfo) == 0x18);
