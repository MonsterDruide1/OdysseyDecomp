#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Camera/CameraTargetBase.h"

namespace al {
class CameraPoser;
struct CameraStartInfo;
class LiveActor;

class ActorMatrixCameraTarget : public CameraTargetBase {
public:
    ActorMatrixCameraTarget(const LiveActor*, const sead::Matrix34f*);

    void calcTrans(sead::Vector3f* trans) const override;
    void calcSide(sead::Vector3f* side) const override;
    void calcUp(sead::Vector3f* up) const override;
    void calcFront(sead::Vector3f* front) const override;
    void calcVelocity(sead::Vector3f* velocity) const override;

private:
    char filler[0x20];
};

}  // namespace al
