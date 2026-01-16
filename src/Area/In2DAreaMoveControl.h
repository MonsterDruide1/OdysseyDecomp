#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>

namespace al {
class AreaObj;
}

class In2DAreaMoveControl {
public:
    In2DAreaMoveControl();

    void update(const sead::FixedPtrArray<al::AreaObj, 8>&);
    void calcGravityDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&) const;
    void calcLockDir(sead::Vector3f*, f32*, const sead::Vector3f&) const;
    void calcLastLockDir(sead::Vector3f*, const sead::Vector3f&) const;
    bool isNearSnapSurface(const sead::Vector3f&, f32) const;
    bool isLastAreaPushOutOpposite() const;
    bool isLastAreaFaceToPushOut() const;

private:
    void* filler[0xA0 / 8];
};

static_assert(sizeof(In2DAreaMoveControl) == 0xA0);
