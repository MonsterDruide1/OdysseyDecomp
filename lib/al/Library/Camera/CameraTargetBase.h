#pragma once

#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

namespace al {

class CameraTargetBase : public IUseHioNode {
public:
    CameraTargetBase();

    virtual const char* getTargetName() const = 0;
    virtual void calcTrans(sead::Vector3f* trans) const = 0;

    virtual void calcSide(sead::Vector3f* side) const {}

    virtual void calcUp(sead::Vector3f* up) const {}

    virtual void calcFront(sead::Vector3f* front) const {}

    virtual void calcGravity(sead::Vector3f* gravity) const { *gravity = {0.0f, -1.0f, 0.0f}; }

    virtual void calcVelocity(sead::Vector3f* velocity) const {}

    virtual bool isCollideGround() const { return false; }

    virtual bool isInWater() const { return false; }

    virtual bool isInMoonGravity() const { return false; }

    virtual bool isClimbPole() const { return false; }

    virtual bool isGrabCeil() const { return false; }

    virtual bool isWallCatch() const { return false; }

    virtual bool isInvalidMoveByInput() const { return false; }

    virtual bool isEnableEndAfterInterpole() const { return false; }

    virtual void update() {}

    virtual f32 getRequestDistance() const { return -1.0f; }

private:
    bool mIsActiveTarget = false;
};

static_assert(sizeof(CameraTargetBase) == 0x10);

}  // namespace al
