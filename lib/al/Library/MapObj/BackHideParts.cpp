#include "Library/MapObj/BackHideParts.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {
BackHideDitherAnimator::BackHideDitherAnimator(LiveActor* actor)
    : DitherAnimator("背面ディザアニメーター"), mActor(actor) {}

void BackHideDitherAnimator::update() {
    sead::Vector3f cameraFront;
    calcCameraFront(&cameraFront, mActor, 0);

    f32 cameraOffset = (getCameraPos(mActor, 0) - getTrans(mActor)).length();

    sead::Vector3f actorFront;
    calcFrontDir(&actorFront, mActor);

    f32 angleDegree = normalize(calcAngleDegree(cameraFront, actorFront), 60.0f, 90.0f);
    cameraOffset = normalize(cameraOffset, 10000.0f, 12000.0f);

    f32 actorAlpha = sead::Mathf::clamp(lerpValue(angleDegree, 1.0f, cameraOffset), 0.0f, 1.0f);
    setModelAlphaMask(mActor, actorAlpha);

    if (isNearZero(actorAlpha, 0.001)) {
        hideModelIfShow(mActor);

        return;
    }

    showModelIfHide(mActor);
}

BackHideParts::BackHideParts(const char* name) : LiveActor(name) {}

// NON_MATCHING: regswap (https://decomp.me/scratch/j5tRy)
void BackHideParts::init(const ActorInitInfo& info) {
    const char* suffix = nullptr;
    tryGetStringArg(&suffix, info, "Suffix");
    initMapPartsActor(this, info, suffix);

    mBackHideDitherAnimator = new BackHideDitherAnimator(this);
    getModelKeeper()->setDitherAnimator(mBackHideDitherAnimator);

    trySyncStageSwitchAppearAndKill(this);
}
}  // namespace al
