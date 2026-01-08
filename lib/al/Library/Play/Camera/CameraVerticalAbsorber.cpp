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
    ByamlIter verticalAbsorbIter;
    if (!iter.tryGetIterByKey(&verticalAbsorbIter, "VerticalAbsorb"))
        return;

    tryGetByamlF32(&mAbsorbScreenPosUp, verticalAbsorbIter, "AbsorbScreenPosUp");
    tryGetByamlF32(&mAbsorbScreenPosDown, verticalAbsorbIter, "AbsorbScreenPosDown");
    tryGetByamlF32(&mHighJumpJudgeSpeedV, verticalAbsorbIter, "HighJumpJudgeSpeedV");

    ByamlIter advanceAbsorbUpIter;
    if (verticalAbsorbIter.tryGetIterByKey(&advanceAbsorbUpIter, "AdvanceAbsorbUp")) {
        mIsAdvanceAbsorbUp = true;
        mAdvanceAbsorbScreenPosUp =
            getByamlKeyFloat(advanceAbsorbUpIter, "AdvanceAbsorbScreenPosUp");
    }
}

void CameraVerticalAbsorber::start(const sead::Vector3f& pos, const CameraStartInfo& info) {
    alCameraPoserFunction::calcTargetFront(&mPrevTargetFront, mCameraPoser);

    mAbsorbVec = {0.0f, 0.0f, 0.0f};
    mPrevTargetTrans.set(pos);

    if (!isValid() || alCameraPoserFunction::isPlayerTypeNotTouchGround(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowAbsolute);

    if (alCameraPoserFunction::isTargetClimbPole(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowClimbPoleNoInterp);

    if (alCameraPoserFunction::isTargetGrabCeil(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowSlow);

    if (!info._25 || alCameraPoserFunction::isTargetCollideGround(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowGround);

    mPrevTargetTrans.set(alCameraPoserFunction::getPreLookAtPos(mCameraPoser));

    const CameraPoser* poser = mCameraPoser;
    sead::Vector3f gravity = {0.0f, 0.0f, 0.0f};
    alCameraPoserFunction::calcTargetGravity(&gravity, poser);

    mAbsorbVec = pos - mPrevTargetTrans;
    parallelizeVec(&mAbsorbVec, gravity, mAbsorbVec);
    setNerve(this, &NrvCameraVerticalAbsorber.Absorb);
}

bool CameraVerticalAbsorber::isValid() const {
    return !_1aa && !mIsInvalidated;
}

inline void updatePoser(sead::Vector3f* outPos, const CameraPoser* poser,
                        const sead::Vector3f& pos) {
    sead::Vector3f gravity = {0.0f, 0.0f, 0.0f};
    alCameraPoserFunction::calcTargetGravity(&gravity, poser);
    *outPos = poser->getTargetTrans() - pos;
    parallelizeVec(outPos, gravity, *outPos);
}

void CameraVerticalAbsorber::update() {
    if (mIsStopUpdate)
        return;

    updatePoser(&mAbsorbVec, mCameraPoser, mPrevTargetTrans);

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

    sead::Vector3f offset = {0.0f, 0.0f, 0.0f};
    if (mIsKeepInFrame) {
        sead::Vector3f offsetTrans = {0.0f, 0.0f, 0.0f};
        alCameraPoserFunction::calcTargetTransWithOffset(&offsetTrans, mCameraPoser);

        alCameraPoserFunction::calcOffsetCameraKeepInFrameV(
            &offset, &mLookAtCamera, offsetTrans, mCameraPoser, mKeepInFrameOffsetUp,
            alCameraPoserFunction::isPlayerTypeHighJump(mCameraPoser) ? 300.0f :
                                                                        mKeepInFrameOffsetDown);
        mAbsorbVec -= offset;
    }
    mPrevTargetTrans.set(mCameraPoser->getTargetTrans() - mAbsorbVec);
    mPrevTargetFront.set(mTargetFront);
}

void CameraVerticalAbsorber::makeLookAtCamera(sead::LookAtCamera* lookAtCamera) const {
    if (!isValid())
        return;

    lookAtCamera->setAt(lookAtCamera->getAt() - mAbsorbVec);
    if (!mIsNoCameraPosAbsorb)
        lookAtCamera->setPos(lookAtCamera->getPos() - mAbsorbVec);
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
                                                               pos + mAbsorbVec, -mAbsorbVec))
        return;

    mAbsorbVec = {0.0f, 0.0f, 0.0f};
    if (alCameraPoserFunction::isTargetCollideGround(mCameraPoser))
        setNerve(this, &NrvCameraVerticalAbsorber.FollowGround);
    else
        setNerve(this, &NrvCameraVerticalAbsorber.Follow);
}

void CameraVerticalAbsorber::exeFollowAbsolute() {
    mAbsorbVec *= 0.8f;
}

}  // namespace al
