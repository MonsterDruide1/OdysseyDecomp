#pragma once

#include <math/seadVector.h>

namespace al {
class AreaObj;

class AreaObjGroup {
private:
    const char* mGroupName;
    al::AreaObj** mBuffer = nullptr;
    s32 mCount = 0;
    s32 mCapacity;

public:
    AreaObjGroup(const char* groupName, const int capacity);
    void createBuffer();
    void createBuffer(const s32 capacity);
    al::AreaObj* getAreaObj(const s32 index) const;
    al::AreaObj* getInVolumeAreaObj(const sead::Vector3f& position) const;
    void incrementCount();
    void registerAreaObj(al::AreaObj* newAreaObj);

    const char* getName() const { return mGroupName; };
};
}  // namespace al
