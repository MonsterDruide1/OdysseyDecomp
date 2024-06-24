#include "Player/PlayerStateSpinCap.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerActionGroundMoveControl.h"
#include "Player/PlayerConst.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/StageSceneFunction.h"
#include "math/seadMatrix.h"

namespace {
NERVE_IMPL(PlayerStateSpinCap, Fall);
NERVE_IMPL(PlayerStateSpinCap, SpinGroundSeparate);
NERVE_IMPL(PlayerStateSpinCap, SpinGround);
NERVE_IMPL(PlayerStateSpinCap, SpinAirSeparate);
NERVE_IMPL(PlayerStateSpinCap, SpinAir);

NERVES_MAKE_NOSTRUCT(PlayerStateSpinCap, Fall, SpinGroundSeparate, SpinGround, SpinAirSeparate,
                   SpinAir);
}

void sub_71004831B8(sead::Vector3f* a1, const al::LiveActor* x1_0, const IUsePlayerCollision* a3, const PlayerInput* x3_0, bool a5) {
    sead::Vector3f a4 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrUpDir(&a4, x1_0, a3);
    x3_0->calcCapThrowInput(a1, a4);
    if ( a5 ) {
        sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&a2, x1_0);
        a2 = -a2;
        sead::Vector3f v11 = {0.0f, 0.0f, 0.0f};
        al::calcUpDir(&v11, x1_0);
        al::alongVectorNormalH(&a2, a2, v11, a4);
        if ( al::tryNormalizeOrZero(&a2) )
        {
        f32 v9 = a1->dot(a2);
        if ( v9 > 0.0 && v9 < 0.96593 )
            al::verticalizeVec(a1, a2, *a1);
        }
    }

    if ( !al::tryNormalizeOrZero(a1) ) {
        sead::Vector3f a2 = {0.0, 0.0, 0.0};
        al::calcUpDir(&a2, x1_0);
        al::calcFrontDir(a1, x1_0);
        al::alongVectorNormalH(a1, *a1, a2, a4);
        al::normalize(a1);
    }
}

PlayerStateSpinCap::PlayerStateSpinCap(al::LiveActor* player, PlayerConst const* pConst,
                                       PlayerInput const* input,
                                       PlayerCounterForceRun const* counterForceRun,
                                       al::WaterSurfaceFinder const* waterSurfaceFinder,
                                       IUsePlayerCollision const* collider, PlayerTrigger* trigger,
                                       PlayerSpinCapAttack* spinCapAttack, PlayerAnimator* animator,
                                       PlayerJointParamCapThrow* jointParamCapThrow)
    : al::ActorStateBase("帽子スピン", player), mConst(pConst), mInput(input),
      mCounterForceRun(counterForceRun), mWaterSurfaceFinder(waterSurfaceFinder),
      mCollider(collider), mTrigger(trigger), mSpinCapAttack(spinCapAttack), mAnimator(animator),
      mJointParamCapThrow(jointParamCapThrow) {
    mActionAirMoveControl = new PlayerActionAirMoveControl(player, pConst, input, collider, false);
    mActionAirMoveControl->setupCollideWallScaleVelocity(mConst->getFallWallScaleVelocity(), 0.0f,
                                                         mConst->getNormalMaxSpeed());
    mActionGroundMoveControl = new PlayerActionGroundMoveControl(player, pConst, input, collider);
    mActionGroundMoveControl->someFlags[0] = false;
    mActionGroundMoveControl->_BB = true;
    mActionGroundMoveControl->_78 = true;
    mActionGroundMoveControl->setup(mConst->getNormalMaxSpeed(), mConst->getNormalMinSpeed(),
                                    mConst->getNormalAccelFrame(), mConst->getStickOnBrakeFrame(),
                                    mConst->getNormalBrakeFrame(), mConst->getGravityMove(),
                                    mConst->getNormalMaxSpeed(), 0);
    mJudgeWaterSurfaceRun =
        new PlayerJudgeWaterSurfaceRun(player, pConst, waterSurfaceFinder, counterForceRun);
    initNerve(&Fall, 0);
}

PlayerStateSpinCap::~PlayerStateSpinCap() = default;

void PlayerStateSpinCap::appear() {
    bool v2 = mTrigger->isOn(PlayerTrigger::EActionTrigger_val33);
    mIsDead = false;
    mIsInWater = false;
    _99 = 0;
    _80 = 0;
    _9C = {0.0f, 0.0f, 0.0f};
    _A8 = 0;
    // TODO set something on JudgeWaterSurfaceRun
    _A9 = mTrigger->isOn(PlayerTrigger::EActionTrigger_val0);
    bool v10 = rs::isOnGround(mActor, mCollider) && !mTrigger->isOn(PlayerTrigger::EActionTrigger_val2);

    if(v2 || v10) {
        if(rs::isOnGroundSkateCode(mActor, mCollider))
            mSpinCapAttack->clearAttackInfo();

        if(mTrigger->isOn(PlayerTrigger::EActionTrigger_val1)) {
            al::alongVectorNormalH(al::getVelocityPtr(mActor), al::getVelocity(mActor), -al::getGravity(mActor), rs::getCollidedGroundNormal(mCollider));
        }

        mActionGroundMoveControl->appear();
        mSpinCapAttack->setupAttackInfo();

        if(mSpinCapAttack->isSeparateSingleSpin())
            al::setNerve(this, &SpinGroundSeparate);
        else
            al::setNerve(this, &SpinGround);
        return;
    } else {
        _78 = 1;
        mSpinCapAttack->setupAttackInfo();
        if(mSpinCapAttack->isSeparateSingleSpin())
            al::setNerve(this, &SpinAirSeparate);
        else
            al::setNerve(this, &SpinAir);
        return;
    }
}

void PlayerStateSpinCap::kill() {
    mIsDead = true;
    if(mSpinCapAttack->tryCancelCapState(mAnimator)) {
        // TODO something in JointParamCapThrow
    }
}

bool PlayerStateSpinCap::update() {
    // TODO something in JudgeWaterSurfaceRun
    rs::updateJudge(mJudgeWaterSurfaceRun);
    mActionGroundMoveControl->_9C = mCounterForceRun->isForceRun();
    mActionGroundMoveControl->_A0 = mCounterForceRun->getCounter();
    return al::NerveStateBase::update();
}

void PlayerStateSpinCap::control() {
    if(rs::isOnGround(mActor, mCollider))
        _78 = false;
}

bool PlayerStateSpinCap::noticeInWater() {
    if(mIsInWater) return false;
    mIsInWater = true;
    return true;
}

void PlayerStateSpinCap::resetJoint() {
    // TODO something in JointParamCapThrow
}

void PlayerStateSpinCap::cancelPoseInterpole() {
    if(_99 && al::isNerve(this, &SpinAir) && !al::isGreaterStep(this, 20)) {
        sead::Vector3f gravity = al::getGravity(mActor);
        sead::Vector3f up = {0.0f, 0.0f, 0.0f};
        al::calcUpDir(&up, mActor);
        if(up.dot(-gravity) <= sead::Mathf::cos(sead::Mathf::deg2rad(mConst->getCollisionResetLimit())))
            mTrigger->set(PlayerTrigger::EActionTrigger_val3);

        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatUpFront(&quat, -gravity, _7C);
        al::updatePoseQuat(mActor, quat);
    }
}

bool PlayerStateSpinCap::isEnableCancelHipDrop() const {
    if(isEnableCancelAir()) return true;
    if(al::isNerve(this, &SpinAirSeparate))
        return al::isGreaterStep(this, 10);
    if(al::isNerve(this, &SpinAir))
        return !mSpinCapAttack->isCapSpinAttack();
    return false;
}

bool PlayerStateSpinCap::isEnableCancelAir() const {
    if(al::isNerve(this, &Fall)) return true;
    if(!al::isNerve(this, &SpinAir) && !al::isNerve(this, &SpinAirSeparate)) return false;
    return !isSpinAttackAir();
}

bool PlayerStateSpinCap::isSpinAttackAir() const {
    return !mIsDead && (al::isNerve(this, &SpinAir) || al::isNerve(this, &SpinAirSeparate)) && al::isLessEqualStep(this, 22);
}

bool PlayerStateSpinCap::isEnableCancelGround() const {
    if(al::isNerve(this, &SpinGroundSeparate))
        return al::isGreaterStep(this, 10);
    if(al::isNerve(this, &SpinGround))
        return !mSpinCapAttack->isCapSpinAttack();
    return false;
}

bool PlayerStateSpinCap::isEnableCancelCarry() const {
    if(mIsDead) return false;
    return isEnableCancelGround() || isEnableCancelAir();
}

bool PlayerStateSpinCap::isEnableReThrowCap() const {
    if(!isEnableCancelGround()) return false;
    if(al::isLessStep(this, 20)) return false;
    if(al::isNerve(this, &SpinGroundSeparate))
        return rs::isCollidedGroundRunAngle(mActor, mCollider, mConst);
    return true; 
}

bool PlayerStateSpinCap::isWaterSurfaceRun() const {
    return al::isNerve(this, &SpinGround) && rs::isJudge(mJudgeWaterSurfaceRun);
}

void PlayerStateSpinCap::exeSpinGround() {
    if(al::isFirstStep(this)) {
        mSpinCapAttack->startCapSpinAttack(mAnimator, mInput);
        // TODO something in JointParamCapThrow
    }

    updateSpinGroundNerve();

    if(al::isStep(this, mSpinCapAttack->getThrowFrameGround()) && mSpinCapAttack->isCapSpinAttack()) {
        rs::addPlayerThrowCapCount(mActor);
        sead::Vector3f v9 = {0.0f, 0.0f, 0.0f};
        rs::calcGroundNormalOrGravityDir(&v9, mActor, mCollider);
        // TODO requires flag from JudgeWaterSurfaceRun
        mSpinCapAttack->startCapThrow(_7C, v9, 1.0f, false, sead::Vector3f::zero);
    }

    if(mAnimator->isAnimEnd() && mSpinCapAttack->isCapSpinAttack())
        kill();
}

void PlayerStateSpinCap::updateSpinGroundNerve() {
    if(al::isFirstStep(this)) {
        sub_71004831B8(&_7C, mActor, mCollider, mInput, _A9);

        sead::Vector3f v6364 = {0.0f, 0.0f, 0.0f};
        rs::calcGroundNormalOrGravityDir(&v6364, mActor, mCollider);
        rs::slerpUpFront(mActor, v6364, _7C, 1.0f, mConst->getHillPoseDegreeMax());

        if(rs::isOnGroundSkateCode(mActor, mCollider)) {
            CRASH
        }
    }

    if(!rs::isOnGroundSkateCode(mActor, mCollider)) {
        if(!(mCounterForceRun->isForceRun() || rs::isJudge(mJudgeWaterSurfaceRun)) || al::isLessStep(this, mConst->getSpinBrakeFrame())) {
            sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
            rs::calcGroundNormalOrUpDir(&a2, mActor, mCollider);
            sead::Vector3f v67 = {0.0f, 0.0f, 0.0f};
            mInput->calcMoveInput(&v67, a2);
            sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
            al::tryNormalizeOrZero(&a1, v67);

            mActionGroundMoveControl->updateNormalAndSnap(al::getVelocityPtr(mActor));
            sead::Vector3f v65 = {0.0f, 0.0f, 0.0f};
            sead::Quatf v6364 = sead::Quatf::unit;
            rs::moveInertiaTurn(&v65, &v6364, mActor, mCollider, a1, mConst->getSpinBrakeRate(), mConst->getSpinRoundLimitDegree(), mConst->getGravityMove(), mConst->getGravityAir(), mConst->getFallSpeedMax());

            sead::Vector3f v62 = {0.0f, 0.0f, 0.0f};
            al::calcFrontDir(&v62, mActor);

            f32 v33 = (float)((float)(v6364.y * v62.z) - (float)(v6364.z * v62.y)) + (float)(v6364.w * v62.x);
            f32 v34 = (float)(v62.y * v6364.w) + (float)((float)(v6364.z * v62.x) - (float)(v62.z * v6364.x));
            f32 v35 = (float)(v62.z * v6364.w) + (float)((float)(v62.y * v6364.x) - (float)(v6364.y * v62.x));
            f32 v36 = (float)((float)-(float)(v62.x * v6364.x) - (float)(v6364.y * v62.y)) - (float)(v62.z * v6364.z);
            v62.x = (float)((float)(v6364.y * v35) + (float)((float)(v6364.w * v33) - (float)(v6364.z * v34)))
                    - (float)(v6364.x * v36);
            v62.y = (float)((float)((float)(v33 * v6364.z) + (float)(v34 * v6364.w)) - (float)(v35 * v6364.x))
                    - (float)(v36 * v6364.y);
            v62.z = (float)((float)((float)(v34 * v6364.x) - (float)(v33 * v6364.y)) + (float)(v35 * v6364.w))
                    - (float)(v36 * v6364.z);
            
            rs::slerpUpFront(mActor, a2, v62, mConst->getSlerpQuatRate(), mConst->getHillPoseDegreeMax());
        }
        else {
            if(_80 || al::isStep(this, mConst->getSpinBrakeFrame())) {
                _80 = false;
                goto LABEL_30;
            }
            mActionGroundMoveControl->update();
        }
    }
    else if(!_80 || al::isFirstStep(this)) {
        _80 = true;
    LABEL_30:
        mActionGroundMoveControl->setup(mConst->getNormalMaxSpeed(), mConst->getNormalMinSpeed(), mConst->getNormalAccelFrame(), mConst->getStickOnBrakeFrame(), mConst->getNormalBrakeFrame(), mConst->getGravityMove(), mConst->getNormalMaxSpeed(), 0);
        mActionGroundMoveControl->updateNormalAndSnap(al::getVelocityPtr(mActor));
        mActionGroundMoveControl->reset(mActionGroundMoveControl->mGravityDir);
        mActionGroundMoveControl->update();
    }

    if(rs::isJudge(mJudgeWaterSurfaceRun)) {
        CRASH
    }
    
    if(mAnimator->getAnimFrameMax() - mAnimator->getAnimFrame() <= 3.0f)
        _A8 = true;
}

void PlayerStateSpinCap::exeSpinGroundSeparate() {
    if(al::isFirstStep(this))
        mSpinCapAttack->startSpinSeparate(mAnimator);

    updateSpinGroundNerve();
    if(mAnimator->isAnimEnd())
        kill();
}

void PlayerStateSpinCap::exeSpinAir() {
    if(al::isFirstStep(this)) {
        mSpinCapAttack->startCapSpinAttackAir(mAnimator, mInput);
        // TODO something in JointParamCapThrow
    }

    updateSpinAirNerve();

    if(al::isStep(this, mSpinCapAttack->getThrowFrameAir()) && mSpinCapAttack->isCapSpinAttack()) {
        rs::addPlayerThrowCapCount(mActor);
        sead::Vector3f v18 = _9C * -sead::Mathf::clampMin(9-al::getNerveStep(this), 0);
        mSpinCapAttack->startCapThrow(_7C, -al::getGravity(mActor), 1.0f, false, v18);
    }

    if(!al::isLessEqualStep(this, 22) && mSpinCapAttack->isCapSpinAttack())
        al::setNerve(this, &Fall);
}

void PlayerStateSpinCap::updateSpinAirNerve() {
    if(al::isFirstStep(this)) {
        sub_71004831B8(&_7C, mActor, mCollider, mInput, _A9);

        _99 = false;
        if(rs::isCollidedGround(mCollider)) {
            mTrigger->set(PlayerTrigger::EActionTrigger_val3);
            rs::slerpUpFront(mActor, rs::getCollidedGroundNormal(mCollider), _7C, 1.0f, mConst->getHillPoseDegreeMax());
        } else {
            sead::Matrix34f dest = sead::Matrix34f::ident;
            
            dest = *mActor->getBaseMtx();
            //mAnimator->calcModelJointRootMtx(&dest);
            
            sead::Vector3f v1416;
            dest.getBase(v1416, 1);
            sead::Vector3f negGravity = -al::getGravity(mActor);
            f32 v20 = v1416.dot(negGravity);
            if(v20 < 0.5f) {
                sead::Vector3f v6062;
                dest.getBase(v6062, 3);
                sead::Vector3f v5859 = {0.0f, 0.0f, 0.0f};
                sead::Matrix34f v57 = sead::Matrix34f::ident;
                al::makeMtxRT(&v57, mActor);
                al::calcMtxLocalTrans(&v5859, v57, v6062);

                sead::Quatf v56 = sead::Quatf::unit;
                dest.toQuat(v56);
                al::updatePoseQuat(mActor, v56);

                sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
                al::verticalizeVec(&a1, negGravity, _7C);
                al::normalize(&a1);

                sead::Vector3f up = {0.0f, 0.0f, 0.0f};
                sead::Vector3f front = {0.0f, 0.0f, 0.0f};
                al::calcUpDir(&up, mActor);
                al::calcFrontDir(&front, mActor);

                f32 v21 = up.dot(a1);
                f32 v23 = sead::Mathf::abs(v21);
                f32 v22 = sead::Mathf::abs(front.dot(a1));

                if(v23 <= v22) {
                    front = a1;
                } else {
                    sead::Vector3f v51 = {0.0f, 0.0f, 0.0f};
                    sead::Vector3f v52 = {0.0f, 0.0f, 0.0f};
                    al::separateVectorHV(&v51, &v52, negGravity, up);
                    f32 v26 = v51.length();
                    f32 v27 = front.dot(negGravity);

                    if(v21 <= 0.0f || v27 <= 0.0f) {
                        if(v27 < 0.0f && v21 < 0.0f)
                            v26 = -v26;
                    } else {
                        v26 = -v26;
                    }

                    front = (v26 * a1) + v52;
                }

                if(al::isParallelDirection(up, front, 0.01f)) {
                    up = negGravity;
                    front = a1;
                }

                rs::slerpUpFront(mActor, front, up, 1.0f, 180.0f);

                sead::Matrix34f v51 = sead::Matrix34f::ident;
                al::makeMtxR(&v51, mActor);

                sead::Vector3f v52 = {0.0f, 0.0f, 0.0f};
                al::calcTransLocalOffsetByMtx(&v52, v51, v5859);

                sead::Vector3f v3234 = v6062 - v52;
                al::setTrans(mActor, v3234+al::getTrans(mActor));
                _9C = v3234 * 0.1f;
                mTrigger->set(PlayerTrigger::EActionTrigger_val3);
                _99 = true;
            }
            else {
                if(v20 <= sead::Mathf::cos(sead::Mathf::deg2rad(mConst->getCollisionResetLimit())))
                    mTrigger->set(PlayerTrigger::EActionTrigger_val3);

                rs::slerpUpFront(mActor, negGravity, _7C, 1.0f, mConst->getHillPoseDegreeMax());
            }
        }

        mActionAirMoveControl->setup(mConst->getSpinAirSpeedMax(), mConst->getSpinAirSpeedMax(), 0, mConst->getSpinAirJumpPower(), mConst->getGravitySpinAir(), 0, 0.0f);
        if(_99) {
            mActionAirMoveControl->something2 = mConst->getSlerpQuatRateSpinAir();
            mActionAirMoveControl->someVec = _7C;
        }
    }

    if(_99 && al::isLessStep(this, 10)) {
        al::setTrans(mActor, al::getTrans(mActor) - _9C);
    }

    if(mTrigger->isOnUpperPunchHit() || rs::isCollidedCeiling(mCollider))
        rs::reflectCeiling(mActor, 0.0f);

    mActionAirMoveControl->update();
}

void PlayerStateSpinCap::exeSpinAirSeparate() {
    if(al::isFirstStep(this))
        mSpinCapAttack->startSpinSeparate(mAnimator);

    updateSpinAirNerve();
    if(al::isLessEqualStep(this, 22))
        return al::setNerve(this, &Fall);
}

void PlayerStateSpinCap::exeFall() {
    if(al::isFirstStep(this)) {
        mActionAirMoveControl->setup(mConst->getJumpMoveSpeedMax(), mConst->getJumpMoveSpeedMin(), 0, al::calcSpeedV(mActor), mConst->getJumpGravity(), 0, 0.0f);
        if(_99) {
            mActionAirMoveControl->something2 = mConst->getSlerpQuatRateSpinAir();
        }
    }

    if(!mAnimator->isAnim("Fall") && mAnimator->isAnimEnd())
        mAnimator->startAnim("Fall");

    mActionAirMoveControl->update();
}
