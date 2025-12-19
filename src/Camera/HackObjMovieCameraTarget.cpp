#include "Camera/HackObjMovieCameraTarget.h"

#include <math/seadMathCalcCommon.h>

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(HackObjMovieCameraTarget, Invalid);
NERVE_IMPL(HackObjMovieCameraTarget, Center);
NERVE_IMPL(HackObjMovieCameraTarget, CenterToHackObj);
NERVE_IMPL(HackObjMovieCameraTarget, CenterFix);
NERVE_IMPL(HackObjMovieCameraTarget, CenterFixToHackObj);
NERVE_IMPL(HackObjMovieCameraTarget, HackObj);

NERVES_MAKE_NOSTRUCT(HackObjMovieCameraTarget, Invalid, Center, CenterToHackObj, CenterFix,
                     CenterFixToHackObj, HackObj);
}  // namespace

HackObjMovieCameraTarget::HackObjMovieCameraTarget(const al::LiveActor* hackActor)
    : ActorCameraTarget(hackActor, 0, nullptr) {
    mNerveKeeper = new al::NerveKeeper(this, &Invalid, 0);
}

const char* HackObjMovieCameraTarget::getTargetName() const {
    return "映像撮影用憑依オブジェカメラターゲット";
}

void HackObjMovieCameraTarget::calcTrans(sead::Vector3f* trans) const {
    if (al::isNerve(this, &Center) || al::isNerve(this, &CenterToHackObj)) {
        sead::Vector3f center = rs::getPlayerPos(getActor()) * 0.5f +
                                sead::Vector3f{0.0f, 0.0f, 0.0f} + al::getTrans(getActor()) * 0.5f;

        if (al::isNerve(this, &Center))
            trans->set(center);
        else
            al::lerpVec(trans, center, al::getTrans(getActor()),
                        al::calcNerveRate(this, mTransitionTime));

    } else if (al::isNerve(this, &CenterFix) || al::isNerve(this, &CenterFixToHackObj)) {
        sead::Vector3f center =
            mFixPlayerPos * 0.5f + sead::Vector3f{0.0f, 0.0f, 0.0f} + mFixHackPos * 0.5f;

        if (al::isNerve(this, &CenterFix))
            trans->set(center);
        else
            al::lerpVec(trans, center, al::getTrans(getActor()),
                        al::calcNerveRate(this, mTransitionTime));
    } else {
        trans->set(al::getTrans(getActor()));
    }
}

void HackObjMovieCameraTarget::updateHack(bool isInHack) {
    mIsInHack = isInHack;
    mNerveKeeper->update();
}

void HackObjMovieCameraTarget::changeTargetToHackObj() {
    f32 distanceBetweenActors = (al::getTrans(getActor()) - rs::getPlayerPos(getActor())).length();

    f32 timeToTranstion = sead::Mathf::clampMin(distanceBetweenActors - 200.0f, 0.0f) / 30.0f;
    f32 half = timeToTranstion >= 0 ? 0.5f : -0.5f;  // in what world is this negative??

    s32 time = (s32)(timeToTranstion + half) + 15;
    s32 clampedTime = sead::Mathi::clampMax(time, 30);
    mTransitionTime = (s32)(timeToTranstion + half) < 0.0f ? 15 : clampedTime;

    if (al::isNerve(this, &CenterFix))
        al::setNerve(this, &CenterFixToHackObj);
    else
        al::setNerve(this, &CenterToHackObj);
}

void HackObjMovieCameraTarget::exeInvalid() {}

void HackObjMovieCameraTarget::exeCenter() {
    if (mIsInHack)
        changeTargetToHackObj();
}

void HackObjMovieCameraTarget::exeCenterFix() {
    if (mIsInHack)
        changeTargetToHackObj();
}

void HackObjMovieCameraTarget::exeCenterToHackObj() {
    al::setNerveAtGreaterEqualStep(this, &HackObj, mTransitionTime);
}

void HackObjMovieCameraTarget::exeCenterFixToHackObj() {
    al::setNerveAtGreaterEqualStep(this, &HackObj, mTransitionTime);
}

void HackObjMovieCameraTarget::exeHackObj() {}

al::CameraDirector* HackObjMovieCameraTarget::getCameraDirector() const {
    return getActor()->getCameraDirector();
}
