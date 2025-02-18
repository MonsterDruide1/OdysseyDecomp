#include "Library/KeyPose/KeyPoseKeeper.h"

#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Project/Joint/KeyPose.h"

namespace al {
KeyPoseKeeper::KeyPoseKeeper() {}

void KeyPoseKeeper::init(const al::PlacementInfo& info) {
    mKeyPoseCount = calcLinkNestNum(info, "KeyMoveNext") + 1;
    tryGetArg((s32*)&mMoveType, info, "MoveType");
    mKeyPoses = new KeyPose[mKeyPoseCount];

    mKeyPoses[0].init(info);

    PlacementInfo prevInfo = info;
    PlacementInfo linkInfo;
    for (s32 i = 1; i < mKeyPoseCount - 1; i++) {
        getLinksInfo(&linkInfo, prevInfo, "KeyMoveNext");
        mKeyPoses[i].init(linkInfo);
        prevInfo = linkInfo;
    }
}

const KeyPose& KeyPoseKeeper::getKeyPose(s32 idx) const {
    return mKeyPoses[idx];
}

const KeyPose& KeyPoseKeeper::getCurrentKeyPose() const {
    return mKeyPoses[mKeyPoseCurrentIdx];
}
}  // namespace al
