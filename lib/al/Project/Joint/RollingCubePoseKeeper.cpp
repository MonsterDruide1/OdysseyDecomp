#include "Project/Joint/RollingCubePoseKeeper.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Project/Joint/RollingCubePoseKeeperUtil.h"
#include "Project/RollingCubePose.h"

namespace al {

RollingCubePoseKeeper::RollingCubePoseKeeper() = default;

void RollingCubePoseKeeper::setCubeSize(const sead::BoundBox3f& cubeSize) {
    mCubeSize = cubeSize;
}

bool RollingCubePoseKeeper::isMoveTypeTurn() const {
    return mMoveType == MoveType::Turn || mMoveType == MoveType::All;
}

bool RollingCubePoseKeeper::isMoveTypeLoop() const {
    return mMoveType == MoveType::Loop || mMoveType == MoveType::All;
}

void RollingCubePoseKeeper::init(const ActorInitInfo& initInfo) {
    tryGetArg((s32*)&mMoveType, initInfo, "MoveType");
    s32 linkNextNum = calcLinkNestNum(initInfo, "KeyMoveNext");

    switch (mMoveType) {
    case MoveType::All:
        if (linkNextNum != 0)
            mPoseCount = linkNextNum * 2;
        else
            mPoseCount = 1;
        break;
    case MoveType::Turn:
        mPoseCount = linkNextNum * 2 + 1;
        break;
    default:
        mPoseCount = linkNextNum + 1;
        break;
    }

    mRollingCubePoses = new RollingCubePose[mPoseCount];

    mRollingCubePoses[0].setCubeSize(mCubeSize);
    mRollingCubePoses[0].init(*initInfo.placementInfo);

    if (mMoveType == MoveType::Turn) {
        mRollingCubePoses[mPoseCount - 1].setCubeSize(mCubeSize);
        mRollingCubePoses[mPoseCount - 1].init(*initInfo.placementInfo);
    }

    PlacementInfo currentPlacementInfo = *initInfo.placementInfo;
    PlacementInfo nextPlacementInfo;
    for (s32 i = 0; i < linkNextNum; i++) {
        getLinksInfo(&nextPlacementInfo, currentPlacementInfo, "KeyMoveNext");
        mRollingCubePoses[i + 1].setCubeSize(mCubeSize);
        mRollingCubePoses[i + 1].init(nextPlacementInfo);

        // When move type is turn all steps are repeated but backwards
        s32 mirrorIndexOffset = mMoveType == MoveType::Turn ? 2 : 1;
        if (i < linkNextNum - 1 && isMoveTypeTurn()) {
            s32 mirrorIndex = mPoseCount - i - mirrorIndexOffset;
            mRollingCubePoses[mirrorIndex].setCubeSize(mCubeSize);
            mRollingCubePoses[mirrorIndex].init(nextPlacementInfo);
        }
        currentPlacementInfo = nextPlacementInfo;
    }

    for (s32 i = 0; i < mPoseCount - 1; i++)
        mRollingCubePoses[i].setNextCubePose(&mRollingCubePoses[i + 1]);

    if (isMoveTypeLoop())
        mRollingCubePoses[mPoseCount - 1].setNextCubePose(&mRollingCubePoses[0]);
}

bool RollingCubePoseKeeper::nextKey() {
    mCurrentKeyIndex++;

    if (mCurrentKeyIndex < mPoseCount)
        return true;

    if (isMoveTypeLoop()) {
        mCurrentKeyIndex = 0;
        return true;
    }

    mCurrentKeyIndex = mPoseCount - 1;
    return false;
}

void RollingCubePoseKeeper::setStart() {
    mCurrentKeyIndex = 0;
}

void RollingCubePoseKeeper::setKeyIndex(s32 index) {
    mCurrentKeyIndex = index;
}

const RollingCubePose& RollingCubePoseKeeper::getCurrentPose() const {
    return mRollingCubePoses[mCurrentKeyIndex];
}

const RollingCubePose& RollingCubePoseKeeper::getPose(s32 index) const {
    return mRollingCubePoses[index];
}

void RollingCubePoseKeeper::calcBoundingBoxCenter(sead::Vector3f* center, const sead::Quatf& quat,
                                                  const sead::Vector3f& trans) const {
    center->set(mCubeSize.getCenter());

    sead::Matrix34f mtx;
    mtx.makeQT(quat, trans);

    center->mul(mtx);
}

}  // namespace al
