#include "Library/Play/Camera/CameraVerticalAbsorber.h"

#include <gfx/seadViewport.h>

#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraStartInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Yaml/ByamlUtil.h"

namespace {
using namespace al;
NERVE_IMPL(CameraVerticalAbsorber, FollowGround);
NERVE_IMPL(CameraVerticalAbsorber, FollowAbsolute);
NERVE_IMPL_(CameraVerticalAbsorber, FollowClimbPoleNoInterp, FollowClimbPole);
NERVE_IMPL_(CameraVerticalAbsorber, FollowSlow, Follow);
NERVE_IMPL(CameraVerticalAbsorber, Absorb);
NERVE_IMPL(CameraVerticalAbsorber, Follow);
NERVE_IMPL(CameraVerticalAbsorber, FollowClimbPole);

NERVES_MAKE_STRUCT(CameraVerticalAbsorber, FollowGround, FollowAbsolute, FollowClimbPoleNoInterp,
                   FollowSlow, Absorb, Follow, FollowClimbPole);
}  // namespace

namespace al {
CameraVerticalAbsorber::CameraVerticalAbsorber(const CameraPoser* cameraParent,
                                               bool isCameraPosAbsorb)
    : NerveExecutor("カメラの縦パン"), mCameraPoser(cameraParent),
      mIsNoCameraPosAbsorb(isCameraPosAbsorb) {
    initNerve(&NrvCameraVerticalAbsorber.FollowGround);
}

void CameraVerticalAbsorber::load(const ByamlIter& iter) {
    ByamlIter VerticalAbsorbIter;
    if (!iter.tryGetIterByKey(&VerticalAbsorbIter, "VerticalAbsorb"))
        return;

    tryGetByamlF32(&mAbsorbScreenPosUp, VerticalAbsorbIter, "AbsorbScreenPosUp");
    tryGetByamlF32(&mAbsorbScreenPosDown, VerticalAbsorbIter, "AbsorbScreenPosDown");
    tryGetByamlF32(&mHighJumpJudgeSpeedV, VerticalAbsorbIter, "HighJumpJudgeSpeedV");

    ByamlIter AdvanceAbsorbUpIter;
    if (VerticalAbsorbIter.tryGetIterByKey(&AdvanceAbsorbUpIter, "AdvanceAbsorbUp")) {
        mIsAdvanceAbsorbUp = true;
        mAdvanceAbsorbScreenPosUp =
            getByamlKeyFloat(AdvanceAbsorbUpIter, "AdvanceAbsorbScreenPosUp");
    }
}

void CameraVerticalAbsorber::start(const sead::Vector3f& pos, const CameraStartInfo& info) {
    alCameraPoserFunction::calcTargetFront(&mPrevTargetFront, mCameraPoser);

    mTargetInterp = {0.0f, 0.0f, 0.0f};
    mPrevTargetTrans.set(pos);

    if (!isValid() || alCameraPoserFunction::isPlayerTypeNotTouchGround(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowAbsolute);

    if (alCameraPoserFunction::isTargetClimbPole(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowClimbPoleNoInterp);

    if (alCameraPoserFunction::isTargetGrabCeil(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowSlow);

    if (!info.isGrounded || alCameraPoserFunction::isTargetCollideGround(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowGround);

    mPrevTargetTrans.set(alCameraPoserFunction::getPreLookAtPos(mCameraPoser));

    const CameraPoser* poser = mCameraPoser;
    sead::Vector3f target = {0.0f, 0.0f, 0.0f};
    alCameraPoserFunction::calcTargetGravity(&target, poser);

    mTargetInterp = pos - mPrevTargetTrans;
    parallelizeVec(&mTargetInterp, target, mTargetInterp);
    setNerve(this, &NrvCameraVerticalAbsorber.Absorb);
}

bool CameraVerticalAbsorber::isValid() const {
    return !_1aa && !mIsInvalidated;
}

// NON_MATCHING: https://decomp.me/scratch/JdemU
void CameraVerticalAbsorber::update() {
    if (mIsStopUpdate)
        return;
    const CameraPoser* poser = mCameraPoser;
    sead::Vector3f gravity = {0.0f, 0.0f, 0.0f};
    alCameraPoserFunction::calcTargetGravity(&gravity, poser);
    mTargetInterp = poser->getTargetTrans() - mPrevTargetTrans;
    parallelizeVec(&mTargetInterp, gravity, mTargetInterp);

    mLookAtCamera.setPos(mCameraPoser->getPosition());
    mLookAtCamera.setAt(mCameraPoser->getTargetTrans());
    mLookAtCamera.setUp(mCameraPoser->getCameraUp());
    mLookAtCamera.normalizeUp();
    makeLookAtCamera(&mLookAtCamera);
    mLookAtCamera.updateViewMatrix();

    mProjection.set(alCameraPoserFunction::getNear(mCameraPoser),
                    alCameraPoserFunction::getFar(mCameraPoser),
                    sead::Mathf::deg2rad(mCameraPoser->getFovyDegree()),
                    alCameraPoserFunction::getAspect(mCameraPoser));
    alCameraPoserFunction::calcTargetFront(&mTargetFront, mCameraPoser);

    if (!isNerve(this, &NrvCameraVerticalAbsorber.FollowGround) &&
        alCameraPoserFunction::isTargetCollideGround(mCameraPoser))
        setNerve(this, &NrvCameraVerticalAbsorber.FollowGround);
    if (!isNerve(this, &NrvCameraVerticalAbsorber.FollowAbsolute) &&
        alCameraPoserFunction::isPlayerTypeNotTouchGround(mCameraPoser))
        setNerve(this, &NrvCameraVerticalAbsorber.FollowAbsolute);
    updateNerve();
    sead::Vector3f prevTargetTrans = {0.0f, 0.0f, 0.0f};
    if (!mIsKeepInFrame) {
        prevTargetTrans = mTargetInterp;
    } else {
        sead::Vector3f offsetTrans = {0.0f, 0.0f, 0.0f};
        alCameraPoserFunction::calcTargetTransWithOffset(&offsetTrans, mCameraPoser);
        alCameraPoserFunction::calcOffsetCameraKeepInFrameV(
            &gravity, &mLookAtCamera, offsetTrans, mCameraPoser, mKeepInFrameOffsetUp,
            alCameraPoserFunction::isPlayerTypeHighJump(mCameraPoser) ? 300.0f :
                                                                        mKeepInFrameOffsetDown);
        prevTargetTrans = mTargetInterp - gravity;
    }
    mPrevTargetTrans = mCameraPoser->getTargetTrans() - prevTargetTrans;
    mPrevTargetFront = mTargetFront;
}

void CameraVerticalAbsorber::makeLookAtCamera(sead::LookAtCamera* lookAtCamera) const {
    if (!isValid())
        return;

    lookAtCamera->setAt(lookAtCamera->getAt() - mTargetInterp);
    if (!mIsNoCameraPosAbsorb)
        lookAtCamera->setPos(lookAtCamera->getPos() - mTargetInterp);
}

void CameraVerticalAbsorber::liberateAbsorb() {
    if (isNerve(this, &NrvCameraVerticalAbsorber.Absorb))
        setNerve(this, &NrvCameraVerticalAbsorber.Follow);
}

bool CameraVerticalAbsorber::isAbsorbing() const {
    return isValid() && isNerve(this, &NrvCameraVerticalAbsorber.Absorb);
}

void CameraVerticalAbsorber::invalidate() {
    mIsInvalidated = true;
    if (!isNerve(this, &NrvCameraVerticalAbsorber.FollowAbsolute))
        setNerve(this, &NrvCameraVerticalAbsorber.FollowAbsolute);
}

void CameraVerticalAbsorber::tryResetAbsorbVecIfInCollision(const sead::Vector3f& pos) {
    if (!alCameraPoserFunction::checkFirstCameraCollisionArrow(nullptr, nullptr, mCameraPoser,
                                                               pos + mTargetInterp, -mTargetInterp))
        return;

    mTargetInterp = {0.0f, 0.0f, 0.0f};
    if (alCameraPoserFunction::isTargetCollideGround(mCameraPoser))
        setNerve(this, &NrvCameraVerticalAbsorber.FollowGround);
    else
        setNerve(this, &NrvCameraVerticalAbsorber.Follow);
}

inline f32 getSpeed(IUseNerve* nerve, const CameraPoser* camera) {
    if (alCameraPoserFunction::isPlayerTypeHighJump(camera))
        return 0.02f;
    if (isNerve(nerve, &NrvCameraVerticalAbsorber.FollowSlow))
        return 0.02f;
    if (isNerve(nerve, &NrvCameraVerticalAbsorber.FollowClimbPoleNoInterp))
        return 0.3f;
    if (isNerve(nerve, &NrvCameraVerticalAbsorber.FollowClimbPole))
        return calcNerveValue(nerve, 60, 0.05f, 0.3f);
    return 0.05f;
}

void updateFollowSpeed(f32* outValue, f32 rateA, f32 rateB, IUseNerve* nerve,
                       const CameraPoser* camera) {
    f32 initialValue = *outValue;

    f32 endValue = lerpValue(initialValue, getSpeed(nerve, camera), rateA);
    *outValue = lerpValue(*outValue, endValue, rateB);
}

void CameraVerticalAbsorber::exeFollowAbsolute() {
    mTargetInterp *= 0.8f;
}

}  // namespace al
