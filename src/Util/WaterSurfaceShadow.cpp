#include "Util/WaterSurfaceShadow.h"

#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"

WaterSurfaceShadow::WaterSurfaceShadow(const al::ActorInitInfo& initInfo, const char* archiveName) {
    mActor = new al::LiveActor("水面影モデル");
    al::initChildActorWithArchiveNameNoPlacementInfo(mActor, initInfo, archiveName, nullptr);

    if (al::isMclAnimExist(mActor, "AlphaColor"))
        al::startMclAnim(mActor, "AlphaColor");
    else
        al::startMclAnim(mActor, "AlfaColor");

    mActor->makeActorDead();
    mWaterSurfaceFinder = new al::WaterSurfaceFinder(mActor);
}

bool WaterSurfaceShadow::isAppearShadow() const {
    return al::isAlive(mActor);
}

void WaterSurfaceShadow::update(const sead::Vector3f& position, const sead::Vector3f& gravity,
                                f32 distance) {
    mWaterSurfaceFinder->updateForSurfaceShadow(position, gravity, distance);

    if (!mWaterSurfaceFinder->isFoundSurface() || mWaterSurfaceFinder->getDistance() >= 50.0f ||
        mWaterSurfaceFinder->getDistance() + distance < 15.0f) {
        if (al::isAlive(mActor))
            mActor->kill();
        return;
    }

    f32 animDistance = sead::Mathf::abs(mWaterSurfaceFinder->getDistance());
    f32 animTime = al::easeIn(al::calcRate01(animDistance, 0.0f, mMaxDistance));

    if (al::isDead(mActor))
        mActor->appear();

    al::setMclAnimNormalFrameAndStop(mActor, al::lerpValue(mStartFrame, mEndFrame, animTime));
    al::resetPosition(mActor, gravity * 0.0f + mWaterSurfaceFinder->getSurfacePosition());
}

void WaterSurfaceShadow::disappearShadow() {
    if (!al::isDead(mActor))
        mActor->kill();
}

void WaterSurfaceShadow::setScale(f32 scale) {
    setScale(scale, scale);
}

void WaterSurfaceShadow::setScale(f32 scaleX, f32 scaleZ) {
    al::setScaleX(mActor, scaleX);
    al::setScaleZ(mActor, scaleZ);
}

void WaterSurfaceShadow::setQuat(const sead::Quatf& quat) {
    al::setQuat(mActor, quat);
}
