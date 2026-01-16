#include "Project/Joint/RollingCubePoseKeeperUtil.h"

#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Project/Joint/RollingCubePoseKeeper.h"
#include "Project/RollingCubePose.h"

namespace al {

RollingCubePoseKeeper* createRollingCubePoseKeeper(const LiveActor* actor,
                                                   const ActorInitInfo& initInfo) {
    sead::BoundBox3f modelBoundBox;
    calcModelBoundingBox(&modelBoundBox, actor);
    return createRollingCubePoseKeeper(modelBoundBox, initInfo);
}

RollingCubePoseKeeper* createRollingCubePoseKeeper(const sead::BoundBox3f& cubeSize,
                                                   const ActorInitInfo& initInfo) {
    RollingCubePoseKeeper* keeper = new RollingCubePoseKeeper();
    keeper->setCubeSize(cubeSize);
    keeper->init(initInfo);
    return keeper;
}

bool nextRollingCubeKey(RollingCubePoseKeeper* keeper) {
    return keeper->nextKey();
}

void setStartRollingCubeKey(RollingCubePoseKeeper* keeper) {
    keeper->setStart();
}

void setRollingCubeKeyIndex(RollingCubePoseKeeper* keeper, s32 index) {
    keeper->setKeyIndex(index);
}

bool isMoveTypeLoopRollingCube(const RollingCubePoseKeeper* keeper) {
    return keeper->isMoveTypeLoop();
}

void fittingToCurrentKeyBoundingBox(sead::Quatf* outQuat, sead::Vector3f* outTrans,
                                    const RollingCubePoseKeeper* keeper) {
    keeper->getCurrentPose().fittingToBoundingBox(outQuat, outTrans);
}

void calcCurrentKeyQT(sead::Quatf* outQuat, sead::Vector3f* outTrans,
                      const RollingCubePoseKeeper* keeper, const sead::Quatf& quat,
                      const sead::Vector3f& trans, f32 rate) {
    keeper->getCurrentPose().calcRotateQT(outQuat, outTrans, quat, trans, rate);
}

void getCurrentKeyQT(sead::Quatf* outQuat, sead::Vector3f* outTrans,
                     const RollingCubePoseKeeper* keeper) {
    const RollingCubePose& rollingCubePose = keeper->getCurrentPose();
    if (outQuat)
        outQuat->set(rollingCubePose.getQuat());
    if (outTrans)
        outTrans->set(rollingCubePose.getTrans());
}

f32 getCurrentKeyRotateDegree(const RollingCubePoseKeeper* keeper) {
    return keeper->getCurrentPose().getRotateDegree();
}

const sead::Vector3f& getCurrentKeySlideVec(const RollingCubePoseKeeper* keeper) {
    return keeper->getCurrentPose().getSlideVec();
}

s32 getCurrentKeyIndex(const RollingCubePoseKeeper* keeper) {
    return keeper->getCurrentKeyIndex();
}

const PlacementInfo& getCurrentKeyPlacementInfo(const RollingCubePoseKeeper* keeper) {
    return keeper->getCurrentPose().getPlacementInfo();
}

bool isMovementCurrentKeyRotate(const RollingCubePoseKeeper* keeper) {
    return keeper->getCurrentPose().isMovementRotate();
}

bool isMovementCurrentKeySlide(const RollingCubePoseKeeper* keeper) {
    return keeper->getCurrentPose().isMovementSlide();
}

f32 calcDistanceCurrentKeyRotateCenterToBoxCenter(const RollingCubePoseKeeper* keeper) {
    const RollingCubePose& rollingCubePose = keeper->getCurrentPose();

    sead::Vector3f center;
    rollingCubePose.calcBoundingBoxCenter(&center);

    sead::Vector3f distance = center - rollingCubePose.getRotateCenter();

    if (!isNearZero(rollingCubePose.getRotateAxis()))
        verticalizeVec(&distance, rollingCubePose.getRotateAxis(), distance);

    return distance.length();
}

}  // namespace al
