#pragma once

#include <math/seadVector.h>

namespace al {

class ICameraInput {
public:
    virtual void calcInputStick(sead::Vector2f* inputStick) const;
    virtual bool isTriggerReset() const;
    virtual bool isHoldZoom() const;

    virtual bool tryCalcSnapShotMoveStick(sead::Vector2f* moveStick) const { return false; }

    virtual bool isHoldSnapShotZoomIn() const;
    virtual bool isHoldSnapShotZoomOut() const;

    virtual bool isHoldSnapShotRollLeft() const { return false; }

    virtual bool isHoldSnapShotRollRight() const { return false; }

    virtual void calcGyroPose(sead::Vector3f* dir1, sead::Vector3f* dir2,
                              sead::Vector3f* dir3) const {
        if (dir1)
            dir1->set(sead::Vector3f::ex);
        if (dir2)
            dir2->set(sead::Vector3f::ey);
        if (dir3)
            dir3->set(sead::Vector3f::ez);
    }
};

}  // namespace al
