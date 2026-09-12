#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Camera/CameraSubTargetBase.h"

namespace al {

class TransCameraSubTarget : public CameraSubTargetBase {
public:
    const char* getTargetName() const override { return mName; }

    void calcTrans(sead::Vector3f* trans) const override { trans->set(*mTrans); }

private:
    const char* mName;
    sead::Vector3f* mTrans;
};

static_assert(sizeof(TransCameraSubTarget) == 0x28);

}  // namespace al
