#pragma once

#include <math/seadVector.h>
#include <math/seadQuat.h>
#include <math/seadMatrix.h>

namespace al {
class LiveActor;
void updatePoseTrans(LiveActor*, const sead::Vector3f&);
void updatePoseRotate(LiveActor*, const sead::Vector3f&);
void updatePoseQuat(LiveActor*, const sead::Quatf&);
void updatePoseMtx(LiveActor*, const sead::Matrix34f&);
const sead::Vector3f& getTrans(const LiveActor*);
sead::Vector3f* getTransPtr(LiveActor*);
}
