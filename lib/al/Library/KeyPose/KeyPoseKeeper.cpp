#include "Library/KeyPose/KeyPoseKeeper.h"

#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Project/Joint/KeyPose.h"

namespace al {
KeyPoseKeeper::KeyPoseKeeper() {}

void KeyPoseKeeper::init(const PlacementInfo& info) {
    mKeyPoseCount = calcLinkNestNum(info, "KeyMoveNext") + 1;
    tryGetArg((s32*)&mMoveType, info, "MoveType");
    mKeyPoses = new KeyPose[mKeyPoseCount];

    mKeyPoses[0].init(info);

    PlacementInfo currentInfo = info;
    PlacementInfo nextInfo;
    for (s32 i = 0; i < mKeyPoseCount - 1; i++) {
        getLinksInfo(&nextInfo, currentInfo, "KeyMoveNext");
        mKeyPoses[i + 1].init(nextInfo);
        currentInfo = nextInfo;
    }
}

const KeyPose& KeyPoseKeeper::getKeyPose(s32 idx) const {
    return mKeyPoses[idx];
}

const KeyPose& KeyPoseKeeper::getCurrentKeyPose() const {
    return mKeyPoses[mKeyPoseCurrentIdx];
}

const KeyPose& KeyPoseKeeper::getNextKeyPose() const {
    return mKeyPoses[calcNextPoseIndex()];
}

s32 KeyPoseKeeper::calcNextPoseIndex() const {
    s32 nextIdx;

    if (!mIsForward)
        if (mKeyPoseCurrentIdx < 1)
            nextIdx = mKeyPoseCount - 1;
        else
            nextIdx = mKeyPoseCurrentIdx - 1;
    else if (mKeyPoseCurrentIdx + 1 < mKeyPoseCount)
        nextIdx = mKeyPoseCurrentIdx + 1;
    else
        nextIdx = 0;

    return nextIdx;
}

void KeyPoseKeeper::reset() {
    mKeyPoseCurrentIdx = 0;
    mIsForward = true;
    mIsStop = false;
    mIsRestart = false;
}

void KeyPoseKeeper::next() {
    mKeyPoseCurrentIdx = calcNextPoseIndex();

    switch (mMoveType) {
    case (MoveType::Turn):
        if (!isLastKey())
            return;

        mIsForward = !mIsForward;

        return;
    case (MoveType::Stop):
        if (!isLastKey())
            return;

        mIsStop = true;

        return;
    case (MoveType::Restart):
        if (!isLastKey())
            return;

        mIsRestart = true;

        return;
    default:
        return;
    }
}

bool KeyPoseKeeper::isLastKey() const {
    return mIsForward ? mKeyPoseCurrentIdx + 1 >= mKeyPoseCount : mKeyPoseCurrentIdx < 1;
}

void KeyPoseKeeper::reverse() {
    mIsForward = !mIsForward;

    if (mMoveType != MoveType::Stop)
        return;

    mIsStop = isLastKey();
}

bool KeyPoseKeeper::isFirstKey() const {
    return mKeyPoseCurrentIdx == 0;
}

void KeyPoseKeeper::setMoveTypeLoop() {
    mMoveType = MoveType::Loop;
}

void KeyPoseKeeper::setMoveTypeTurn() {
    mMoveType = MoveType::Turn;
}

void KeyPoseKeeper::setMoveTypeStop() {
    mMoveType = MoveType::Stop;
}

void KeyPoseKeeper::setMoveTypeRestart() {
    mMoveType = MoveType::Restart;
}
}  // namespace al
