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
    if (!mIsGoingToEnd)
        if (mKeyPoseCurrentIdx < 1)
            return mKeyPoseCount - 1;
        else
            return mKeyPoseCurrentIdx - 1;
    else if (mKeyPoseCurrentIdx + 1 < mKeyPoseCount)
        return mKeyPoseCurrentIdx + 1;

    return 0;
}

void KeyPoseKeeper::reset() {
    mKeyPoseCurrentIdx = 0;
    mIsGoingToEnd = true;
    mIsStop = false;
    mIsRestart = false;
}

void KeyPoseKeeper::next() {
    mKeyPoseCurrentIdx = calcNextPoseIndex();

    switch (mMoveType) {
    case MoveType::Turn:
        if (isLastKey())
            mIsGoingToEnd = !mIsGoingToEnd;

        break;
    case MoveType::Stop:
        if (isLastKey())
            mIsStop = true;

        break;
    case MoveType::Restart:
        if (isLastKey())
            mIsRestart = true;

        break;
    default:
        break;
    }
}

bool KeyPoseKeeper::isLastKey() const {
    return mIsGoingToEnd ? mKeyPoseCurrentIdx + 1 >= mKeyPoseCount : mKeyPoseCurrentIdx < 1;
}

void KeyPoseKeeper::reverse() {
    mIsGoingToEnd = !mIsGoingToEnd;

    if (mMoveType == MoveType::Stop)
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
