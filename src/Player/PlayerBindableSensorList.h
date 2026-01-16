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
    void remove(al::HitSensor* toRemove);
    void sort();

    u32 getNum() const;
    al::HitSensor* get(u32 index) const;
    u32 getType(u32 index) const;

private:
    struct SensorInfo {
        al::HitSensor* bindSensor = nullptr;
        u32 type = 0;
        f32 overlap = 0;
        s32 priority = 0;
        inline bool operator<(const SensorInfo& other) const;
    };

    sead::PtrArray<SensorInfo> mActiveSensors;
    sead::PtrArray<SensorInfo> mPool;

    static_assert(sizeof(SensorInfo) == 0x18);
};

static_assert(sizeof(PlayerBindableSensorList) == 0x20);
