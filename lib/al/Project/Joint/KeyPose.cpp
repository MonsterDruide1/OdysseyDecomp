#include "Project/Joint/KeyPose.h"

#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

namespace al {

KeyPose::KeyPose() = default;

void KeyPose::init(const PlacementInfo& info) {
    tryGetQuat(&mQuat, info);
    tryGetTrans(&mTrans, info);
    mPlacementInfo = new PlacementInfo(info);
}

}  // namespace al
