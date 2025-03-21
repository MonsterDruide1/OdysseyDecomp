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

    void calcTrans(sead::Vector3f*) const;
    void calcSide(sead::Vector3f*) const;
    void calcUp(sead::Vector3f*) const;
    void calcFront(sead::Vector3f*) const;
    void calcVelocity(sead::Vector3f*) const;

private:
    char filler[0x20];
};

}  // namespace al
