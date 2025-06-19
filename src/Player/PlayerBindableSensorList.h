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
    al::HitSensor* bindSensor = nullptr;
    u32 type = 0;
    f32 distance = 0;
    s32 priority = 0;

    bool operator<(const SensorInfo& other) const {
        if (priority < other.priority)
            return true;
        else if (priority == other.priority)
            return distance < other.distance;
        else
            return false;
    }
};

static_assert(sizeof(PlayerBindableSensorList::SensorInfo) == 0x18);
