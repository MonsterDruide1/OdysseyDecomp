#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class PlacementInfo;
class RollingCubePoseKeeper;
class LiveActor;
struct ActorInitInfo;

RollingCubePoseKeeper* createRollingCubePoseKeeper(const LiveActor* actor,
                                                   const ActorInitInfo& initInfo);
RollingCubePoseKeeper* createRollingCubePoseKeeper(const sead::BoundBox3f& cubeSize,
                                                   const ActorInitInfo& initInfo);
bool nextRollingCubeKey(RollingCubePoseKeeper* keeper);
void setStartRollingCubeKey(RollingCubePoseKeeper* keeper);
void setRollingCubeKeyIndex(RollingCubePoseKeeper* keeper, s32 index);
bool isMoveTypeLoopRollingCube(const RollingCubePoseKeeper* keeper);
void fittingToCurrentKeyBoundingBox(sead::Quatf* outQuat, sead::Vector3f* outTrans,
                                    const RollingCubePoseKeeper* keeper);
void calcCurrentKeyQT(sead::Quatf* outQuat, sead::Vector3f* outTrans,
                      const RollingCubePoseKeeper* keeper, const sead::Quatf& quat,
                      const sead::Vector3f& trans, f32 rate);
void getCurrentKeyQT(sead::Quatf* outQuat, sead::Vector3f* outTrans,
                     const RollingCubePoseKeeper* keeper);
f32 getCurrentKeyRotateDegree(const RollingCubePoseKeeper* keeper);
const sead::Vector3f& getCurrentKeySlideVec(const RollingCubePoseKeeper* keeper);
s32 getCurrentKeyIndex(const RollingCubePoseKeeper* keeper);
const PlacementInfo& getCurrentKeyPlacementInfo(const RollingCubePoseKeeper* keeper);
bool isMovementCurrentKeyRotate(const RollingCubePoseKeeper* keeper);
bool isMovementCurrentKeySlide(const RollingCubePoseKeeper* keeper);
f32 calcDistanceCurrentKeyRotateCenterToBoxCenter(const RollingCubePoseKeeper* keeper);
void calcMtxLandEffect(sead::Matrix34f* effectMtx, const RollingCubePoseKeeper* keeper,
                       const sead::Quatf& quat, const sead::Vector3f& trans);
void calcRollingCubeClippingInfo(sead::Vector3f* clippingTrans, f32* clippingRadius,
                                 const RollingCubePoseKeeper* keeper, f32 offset);
}  // namespace al
