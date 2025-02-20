#include "Library/Play/Camera/CameraVerticalAbsorber.h"

#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraStartInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ByamlUtil.h"

namespace {
using namespace al;
NERVE_IMPL(CameraVerticalAbsorber, FollowGround);
NERVE_IMPL(CameraVerticalAbsorber, FollowAbsolute);
NERVE_IMPL_(CameraVerticalAbsorber, FollowClimbPoleNoInterp, Follow);
NERVE_IMPL_(CameraVerticalAbsorber, FollowSlow, Absorb);
NERVE_IMPL(CameraVerticalAbsorber, Absorb);
NERVE_IMPL(CameraVerticalAbsorber, Follow);
NERVE_IMPL(CameraVerticalAbsorber, FollowClimbPole);

NERVES_MAKE_STRUCT(CameraVerticalAbsorber, FollowGround, FollowAbsolute, FollowClimbPoleNoInterp,
                   FollowSlow, Absorb, Follow, FollowClimbPole);
}  // namespace

namespace al {

void CameraVerticalAbsorber::exeFollowAbsolute() {
    mTargetInterp *= 0.8f;
}

void CameraVerticalAbsorber::invalidate() {
    mIsInvalidated = true;
    if (!isNerve(this, &NrvCameraVerticalAbsorber.FollowAbsolute))
        setNerve(this, &NrvCameraVerticalAbsorber.FollowAbsolute);
}

// NON_MATCHING
void CameraVerticalAbsorber::start(const sead::Vector3f& pos, const CameraStartInfo& info) {
    alCameraPoserFunction::calcTargetFront(&mPrevTargetFront, mCameraPoser);

    mTargetInterp.x = 0.0f;
    mTargetInterp.y = 0.0f;
    mTargetInterp.z = 0.0f;

    mPrevTargetTrans = pos;

    if (unk_unusedBool || mIsInvalidated ||
        alCameraPoserFunction::isPlayerTypeNotTouchGround(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowAbsolute);
    if (alCameraPoserFunction::isTargetClimbPole(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowClimbPoleNoInterp);
    if (alCameraPoserFunction::isTargetGrabCeil(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowSlow);
    if (!info.mIsGrounded || alCameraPoserFunction::isTargetCollideGround(mCameraPoser))
        return setNerve(this, &NrvCameraVerticalAbsorber.FollowGround);

    mPrevTargetTrans = alCameraPoserFunction::getPreLookAtPos(mCameraPoser);

    sead::Vector3f target;

    alCameraPoserFunction::calcTargetGravity(&target, mCameraPoser);
    mTargetInterp = pos - mPrevTargetTrans;
    parallelizeVec(&mTargetInterp, target, mTargetInterp);
    setNerve(this, &NrvCameraVerticalAbsorber.Absorb);
}

void CameraVerticalAbsorber::load(const ByamlIter& data) {
    ByamlIter it;
    if (!data.tryGetIterByKey(&it, "VerticalAbsorb"))
        return;

    tryGetByamlF32(&mAbsorbScreenPosUp, it, "AbsorbScreenPosUp");
    tryGetByamlF32(&mAbsorbScreenPosDown, it, "AbsorbScreenPosDown");
    tryGetByamlF32(&mHighJumpJudgeSpeedV, it, "HighJumpJudgeSpeedV");
    ByamlIter it2;

    if (!it.tryGetIterByKey(&it2, "AdvanceAbsorbUp"))
        return;
    mIsAdvanceAbsorbUp = true;
    mAdvanceAbsorbScreenPosUp = getByamlKeyFloat(it2, "AdvanceAbsorbScreenPosUp");
}

// NON_MATCHING
void CameraVerticalAbsorber::update() {
    if (mIsStopUpdate)
        return;
    sead::Vector3f gravity{};
    alCameraPoserFunction::calcTargetGravity(&gravity, mCameraPoser);
    mTargetInterp = mCameraPoser->getPosition() - mPrevTargetTrans;
    parallelizeVec(&mTargetInterp, gravity, mTargetInterp);
    mLookAtCamera.getPos() = mCameraPoser->getPosition();
    mLookAtCamera.getAt() = mCameraPoser->getTargetTrans();
    mLookAtCamera.getUp() = mCameraPoser->getCameraUp();
    if (mLookAtCamera.getUp().length() > 0.0f)
        mLookAtCamera.getUp().normalize();
    if (!unk_unusedBool && !mIsInvalidated) {
        mLookAtCamera.getAt() -= mTargetInterp;
        if (!mIsNoCameraPosAbsorb)
            mLookAtCamera.getPos() -= mTargetInterp;
    }
    mLookAtCamera.doUpdateMatrix(&mLookAtCamera.getMatrix());
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
    sead::Vector3f prevTargetTrans = sead::Vector3f::zero;
    if (!mIsKeepInFrame) {
        prevTargetTrans = mTargetInterp;
    } else {
        sead::Vector3f offsetTrans = sead::Vector3f::zero;
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

}  // namespace al
