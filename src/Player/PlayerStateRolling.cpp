#include "Player/PlayerStateRolling.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionSlopeSlideControl.h"
#include "Player/PlayerJudgePreInputJump.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "Util/StageSceneFunction.h"

namespace {
NERVE_IMPL(PlayerStateRolling, Land);
NERVE_IMPL(PlayerStateRolling, Start);
NERVE_IMPL(PlayerStateRolling, Rolling);
NERVE_IMPL(PlayerStateRolling, BoostStart);
NERVE_IMPL(PlayerStateRolling, Jump);
NERVE_IMPL(PlayerStateRolling, StandUp);
NERVE_IMPL(PlayerStateRolling, EndSquat);
NERVE_IMPL(PlayerStateRolling, UnRoll);
NERVE_IMPL(PlayerStateRolling, Brake);

NERVE_MAKE(PlayerStateRolling, Land);
NERVE_MAKE(PlayerStateRolling, Start);
NERVE_MAKE(PlayerStateRolling, Rolling);
NERVE_MAKE(PlayerStateRolling, BoostStart);
NERVE_MAKE(PlayerStateRolling, Jump);
NERVE_MAKE(PlayerStateRolling, StandUp);
NERVE_MAKE(PlayerStateRolling, EndSquat);
NERVE_MAKE(PlayerStateRolling, UnRoll);
NERVE_MAKE(PlayerStateRolling, Brake);
}  // namespace

void sub_710047B228(al::LiveActor* a1, const IUsePlayerCollision* a2, const PlayerConst* a3,
                    const sead::Vector3f& x3_0, float a5) {
    const sead::Vector3f* v14;
    const sead::Vector3f* v15;
    float v16;
    float v17;
    float v18;
    sead::Vector3f v19;

    sead::Vector3f a4 = {0.0, 0.0, 0.0};
    rs::calcGroundNormalOrGravityDir(&a4, a1, a2);
    al::alongVectorNormalH(al::getVelocityPtr(a1), al::getVelocity(a1), x3_0, a4);
    if (!al::isFloorPolygonCos(a4, al::getGravity(a1),
                               sead::Mathf::cos(sead::Mathf::deg2rad(a3->getStandAngleMin())))) {
        v19.x = 0.0;
        v19.y = 0.0;
        v19.z = 0.0;
        v14 = &al::getGravity(a1);
        rs::calcSlideDir(&v19, *v14, a4);
        v15 = &al::getVelocity(a1);
        v16 = v19.dot(*v15);
        v17 = sead::Mathf::clampMin(a5 - v16, 0.0);
        if (v16 >= 0.0)
            v18 = v17;
        else
            v18 = a5 - v16;

        al::addVelocityToDirection(a1, v19, v18);
    }
}

void sub_710047B358(const al::LiveActor* a1, PlayerActionSlopeSlideControl* a2,
                    const PlayerConst* a3, const IUsePlayerCollision* a4, PlayerSeCtrl* a5) {
    if (rs::isOnGroundSkateCode(a1, a4)) {
        a5->_174 =
            a2->update(a3->getSlopeRollingAccelOnSkate(), a3->getSlopeRollingBrakeOnSkate(), 0.0f,
                       0.0f, a3->getSlopeRollingMaxSpeed(), a3->getSlopeRollingSideAccelOnSkate(),
                       a3->getSlopeRollingBrakeOnSkate(), a3->getSlopeRollingSideMaxSpeed(),
                       a3->getSlopeTurnDegree(), a3->getGravityAir(), true);
    } else {
        a5->_174 = a2->update(a3->getSlopeRollingAccel(), a3->getSlopeRollingBrake(),
                              a3->getSlopeRollingAgainst(), a3->getSlopeRollingAnglePowerMax(),
                              a3->getSlopeRollingMaxSpeed(), a3->getSlopeRollingSideAccel(),
                              a3->getSlopeRollingSideBrake(), a3->getSlopeRollingSideMaxSpeed(),
                              a3->getSlopeTurnDegree(), a3->getGravityAir(), false);
    }
}

PlayerStateRolling::PlayerStateRolling(
    al::LiveActor* player, PlayerConst const* pConst, PlayerInput const* input,
    IUsePlayerCollision const* collider, PlayerTrigger* trigger, PlayerAnimator* animator,
    PlayerEffect* effect, PlayerJudgeStartRolling* judgeStartRolling, IJudge* judgeStandUp,
    PlayerJudgePreInputJump* judgePreInputJump, PlayerJudgePreInputCapThrow* judgePreInputCapThrow,
    IJudge* judgeForceRolling, PlayerContinuousLongJump* continuousLongJump, PlayerSeCtrl* seCtrl)
    : al::ActorStateBase("ローリング", player), mConst(pConst), mInput(input), mCollider(collider),
      mTrigger(trigger), mAnimator(animator), mEffect(effect), mActionAirMoveControl(nullptr),
      mActionSlopeSlideControl(nullptr), mContinuousLongJump(continuousLongJump),
      mJudgeStartRolling(judgeStartRolling), mJudgeEnableStandUp(judgeStandUp),
      mJudgePreInputJump(judgePreInputJump), mJudgePreInputCapThrow(judgePreInputCapThrow),
      mJudgeForceRolling(judgeForceRolling), mSeCtrl(seCtrl) {
    mActionAirMoveControl = new PlayerActionAirMoveControl(player, pConst, input, collider, 0);
    mActionSlopeSlideControl = new PlayerActionSlopeSlideControl(player, pConst, input, collider);
    initNerve(&Start, 0);
}

PlayerStateRolling::~PlayerStateRolling() = default;

void PlayerStateRolling::appear() {
    mIsDead = false;
    rs::startHitReactionLandJumpIfLanding(mActor, mCollider, false);
    _98 = 0;
    _90 = 0;
    _94 = 0;
    _A0 = 0;
    _B0 = rs::updateJudgeAndResult(mJudgeForceRolling);

    if (mTrigger->isOn(PlayerTrigger::EActionTrigger_val8)) {
        _98 = mConst->getSlopeRollingFrameMin();
        _9C = mConst->getSlopeRollingReStartCharge();
        _90 = 1;
        sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
        rs::calcGroundNormalExceptJustLandOrGravityDir(&a2, mActor, mCollider);
        sead::Vector3f front = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&front, mActor);
        al::verticalizeVec(&front, a2, front);
        al::tryNormalizeOrZero(&front);
        al::addVelocityToDirection(mActor, front,
                                   sead::Mathf::clampMin(mConst->getSlopeRollingSpeedStart() -
                                                             al::getVelocity(mActor).dot(front),
                                                         0.0f));

        if (rs::isCollidedGroundOverAngle(mActor, mCollider, mConst->getStandAngleMin()))
            mActionSlopeSlideControl->setupCutSlideOppositeDir();
        else
            mActionSlopeSlideControl->setup();
        al::setNerve(this, &Rolling);
        return;
    }

    mActionSlopeSlideControl->setup();
    if (mTrigger->isOn(PlayerTrigger::EActionTrigger_val17)) {
        _98 = mConst->getSlopeRollingFrameMin();
        _9C = mConst->getSlopeRollingReStartCharge();
        _90 = 1;
        sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
        rs::calcGroundNormalExceptJustLandOrGravityDir(&a2, mActor, mCollider);
        sead::Vector3f front = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&front, mActor);
        al::verticalizeVec(&front, a2, front);
        al::tryNormalizeOrZero(&front);
        al::addVelocityToDirection(mActor, front,
                                   sead::Mathf::clampMin(mConst->getSlopeRollingSpeedBoost() -
                                                             al::getVelocity(mActor).dot(front),
                                                         0.0f));
        rs::resetJudge(mJudgePreInputCapThrow);
        al::setNerve(this, &BoostStart);
        return;
    }

    _9C = 0;
    sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalExceptJustLandOrGravityDir(&a2, mActor, mCollider);
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, mActor);
    al::verticalizeVec(&front, a2, front);
    al::tryNormalizeOrZero(&front);
    al::addVelocityToDirection(mActor, front,
                               sead::Mathf::clampMin(mConst->getSlopeRollingSpeedStart() -
                                                         al::getVelocity(mActor).dot(front),
                                                     0.0f));
    rs::resetJudge(mJudgePreInputCapThrow);
    al::setNerve(this, &Start);
}

void PlayerStateRolling::kill() {
    mEffect->tryDeleteRollingEffect();
    mIsDead = true;
}

bool PlayerStateRolling::update() {
    if (rs::isCollidedGround(mCollider))
        _B0 = rs::updateJudgeAndResult(mJudgeForceRolling);
    return al::NerveStateBase::update();
}

void PlayerStateRolling::control() {
    if (_98 <= sead::Mathf::max(mConst->getSlopeRollingFrameMin(),
                                mConst->getSlopeRollingReStartInterval()))
        _98++;

    if (al::isNerve(this, &Jump)) {
        _9C = 0;
        _A0 = 0;
        return;
    }

    if (_9C <= mConst->getSlopeRollingReStartCharge())
        _9C++;
    if (rs::updateJudgeAndResult(mJudgeStartRolling))
        _9C = 0;
    _A0 = al::converge(_A0, mConst->getSlopeRollingReStartForce(), 1);

    if (al::isNerve(this, &BoostStart) || al::isNerve(this, &Start) ||
        al::isNerve(this, &Rolling)) {
        if (rs::isCollidedGround(mCollider)) {
            if (_94 >= 20)
                al::startHitReaction(mActor, "転がり着地");
            _94 = 0;
        } else {
            _94 = al::converge(_94, 20, 1);
        }
    }
}

bool PlayerStateRolling::isRolling() const {
    return !mIsDead && !al::isNerve(this, &Jump);
}

bool PlayerStateRolling::isRollingJump() const {
    return !mIsDead && al::isNerve(this, &Jump);
}

bool PlayerStateRolling::isEnableCancelNormalJump() const {
    return !al::isNerve(this, &StandUp);
}

bool PlayerStateRolling::isEndSquat() const {
    return al::isNerve(this, &EndSquat);
}

bool PlayerStateRolling::isEndStandUp() const {
    return mIsDead && al::isNerve(this, &StandUp);
}

bool PlayerStateRolling::isEnableTrample(al::HitSensor const* a2, al::HitSensor const* a3) const {
    if (mIsDead || al::isNerve(this, &Jump))
        return true;

    sead::Vector3f gravity = al::getGravity(mActor);
    sead::Vector3f dist = {0.0f, 0.0f, 0.0f};
    al::calcVecBetweenSensors(&dist, a2, a3);
    return (-gravity).dot(dist) >= (al::getSensorRadius(a3) + al::getSensorRadius(a2)) -
                                       (mConst->getFallSpeedMax() + 10.0f);
}

f32 PlayerStateRolling::getInverseKinematicsRate() const {
    return al::isNerve(this, &StandUp) ? 1.0f : 0.0f;
}

void PlayerStateRolling::exeBoostStart() {
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("Rolling");
        sub_710047B228(mActor, mCollider, mConst, mActionSlopeSlideControl->_2c,
                       mConst->getSlopeRollingSpeedBoost());
        al::startHitReaction(mActor, "転がり再加速[2回目以降]");
    }

    sub_710047B358(mActor, mActionSlopeSlideControl, mConst, mCollider, mSeCtrl);
    updateRollingAnimFrameRate();

    if (rs::isCollidedGround(mCollider))
        mEffect->tryEmitRollingEffect();
    else
        mEffect->tryDeleteRollingEffect();

    if (!_B0 && rs::isCollidedGroundLessAngle(mActor, mCollider, mConst->getStandAngleMin()) &&
        rs::updateJudgeAndResult(mJudgeEnableStandUp) &&
        rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
        al::setNerve(this, &Jump);
        return;
    }

    if (al::isGreaterEqualStep(this, mConst->getSlopeRollingFrameMinBoost())) {
        al::setNerve(this, &Rolling);
        return;
    }
}

void PlayerStateRolling::updateRollingAnimFrameRate() {
    WARN_UNIMPL; // required if any animations are used as timers. ExeStart does it, but that's fine
}

void PlayerStateRolling::exeStart() {
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("RollingStart");
        sub_710047B228(mActor, mCollider, mConst, mActionSlopeSlideControl->_2c,
                       mConst->getSlopeRollingStartSlideSpeed());
        sead::Vector3f v17 = {0.0f, 0.0f, 0.0f};
        rs::calcGroundNormalOrGravityDir(&v17, mActor, mCollider);
        al::addVelocityToDirection(mActor, v17, mConst->getSlopeRollingStartJumpPower());
        mEffect->tryDeleteRollingEffect();
    }

    sub_710047B358(mActor, mActionSlopeSlideControl, mConst, mCollider, mSeCtrl);

    if (mAnimator->isAnimEnd()) {
        al::setNerve(this, &Rolling);
        return;
    }

    if (isStartRollingBrake()) {
        al::setNerve(this, &UnRoll);
        return;
    }
}

bool PlayerStateRolling::isStartRollingBrake() const {
    return rs::isOnGroundSlopeSlideEnd(mActor, mCollider, mConst) &&
           mActionSlopeSlideControl->_38.length() <= mConst->getSlopeRollingSpeedEnd() &&
           _98 >= mConst->getSlopeRollingFrameMin() && !_B0;
}

void PlayerStateRolling::exeRolling() {
    if (al::isFirstStep(this)) {
        if (!mAnimator->isAnim("Rolling"))
            mAnimator->startAnim("Rolling");
    }

    sub_710047B358(mActor, mActionSlopeSlideControl, mConst, mCollider, mSeCtrl);
    updateRollingAnimFrameRate();

    if (rs::isCollidedGround(mCollider))
        mEffect->tryEmitRollingEffect();
    else
        mEffect->tryDeleteRollingEffect();

    if (!_B0) {
        if (!mInput->isHoldSquat() && mConst->getSlopeRollingFrameMin() <= _98 &&
            rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
            al::setNerve(this, &StandUp);
            return;
        }
        if (!_B0) {
            if (rs::isCollidedGroundLessAngle(mActor, mCollider, mConst->getStandAngleMin()) &&
                rs::updateJudgeAndResult(mJudgeEnableStandUp) &&
                rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
                al::setNerve(this, &Jump);
                return;
            }
        }
    }

    if (rs::updateJudgeAndResult(mJudgeStartRolling)) {
        if (_98 >= mConst->getSlopeRollingReStartInterval()) {
            bool v24;
            if (mJudgeStartRolling->isTriggerRestartSwing())
                v24 = mConst->getSlopeRollingReStartSwing() <= _9C;
            else if (mConst->getSlopeRollingReStartForce() <= _A0)
                v24 = true;
            else
                v24 = mConst->getSlopeRollingReStartCharge() <= _9C;

            restartRolling(v24, mJudgeStartRolling->isTriggerRestartSwing());
            al::setNerve(this, &Start);
            return;
        }
    }

    if (isStartRollingBrake()) {
        al::setNerve(this, &UnRoll);
        return;
    }
}

void PlayerStateRolling::restartRolling(bool w1_0, bool w2_0) {
    f32 v9 = 0.0f;
    if (w1_0) {
        _90 = al::converge(_90, w2_0 ? 3 : 2, 1);
        if (_90 == 1)
            al::startHitReaction(mActor, "転がり再加速[1回目]");
        else if (_90 >= 2)
            al::startHitReaction(mActor, "転がり再加速[2回目以降]");

        v9 = (float)_90;
        _A0 = 0;
    }

    _98 = 0;
    sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f v20 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrGravityDir(&v20, mActor, mCollider);
    al::verticalizeVec(&a1, v20, al::getVelocity(mActor));
    f32 v11 = a1.length();
    f32 v12_1 = mConst->getSlopeRollingSpeedStart();
    f32 v14 = sead::Mathf::max(v11, v12_1 + (v9 * mConst->getSlopeRollingReStartMaxAdd()));
    f32 v12_2 = sead::Mathf::clamp(v11 + (v9 * mConst->getSlopeRollingReStartAccel()), v12_1, v14);

    sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalExceptJustLandOrGravityDir(&a2, mActor, mCollider);
    sead::Vector3f a3 = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&a3, mActor);
    al::verticalizeVec(&a3, a2, a3);
    al::tryNormalizeOrZero(&a3);
    al::addVelocityToDirection(mActor, a3,
                               sead::Mathf::clampMin(v12_2 - al::getVelocity(mActor).dot(a3), 0.0));
    return rs::resetJudge(mJudgePreInputCapThrow);
}

void PlayerStateRolling::exeJump() {
    const sead::Vector3f& gravity = al::getGravity(mActor);
    if(al::isFirstStep(this)) {
        rs::noticePlayerJumpStart(mTrigger, mActor);
        mEffect->tryDeleteRollingEffect();
        _9C = 0;
        if(rs::isModeE3MovieRom()) {
            mAnimator->startAnim("JumpBroad3");
        } else {
            mAnimator->startAnim(mContinuousLongJump->getLongJumpAnimName());
        }
        mContinuousLongJump->countUp();
        sub_710047B228(mActor, mCollider, mConst, mActionSlopeSlideControl->_2c, 0.0f);
        rs::setupLongJumpVelocity(mActor, mCollider, mConst->getJumpInertiaRate(), mConst->getLongJumpMovePow(), mConst->getLongJumpSpeedMin(), mConst->getLongJumpInitSpeed(), mConst->getLongJumpJumpPow());
    }

    if(mTrigger->isOnUpperPunchHit())
        rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, false);

    if(rs::isCollidedCeiling(mCollider))
        rs::reflectCeiling(mActor, 0.0f);

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -gravity);
    rs::moveDivingJump(mActor, moveInput, mConst->getLongJumpAccel(), mConst->getLongJumpBrake(), mConst->getLongJumpSpeed(), mConst->getLongJumpSpeedMin(), mConst->getLongJumpSideAccel(), mConst->getLongJumpGravity(), mConst->getFallSpeedMax(), mConst->getSlerpQuatRate());

    if(rs::isOnGround(mActor, mCollider)) {
        rs::startHitReactionLandIfLanding(mActor, mCollider, false);
        if(mInput->isHoldSquat() && rs::isOnGroundRunAngle(mActor, mCollider, mConst)) {
            _9C = mConst->getSlopeRollingReStartCharge();
            _98 = mConst->getSlopeRollingReStartInterval();
            mActionSlopeSlideControl->setup();
            al::setNerve(this, &Land);
            return;
        }

        kill();
        return;
    }
}

void PlayerStateRolling::exeLand() {
    if(al::isFirstStep(this)) {
        mAnimator->startAnim("RollingLand");
        mEffect->tryDeleteRollingEffect();
    }

    sub_710047B358(mActor, mActionSlopeSlideControl, mConst, mCollider, mSeCtrl);

    if(!_B0) {
        if(!mInput->isHoldSquat() && mConst->getSlopeRollingFrameMin() <= _98 && rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
            al::setNerve(this, &StandUp);
            return;
        }
    }

    if(rs::isCollidedGroundLessAngle(mActor, mCollider, mConst->getStandAngleMin()) && rs::updateJudgeAndResult(mJudgeEnableStandUp) && rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
        al::setNerve(this, &Jump);
        return;
    }

    if(rs::updateJudgeAndResult(mJudgeStartRolling)) {
        if(_98 >= mConst->getSlopeRollingReStartInterval()) {
            bool v22;
            if(mJudgeStartRolling->isTriggerRestartSwing()) {
                v22 = mConst->getSlopeRollingReStartSwing() <= _9C;
            } else if(mConst->getSlopeRollingReStartForce() <= _A0) {
                v22 = true;
            } else {
                v22 = mConst->getSlopeRollingReStartCharge() <= _9C;
            }
            restartRolling(v22, mJudgeStartRolling->isTriggerRestartSwing());
            al::setNerve(this, &Start);
            return;
        }
    }

    if(mAnimator->isAnimEnd()) {
        al::setNerve(this, &Rolling);
        return;
    }

    if(isStartRollingBrake()) {
        al::setNerve(this, &UnRoll);
        return;
    }
}

void PlayerStateRolling::exeUnRoll() {
    if(al::isFirstStep(this)) {
        if(mAnimator->isAnim("Rolling")) {
            f32 currentFrame = mAnimator->getAnimFrame();
            mAnimator->startAnim("RollingEnd");
            mAnimator->setAnimFrame(currentFrame);
        } else {
            mAnimator->startAnim("Rolling");
        }
        mEffect->tryDeleteRollingEffect();
    }

    sead::Vector3f v38 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrUpDir(&v38, mActor, mCollider);

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, v38);

    sead::Vector3f v36 = {0.0f, 0.0f, 0.0f};
    rs::moveInertiaSlide(&v36, mActor, mCollider, moveInput, mConst->getSlopeRollingBrake(), 0.0f, 0.0f, 0.0f, mConst->getGravity(), mConst->getGravityAir(), mConst->getFallSpeedMax());
    rs::slerpUp(mActor, v38, mConst->getSlerpQuatRate(), mConst->getHillPoseDegreeMax());
    updateRollingAnimFrameRate();

    if(rs::updateJudgeAndResult(mJudgeStartRolling)) {
        if(_98 >= mConst->getSlopeRollingReStartInterval()) {
            bool v24;
            if(mJudgeStartRolling->isTriggerRestartSwing()) {
                v24 = mConst->getSlopeRollingReStartSwing() <= _9C;
            } else if(mConst->getSlopeRollingReStartForce() <= _A0) {
                v24 = true;
            } else {
                v24 = mConst->getSlopeRollingReStartCharge() <= _9C;
            }
            restartRolling(v24, mJudgeStartRolling->isTriggerRestartSwing());
            al::setNerve(this, &Start);
            return;
        }
    }

    if(rs::isCollidedGroundLessAngle(mActor, mCollider, mConst->getStandAngleMin()) && rs::updateJudgeAndResult(mJudgeEnableStandUp) && rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
        al::setNerve(this, &Jump);
        return;
    }

    if(_B0) {
        al::setNerve(this, &Rolling);
        return;
    }

    if(!mInput->isHoldSquat() && mConst->getSlopeRollingFrameMin() <= _98 && rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        al::setNerve(this, &StandUp);
        return;
    }

    if(al::isGreaterEqualStep(this, mConst->getSlopeRollingUnRollFrame())) {
        al::setNerve(this, &Brake);
        return;
    }
}
void PlayerStateRolling::exeBrake() {
    if(al::isFirstStep(this)) {}
    
    sead::Vector3f v38 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrUpDir(&v38, mActor, mCollider);

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, v38);

    sead::Vector3f v36 = {0.0f, 0.0f, 0.0f};
    rs::moveInertiaSlide(&v36, mActor, mCollider, moveInput, mConst->getSlopeRollingEndBrake(), 0.0f, 0.0f, 0.0f, mConst->getGravity(), mConst->getGravityAir(), mConst->getFallSpeedMax());
    rs::slerpUp(mActor, v38, mConst->getSlerpQuatRate(), mConst->getHillPoseDegreeMax());
    updateRollingAnimFrameRate();

    if(rs::isCollidedGround(mCollider))
        mEffect->tryEmitRollingEffect();
    else
        mEffect->tryDeleteRollingEffect();

    if(rs::updateJudgeAndResult(mJudgeStartRolling)) {
        if(_98 >= mConst->getSlopeRollingReStartInterval()) {
            bool v24;
            if(mJudgeStartRolling->isTriggerRestartSwing()) {
                v24 = mConst->getSlopeRollingReStartSwing() <= _9C;
            } else if(mConst->getSlopeRollingReStartForce() <= _A0) {
                v24 = true;
            } else {
                v24 = mConst->getSlopeRollingReStartCharge() <= _9C;
            }
            restartRolling(v24, mJudgeStartRolling->isTriggerRestartSwing());
            al::setNerve(this, &Start);
            return;
        }
    }

    if(rs::isCollidedGroundLessAngle(mActor, mCollider, mConst->getStandAngleMin()) && rs::updateJudgeAndResult(mJudgeEnableStandUp) && rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
        al::setNerve(this, &Jump);
        return;
    }

    if(!rs::isCollidedGround(mCollider)) {
        kill();
        return;
    }

    if(!_B0) {
        if(!mInput->isHoldSquat() && mConst->getSlopeRollingFrameMin() <= _98 && rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
            al::setNerve(this, &StandUp);
            return;
        }
    }

    if(v36.length() > mConst->getSlopeRollingEndBrakeEndSpeed()) {
        al::setNerve(this, &EndSquat);
        return;
    }
}
void PlayerStateRolling::exeStandUp() {
    if(al::isFirstStep(this)) {
        mAnimator->startAnim("RollingStandUp");
        mEffect->tryDeleteRollingEffect();
    }

    sead::Vector3f v38 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrUpDir(&v38, mActor, mCollider);

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, v38);

    sead::Vector3f v36 = {0.0f, 0.0f, 0.0f};
    rs::moveInertiaSlide(&v36, mActor, mCollider, moveInput, mConst->getSlopeRollingBrake(), 0.0f, 0.0f, 0.0f, mConst->getGravity(), mConst->getGravityAir(), mConst->getFallSpeedMax());
    rs::slerpUp(mActor, v38, mConst->getSlerpQuatRate(), mConst->getHillPoseDegreeMax());

    if(_B0) {
        al::setNerve(this, &Rolling);
        return;
    }

    if(mAnimator->isAnimEnd()) {
        kill();
        return;
    }
}
void PlayerStateRolling::exeEndSquat() {
    if(al::isFirstStep(this)) {
        mEffect->tryDeleteRollingEffect();
    }
}
