#include "Library/Area/AreaObjGroup.h"

#include "Library/Area/AreaObj.h"

namespace al {
AreaObjGroup::AreaObjGroup(const char* groupName, const int capacity)
    : mGroupName(groupName), mCapacity(capacity) {}

void AreaObjGroup::createBuffer() {
    if (mCapacity < 1) {
        return;
    }
    mBuffer = new AreaObj*[mCapacity];
}

void AreaObjGroup::createBuffer(s32 capacity) {
    mCapacity = capacity;
    createBuffer();
}

AreaObj* AreaObjGroup::getAreaObj(const s32 index) const {
    return mBuffer[index];
}

AreaObj* AreaObjGroup::getInVolumeAreaObj(const sead::Vector3f& position) const {
    AreaObj* bestInVolumeAreaObj = nullptr;
    for (s32 i = 0; i < mCount; i++) {
        AreaObj* currentAreaObj = mBuffer[i];
        if (bestInVolumeAreaObj == nullptr ||
            bestInVolumeAreaObj->getPriority() <= currentAreaObj->getPriority()) {
            if (currentAreaObj->isInVolume(position)) {
                bestInVolumeAreaObj = currentAreaObj;
            }
        }
    }
    return bestInVolumeAreaObj;
}

void AreaObjGroup::incrementCount() {
    mCapacity += 1;
}

void AreaObjGroup::registerAreaObj(AreaObj* newAreaObj) {
    if (mCount >= mCapacity) {
        return;
    }
    mBuffer[mCount] = newAreaObj;
    mCount += 1;
}

}  // namespace al
