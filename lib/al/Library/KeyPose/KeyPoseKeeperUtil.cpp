#include "Library/KeyPose/KeyPoseKeeperUtil.h"

#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Project/Joint/KeyPose.h"

namespace al {
KeyPoseKeeper* createKeyPoseKeeper(const ActorInitInfo& info) {
    return createKeyPoseKeeper(*getPlacementInfo(info));
}

KeyPoseKeeper* createKeyPoseKeeper(const PlacementInfo& info) {
    KeyPoseKeeper* keyPoseKeeper = new KeyPoseKeeper();
    keyPoseKeeper->init(info);

    return keyPoseKeeper;
}

void resetKeyPose(KeyPoseKeeper* keyPoseKeeper) {
    keyPoseKeeper->reset();
}

void nextKeyPose(KeyPoseKeeper* keyPoseKeeper) {
    keyPoseKeeper->next();
}

void restartKeyPose(KeyPoseKeeper* keyPoseKeeper, sead::Vector3f* pos, sead::Quatf* orientation) {
    resetKeyPose(keyPoseKeeper);

    KeyPose keyPose = keyPoseKeeper->getKeyPose(0);

    if (pos != nullptr)
        pos->set(keyPose.getTrans());

    if (orientation != nullptr)
        orientation->set(keyPose.getQuat());
}
}  // namespace al
