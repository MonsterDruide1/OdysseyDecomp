#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class IUseCamera;
class LiveActor;

void addPlayerAccelStick(LiveActor*, sead::Vector3f*, f32, s32, const sead::Matrix34f*);
void addPlayerAccelStickGravity(LiveActor*, sead::Vector3f*, f32, const sead::Vector3f&, s32,
                                const sead::Matrix34f*);
void addPlayerAccelInputGravity(LiveActor*, sead::Vector3f*, const sead::Vector2f&, f32,
                                const sead::Vector3f&, const sead::Matrix34f*);
bool tryCalcTouchWorldPosPlane(const IUseCamera*, sead::Vector3f*, const sead::Vector3f&,
                               const sead::Vector3f&);
bool tryCalcTouchWorldPosPlaneXZ(const IUseCamera*, sead::Vector3f*, f32);
}  // namespace al
