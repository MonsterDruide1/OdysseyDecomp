
#include "Player/PlayerStateNormalWallSlide.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/VectorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/stuff.h"
#include "Stuff.h"
#include "Util/PlayerCollisionUtil.h"
#include "math/seadQuat.h"
#include "math/seadVectorFwd.h"

namespace {
NERVE_IMPL(PlayerStateNormalWallSlide, Keep)
NERVE_IMPL(PlayerStateNormalWallSlide, Slide)

NERVES_MAKE_NOSTRUCT(PlayerStateNormalWallSlide, Keep, Slide)
}  // namespace

void sub_7100476580(al::LiveActor* x0_0, const PlayerInput* a2, const sead::Vector3f& a3, float* a4,
                    float a5) {
    const sead::Vector3f& gravity = al::getGravity(x0_0);
    sead::Vector3f a1 = {0.0, 0.0, 0.0};
    al::verticalizeVec(&a1, a3, gravity);
    if (!al::tryNormalizeOrZero(&a1))
        a1 = gravity;

    sead::Vector3f v28 = {0.0, 0.0, 0.0};
    sead::Vector3f v27 = {0.0, 0.0, 0.0};
    al::separateVelocityDirHV(&v27, &v28, x0_0, a3);

    sead::Vector3f v26 = {0.0, 0.0, 0.0};
    sead::Vector3f v25 = {0.0, 0.0, 0.0};
    al::separateVectorHV(&v26, &v25, a1, v27);

    sead::Vector3f v24;
    v24.setCross(a3, a1);
    al::tryNormalizeOrZero(&v24);

    sead::Vector3f v23 = {0.0, 0.0, 0.0};
    a2->calcMoveInput(&v23, -gravity);
    *a4 = al::converge(*a4, v23.dot(v24) * 3.0, 0.1);
    al::addVectorLimit(&v28, a3 * -15.0f, 15.0);
    al::addVectorLimit(&v25, a1 * a5, 15.0);
    al::setVelocity(x0_0, (v28 + v25) + (*a4 * v24));
}

PlayerStateNormalWallSlide::PlayerStateNormalWallSlide(al::LiveActor* player,
                                                       PlayerConst const* pConst,
                                                       PlayerInput const* input,
                                                       IUsePlayerCollision* collider,
                                                       PlayerAnimator* animator)
    : al::ActorStateBase("壁すべり", player), mConst(pConst), mInput(input), mCollision(collider),
      mAnimator(animator) {
    initNerve(&Keep, 0);
}

PlayerStateNormalWallSlide::~PlayerStateNormalWallSlide() = default;

void PlayerStateNormalWallSlide::appear() {
    mIsDead = false;
    mNormalStart = rs::getCollidedWallNormal(mCollision);
    mNormalEnd = mNormalStart;
    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatFrontUp(&quat, -mNormalStart, -al::getGravity(mActor));
    al::updatePoseQuat(mActor, quat);
    _58 = 0;
    al::setVelocityZero(mActor);
    al::tryAddVelocityLimit(mActor, mNormalStart * -15.0f, 15.0f);

    bool isCollidedFace = rs::isCollidedWallFace(mCollision);
    sead::Vector3f collidedWallPos = rs::getCollidedWallPos(mCollision);
    rs::resetCollisionExpandCheck(mCollision);
    if (!isCollidedFace) {
        sead::Vector3f a3 = {0.0f, 0.0f, 0.0f};
        a3 = al::getTrans(mActor) - collidedWallPos;
        al::verticalizeVec(&a3, mNormalStart, a3);

        sead::Vector3f v44 = {0.0f, 0.0f, 0.0f};
        v44.setCross(mNormalStart, al::getGravity(mActor));
        al::normalize(&v44);

        sead::Vector3f a2;
        a2.setCross(mNormalStart, v44);
        al::normalize(&a2);

        al::verticalizeVec(&a3, a2, a3);
        a3 = -a3;

        al::setTrans(mActor, al::getTrans(mActor) + a3);
    }

    sead::Vector3f offset =
        mNormalStart *
        sead::Mathf::clampMin((al::getTrans(mActor) - collidedWallPos).dot(mNormalStart), 0.0f);
    al::setTrans(mActor, al::getTrans(mActor) - offset);
    _5C = 0;
    _60 = 0;
    al::setNerve(this, &Keep);
}

void PlayerStateNormalWallSlide::exeKeep() {
    if (!followNormal())
        return;

    if (al::isFirstStep(this)) {
        if (mAnimator->mIsSubAnimOnlyAir)
            mAnimator->endSubAnim();

        // doesn't matter, both are 26 frames
        _60 = true;  // rs::isPlayerSideFaceToCameraZ(mActor);
        if (_60)
            mAnimator->startAnim("WallKeep");
        else
            mAnimator->startAnim("WallKeepReverse");
    }

    if (al::isGreaterStep(this, mConst->getWallKeepFrame()))
        sub_7100476580(mActor, mInput, mNormalEnd, &_58, mConst->getGravityWallSlide());
    else
        al::tryAddVelocityLimit(mActor, mNormalEnd * -15.0f, 15.0f);

    if (mAnimator->isAnimEnd())
        al::setNerve(this, &Slide);
}

bool PlayerStateNormalWallSlide::followNormal() {
    if (!rs::isCollidedWall(mCollision) || rs::isActionCodeNoWallKeepWall(mCollision)) {
        sead::Vector3f* velocityPtr = al::getVelocityPtr(mActor);
        al::verticalizeVec(velocityPtr, mNormalEnd, *velocityPtr);
        kill();
        return false;
    }

    sead::Vector3f collidedWallNormal = rs::getCollidedWallNormal(mCollision);
    sead::Vector3f v44;
    v44.setCross(collidedWallNormal, al::getGravity(mActor));
    al::normalize(&v44);

    f32 v15 = sead::Mathf::abs(
        al::calcAngleOnPlaneDegree(mNormalStart, collidedWallNormal, al::getGravity(mActor)));
    f32 v40 = 0.0f;
    al::tryCalcAngleOnPlaneDegree(&v40, mNormalStart, collidedWallNormal, v44);
    v40 = sead::Mathf::abs(v40);

    if (v15 <= mConst->getWallFollowAngleH() && v40 <= mConst->getWallFollowAngleV()) {
        _5C = 0;
        mNormalStart = collidedWallNormal;

        sead::Quatf quat1 = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat1, -mNormalStart, -al::getGravity(mActor));

        sead::Quatf quat2 = sead::Quatf::unit;
        al::calcQuat(&quat2, mActor);

        sead::Quatf rQuat = sead::Quatf::unit;
        al::slerpQuat(&rQuat, quat2, quat1, 0.25f);
        al::updatePoseQuat(mActor, rQuat);

        sead::Vector3f v37;
        v37.setCross(collidedWallNormal, v44);
        al::normalize(&v37);

        sead::Vector3f v36 = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&v36, mActor);

        sead::Vector3f a4 = {0.0f, 0.0f, 0.0f};
        al::turnVecToVecCosOnPlane(&a4, -mNormalStart, v36, v37, -1.0f);

        sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
        al::alongVectorNormalH(&a1, al::getVelocity(mActor), mNormalEnd, -a4);
        al::setVelocity(mActor, a1);
        mNormalEnd = a4;
    } else {
        _5C = al::converge(_5C, 0, 1);
        if (_5C >= 0) {
            sead::Vector3f* velocityPtr = al::getVelocityPtr(mActor);
            al::verticalizeVec(velocityPtr, mNormalEnd, *velocityPtr);
            kill();
            return false;
        }
    }

    if (rs::isOnGround(mActor, mCollision)) {
        sead::Vector3f* velocityPtr = al::getVelocityPtr(mActor);
        al::verticalizeVec(velocityPtr, mNormalEnd, *velocityPtr);
        kill();
        return false;
    }

    return true;
}

void PlayerStateNormalWallSlide::exeSlide() {
    if(!followNormal())
        return;

    if (al::isFirstStep(this)) {
        if (_60)
            mAnimator->startAnim("WallSlideL");
        else
            mAnimator->startAnim("WallSlideR");
    }

    sub_7100476580(mActor, mInput, mNormalEnd, &_58, mConst->getGravityWallSlide());
}
