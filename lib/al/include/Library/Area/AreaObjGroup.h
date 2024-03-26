#pragma once

#include <math/seadVector.h>

namespace al {
class AreaObj;

class AreaObjGroup {
public:
    AreaObjGroup(const char* groupName, const int capacity);
    void createBuffer();
    void createBuffer(const s32 capacity);
    AreaObj* getAreaObj(const s32 index) const;
    AreaObj* getInVolumeAreaObj(const sead::Vector3f& position) const;
    void incrementCount();
    void registerAreaObj(AreaObj* newAreaObj);

    const char* getName() const { return mGroupName; };

private:
    const char* mGroupName;
    AreaObj** mBuffer = nullptr;
    s32 mCount = 0;
    s32 mCapacity;
};
}  // namespace al
