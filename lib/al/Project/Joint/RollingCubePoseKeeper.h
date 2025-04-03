#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>

namespace al {
class RollingCubePose;
class PlacementInfo;
class LiveActor;
struct ActorInitInfo;

// TODO: Complete RollingCubePoseKeeper
class RollingCubePoseKeeper {
public:
    RollingCubePoseKeeper();

    void setCubeSize(const sead::BoundBox3f& cubeSize);
    bool isMoveTypeTurn() const;
    bool isMoveTypeLoop() const;
    void init(const ActorInitInfo& info);
    bool nextKey();
    void setStart();
    void setKeyIndex(s32 index);
    RollingCubePose* getCurrentPose() const;
    RollingCubePose* getPose(s32 index) const;
    void calcBoundingBoxCenter(sead::Vector3f*, const sead::Quatf&, const sead::Vector3f&) const;

private:
    RollingCubePose** mRollingCubePoses;
    s32 mRollingCubePoseCount;
    s32 mCurrentKey;
    sead::BoundBox3f mCubeSize;
    u32 mMoveType;
};

void fittingToCurrentKeyBoundingBox(sead::Quatf* quat, sead::Vector3f* trans,
                                    const RollingCubePoseKeeper* rollingCubePoseKeeper);
void calcCurrentKeyQT(sead::Quatf* outQuat, sead::Vector3f* outTrans,
                      const RollingCubePoseKeeper* rollingCubePoseKeeper, const sead::Quatf& quat,
                      const sead::Vector3f& trans, f32 rate);
bool nextRollingCubeKey(RollingCubePoseKeeper* rollingCubePoseKeeper);
bool isMovementCurrentKeyRotate(const RollingCubePoseKeeper* rollingCubePoseKeeper);
const PlacementInfo& getCurrentKeyPlacementInfo(const RollingCubePoseKeeper* rollingCubePoseKeeper);
void calcMtxLandEffect(sead::Matrix34f* effectMtx,
                       const RollingCubePoseKeeper* rollingCubePoseKeeper, const sead::Quatf& quat,
                       const sead::Vector3f& trans);
void calcRollingCubeClippingInfo(sead::Vector3f* clippingTrans, f32* clippingRadius,
                                 const RollingCubePoseKeeper* rollingCubePoseKeeper, f32 offset);
RollingCubePoseKeeper* createRollingCubePoseKeeper(const LiveActor* actor,
                                                   const ActorInitInfo& info);
RollingCubePoseKeeper* createRollingCubePoseKeeper(const sead::BoundBox3f& boundBox,
                                                   const ActorInitInfo& info);
}  // namespace al
