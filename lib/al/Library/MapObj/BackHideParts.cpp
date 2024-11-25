#include "Library/MapObj/BackHideParts.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {
BackHideDitherAnimator::BackHideDitherAnimator(LiveActor* actor)
    : DitherAnimator("背面ディザアニメーター"), mActor(actor) {}

void BackHideDitherAnimator::update() {
    sead::Vector3f cameraFront;
    calcCameraFront(&cameraFront, mActor, 0);

    sead::Vector3f cameraOffset = getCameraPos(mActor, 0) - getTrans(mActor);

    sead::Vector3f actorFront;
    calcFrontDir(&actorFront, mActor);

    f32 actorAlpha = sead::Mathf::clamp(
        lerpValue(normalize(calcAngleDegree(cameraFront, actorFront), 60.0f, 90.0f), 1.0f,
                  normalize(cameraOffset.length(), 10000.0f, 12000.0f)),
        0.0f, 1.0f);

    setModelAlphaMask(mActor, actorAlpha);

    if (isNearZero(actorAlpha, 0.001)) {
        hideModelIfShow(mActor);

        return;
    }

    showModelIfHide(mActor);
}

BackHideParts::BackHideParts(const char* name) : LiveActor(name) {}

void BackHideParts::init(const ActorInitInfo& info) {
    const char* suffix = nullptr;
    tryGetStringArg(&suffix, info, "Suffix");
    initMapPartsActor(this, info, suffix);

    mBackHideDitherAnimator = new BackHideDitherAnimator(this);
    getModelKeeper()->setDitherAnimator(mBackHideDitherAnimator);

    trySyncStageSwitchAppearAndKill(this);
}
}  // namespace al
