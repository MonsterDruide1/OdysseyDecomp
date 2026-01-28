#include "Enemy/CatchBomb.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Collision/KCollisionServer.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Boss/Loop/LoopAnimState.h"
#include "Enemy/DisregardReceiver.h"
#include "MapObj/CapMessageShowInfo.h"
#include "Util/Hack.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CatchBomb, Wait);
NERVE_IMPL(CatchBomb, Catch);
NERVE_IMPL(CatchBomb, Hide);
NERVE_IMPL(CatchBomb, Explode);
NERVE_IMPL(CatchBomb, Pull);
NERVE_IMPL(CatchBomb, March);
NERVE_IMPL(CatchBomb, Find);
NERVE_IMPL(CatchBomb, Back);
NERVE_IMPL(CatchBomb, Move);
NERVE_IMPL(CatchBomb, Turn);
NERVE_IMPL(CatchBomb, Appear);
NERVE_IMPL(CatchBomb, Throw);
NERVE_IMPL(CatchBomb, Attack);
NERVE_IMPL(CatchBomb, Fall);

NERVES_MAKE_STRUCT(CatchBomb, Wait, Catch, Hide, Explode, Pull, March, Find, Back, Move, Turn,
                   Appear, Throw, Attack, Fall);
}  // namespace

CatchBomb::CatchBomb(const char* name) : al::LiveActor(name) {}

void CatchBomb::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &NrvCatchBomb.Wait, 1);
    mCatchState = new LoopAnimState(this, "Catch", false);
    al::initNerveState(this, mCatchState, &NrvCatchBomb.Catch, "キャッチ");
    al::invalidateHitSensor(this, "Explosion");
    al::tryGetArg(&mIsRevival, info, "Revival");
    mSpawnPos.set(al::getTrans(this));
    mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);
    mCollisionPartsFilter = new al::CollisionPartsFilterSpecialPurpose("MoveLimit");
    al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);
    al::initJointControllerKeeper(this, 2);
    mJointSpringControllerHolder = new al::JointSpringControllerHolder();
    mJointSpringControllerHolder->init(this, "InitJointSpringCtrl");
    makeActorAlive();
    al::invalidateShadowMask(this, "ダミー帽子影");
    mDisregardReceiver = new DisregardReceiver(this, nullptr);
    mCatchBombGeneratePoint = new al::LiveActor("キャッチボム出現ポイント");
    al::initActorWithArchiveName(mCatchBombGeneratePoint, info, "CatchBombGeneratePoint", nullptr);
    mCatchBombGeneratePoint->makeActorAlive();
}

inline bool isAppearNew(const CatchBomb* self) {
    return al::isNerve(self, &NrvCatchBomb.Appear) && al::isLessEqualStep(self, 60);
}

void CatchBomb::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorName(other, "SpinAttack") || al::isNerve(this, &NrvCatchBomb.Hide))
        return;

    if (al::isSensorName(self, "Explosion")) {
        if (!al::isSensorPlayer(other))
            al::sendMsgExplosion(other, self, nullptr);
        return;
    }

    if (al::isNerve(this, &NrvCatchBomb.Explode))
        return;

    if (isEnableExplosion()) {
        if (al::isSensorName(self, "Head") &&
            !(al::isSensorPlayer(other) && rs::isPlayerSquat(this))) {
            if (!rs::sendMsgCatchBombThrough(other, self) &&
                al::sendMsgExplosion(other, self, nullptr)) {
                tryShiftExplode(false);
                return;
            }

            al::sendMsgPush(other, self);
            rs::sendMsgPushToPlayer(other, self);
        }
        return;
    }

    if ((al::isSensorPlayer(other) || al::isSensorEnemyBody(other)) &&
        al::isSensorName(self, "Body")) {
        if (isEnableEnemyAttack() && al::sendMsgEnemyAttack(other, self)) {
            tryShiftAttack();
            return;
        }

        al::sendMsgPush(other, self);
        rs::sendMsgPushToPlayer(other, self);
    }
}

bool CatchBomb::isEnableExplosion() const {
    return al::isNerve(this, &NrvCatchBomb.Pull) || al::isNerve(this, &NrvCatchBomb.March);
}

bool CatchBomb::tryShiftExplode(bool unused) {
    if (!al::isNerve(this, &NrvCatchBomb.Explode)) {
        requestCapHitReaction("キャッチボム捕まって爆発", false);
        al::setNerve(this, &NrvCatchBomb.Explode);
        return true;
    }
    return false;
}

bool CatchBomb::isEnableEnemyAttack() const {
    if (isAppearNew(this) || al::isNerve(this, &NrvCatchBomb.Explode) ||
        al::isNerve(this, &NrvCatchBomb.Pull) || al::isNerve(this, &NrvCatchBomb.March)) {
        return false;
    }
    return true;
}

void CatchBomb::tryShiftAttack() {
    if (al::isNerve(this, &NrvCatchBomb.Wait) || al::isNerve(this, &NrvCatchBomb.Move) ||
        al::isNerve(this, &NrvCatchBomb.Turn)) {
        al::setNerve(this, &NrvCatchBomb.Attack);
    }
}

bool CatchBomb::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                           al::HitSensor* self) {
    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo))
        return true;

    if (mDisregardReceiver->receiveMsg(message, other, self))
        return true;

    if (al::isNerve(this, &NrvCatchBomb.Hide))
        return false;

    if (al::isNerve(this, &NrvCatchBomb.Wait) && tryReceiveMsgPushLocal(message, other, self))
        return true;

    if (rs::isMsgCapReflect(message))
        return isCapReflectNerve();

    if (!isUncapNerve()) {
        if (rs::isMsgCapStartLockOn(message)) {
            mCap = al::getSensorHost(other);
            al::invalidateClipping(this);
            mExplodeTimer = 0;
            al::setNerve(this, &NrvCatchBomb.Catch);
            return true;
        }

        if (tryReceiveMsgPushLocal(message, other, self))
            return true;
    }

    if (al::isNerve(this, &NrvCatchBomb.Catch) || isEnableExplosion()) {
        if (rs::isMsgCapKeepLockOn(message))
            return true;

        if (rs::isMsgCapIgnoreCancelLockOn(message))
            return true;

        if (tryReceiveMsgPushLocal(message, other, self))
            return true;
    }

    if (rs::isMsgCapCancelLockOn(message)) {
        tryShiftExplode(false);
        return true;
    }

    if (!al::isNerve(this, &NrvCatchBomb.Explode) &&
        (al::isMsgExplosion(message) || rs::isMsgTsukkunThrust(message, nullptr))) {
        tryShiftExplode(false);
        return true;
    }

    if (rs::isMsgNpcScareByEnemy(message))
        return true;
    return false;
}

bool CatchBomb::tryReceiveMsgPushLocal(const al::SensorMsg* message, al::HitSensor* other,
                                       al::HitSensor* self) {
    if (al::isSensorName(self, "Body") &&
        al::tryReceiveMsgPushAndAddVelocityH(this, message, other, self, 3.0f)) {
        return true;
    }
    return false;
}

bool CatchBomb::isCapReflectNerve() const {
    return al::isNerve(this, &NrvCatchBomb.Throw);
}

bool CatchBomb::isUncapNerve() const {
    return isAppearNew(this) || al::isNerve(this, &NrvCatchBomb.Back) ||
           al::isNerve(this, &NrvCatchBomb.Hide) || al::isNerve(this, &NrvCatchBomb.Catch) ||
           isEnableExplosion() || al::isNerve(this, &NrvCatchBomb.Explode) || isCapReflectNerve();
}

void CatchBomb::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    calcGravity();
    calcBrake();

    if (mIsFirstWait) {
        mIsFirstWait = false;
        return;
    }

    if (tryShiftFall())
        return;

    if (tryShiftDamageCollisionExplode())
        return;

    if (mPlayerFarCounter == 0 && al::isNear(this, al::getPlayerActor(this, 0), 1000.0f)) {
        al::setNerve(this, &NrvCatchBomb.Find);
        return;
    }

    if (mPlayerFarCounter > 120 && al::isFar(this, mSpawnPos, 300.0f))
        al::setNerve(this, &NrvCatchBomb.Back);
}

void CatchBomb::calcGravity() {
    if (al::isOnGround(this, 0))
        al::setVelocityZeroY(this);
    al::addVelocityY(this, -0.5f);
}

void CatchBomb::calcBrake() {
    al::scaleVelocityHV(this, 0.95f, 1.0f);
}

bool CatchBomb::tryShiftFall() {
    if (!al::isOnGround(this, 4)) {
        al::setNerve(this, &NrvCatchBomb.Fall);
        return true;
    }
    return false;
}

bool CatchBomb::tryShiftDamageCollisionExplode() {
    if (al::isCollidedFloorCode(this, "DamageFire") || al::isCollidedFloorCode(this, "Needle") ||
        al::isCollidedFloorCode(this, "Poison")) {
        tryShiftExplode(false);
        return true;
    }
    return false;
}

void CatchBomb::exeFind() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Find");
        mTurnTarget.set(al::getPlayerPos(this, 0));
    }

    calcGravity();
    calcBrake();
    al::turnToTarget(this, mTurnTarget, 7.0f);

    if (tryShiftFall())
        return;

    if (tryShiftDamageCollisionExplode())
        return;

    if (tryShiftWait())
        return;

    if (al::isActionEnd(this)) {
        sead::Vector3f frontDir;
        al::calcFrontDir(&frontDir, this);
        if (al::isFaceToTargetDegreeH(this, al::getPlayerPos(this, 0), frontDir, 90.0f))
            al::setNerve(this, &NrvCatchBomb.Move);
        else
            al::setNerve(this, &NrvCatchBomb.Turn);
    }
}

bool CatchBomb::tryShiftWait() {
    if (mPlayerFarCounter > 60) {
        al::setNerve(this, &NrvCatchBomb.Wait);
        return true;
    }
    return false;
}

void CatchBomb::exeMove() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "Walk");

    if (al::isCollidedWallVelocity(this)) {
        sead::Vector3f* vel = al::getVelocityPtr(this);
        al::verticalizeVec(vel, al::getCollidedWallNormal(this), *vel);
    }

    calcGravity();
    calcBrake();

    if (!al::isNearPlayer(this, 100.0f))
        al::turnToTarget(this, al::getPlayerPos(this, 0), 2.0f);

    sead::Vector3f dir = al::getPlayerPos(this, 0) - al::getTrans(this);
    dir.y = 0.0f;
    al::tryNormalizeOrZero(&dir);
    sead::Vector3f vel = al::getVelocity(this);
    vel.y = 0.0f;
    al::addVelocity(this, (dir * 5.0f - vel) * 0.05f);

    if (tryShiftFall())
        return;

    if (tryShiftDamageCollisionExplode())
        return;

    if (tryShiftWait())
        return;

    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    if (!al::isFaceToTargetDegreeH(this, al::getPlayerPos(this, 0), frontDir, 90.0f))
        al::setNerve(this, &NrvCatchBomb.Turn);
}

void CatchBomb::exeTurn() {
    if (al::isFirstStep(this))
        al::startAction(this, "Turn");

    calcGravity();
    calcBrake();

    al::turnToTarget(this, al::getPlayerPos(this, 0), 2.0f);

    if (tryShiftFall())
        return;

    if (tryShiftDamageCollisionExplode())
        return;

    if (tryShiftWait())
        return;

    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    if (al::isFaceToTargetDegreeH(this, al::getPlayerPos(this, 0), frontDir, 30.0f))
        al::setNerve(this, &NrvCatchBomb.Move);
}

void CatchBomb::exeFall() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    calcGravity();

    if (tryShiftDamageCollisionExplode())
        return;

    if (isOnGround(this, 0))
        al::setNerve(this, &NrvCatchBomb.Move);
}

void CatchBomb::exeBack() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Disappear");
        al::setVelocityZeroH(this);
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvCatchBomb.Hide);
}

void CatchBomb::exeAttack() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Attack");
        al::setVelocityZeroH(this);
    }

    calcGravity();
    calcBrake();

    if (tryShiftFall())
        return;

    if (tryShiftDamageCollisionExplode())
        return;

    if (al::isActionPlaying(this, "Attack") && al::isActionEnd(this))
        al::startAction(this, "Wait");

    if (al::isActionPlaying(this, "Wait") && al::isGreaterEqualStep(this, 90))
        al::setNerve(this, &NrvCatchBomb.Move);
}

void CatchBomb::exeCatch() {
    if (al::isFirstStep(this)) {
        al::setColliderFilterCollisionParts(this, nullptr);
        rs::tryShowCapMsgCatchBombCatchFirst(this);
        al::validateShadowMask(this, "ダミー帽子影");
    }

    if (al::isGreaterEqualStep(this, 106))
        al::holdSe(this, "CountFastLv");
    else
        al::holdSe(this, "CountNormalLv");

    if (al::getNerveStep(this) == al::getActionFrameMax(this, "CatchStart"))
        requestCapHitReaction("キャッチボム捕まった", false);

    if (al::getNerveStep(this) == al::getActionFrameMax(this, "CatchStart") + 38.0f)
        requestCapHitReaction("キャッチボム突進予兆１", false);

    if (al::getNerveStep(this) == al::getActionFrameMax(this, "CatchStart") + 91.0f)
        requestCapHitReaction("キャッチボム突進予兆２", false);

    al::faceToTarget(this, al::getPlayerPos(this, 0));
    calcGravity();
    calcBrake();
    al::updateNerveState(this);

    if (al::isGreaterEqualStep(this, 146))
        al::setNerve(this, &NrvCatchBomb.Pull);
}

void CatchBomb::requestCapHitReaction(const char* reactionName, bool unused) {
    rs::requestLockOnCapHitReaction(this, mCapTargetInfo, reactionName);
}

void CatchBomb::exePull() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "CatchEnd");
        al::setVelocityZero(this);
    }

    if (!al::isActionPlaying(this, "CatchEndLoop") && al::isActionEnd(this)) {
        al::startAction(this, "CatchEndLoop");
        requestCapHitReaction("キャッチボム捕まって突進", false);
    }

    sead::Vector3f dir = rs::getPlayerHeadPos(this) - al::getTrans(this);
    dir.y = 0.0f;
    if (!al::tryNormalizeOrZero(&dir))
        al::calcFrontDir(&dir, this);
    al::faceToDirection(this, dir);

    if (checkCollisionExplode(dir))
        return;

    if (al::isGreaterEqualStep(this, 2) && al::isNearPlayerH(this, 1000.0f)) {
        al::setNerve(this, &NrvCatchBomb.March);
        return;
    }

    al::addVelocity(this, (dir * 35.0 - al::getVelocity(this)) * 0.3f);
}

bool CatchBomb::checkCollisionExplode(const sead::Vector3f& dir) {
    al::HitSensor* hitSensor = nullptr;
    if (al::isCollidedWallVelocity(this))
        hitSensor = al::getCollidedWallSensor(this);

    if (!hitSensor && al::isCollidedGround(this) &&
        dir.dot(sead::Vector3f::ey) < sead::Mathf::cos(sead::Mathf::deg2rad(135.0f))) {
        hitSensor = al::getCollidedGroundSensor(this);
    }

    if (!hitSensor && al::isCollidedCeilingVelocity(this))
        hitSensor = al::getCollidedCeilingSensor(this);

    if (hitSensor) {
        al::sendMsgExplosionCollide(hitSensor, al::getHitSensor(this, "Body"), nullptr);
        al::invalidateHitSensors(this);

        tryShiftExplode(false);
        return true;
    }
    return false;
}

void CatchBomb::exeMarch() {
    if (al::isFirstStep(this)) {
        f32 speed = al::getVelocity(this).length();
        sead::Vector3f dir = rs::getPlayerHeadPos(al::getPlayerActor(this, 0)) - al::getTrans(this);
        dir.y = 0.0f;
        if (al::tryNormalizeOrZero(&dir))
            al::getVelocityPtr(this)->set(dir * speed);
    }

    if (!al::isActionPlaying(this, "CatchEndLoop") && al::isActionEnd(this)) {
        al::startAction(this, "CatchEndLoop");
        requestCapHitReaction("キャッチボム捕まって突進", false);
    }

    if (al::getVelocity(this).squaredLength() < sead::Mathf::square(35.0f)) {
        sead::Vector3f dir = al::getVelocity(this);
        al::tryNormalizeOrZero(&dir);
        dir *= 35.0f;
        al::addVelocity(this, (dir - al::getVelocity(this)) * 0.3f);
    }

    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    if (!checkCollisionExplode(frontDir) && al::isGreaterEqualStep(this, 150))
        tryShiftExplode(false);
}

void CatchBomb::exeExplode() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Explode");
        al::validateHitSensor(this, "Explosion");
        al::setVelocityZero(this);
        al::invalidateShadowMask(this, "ダミー帽子影");
    }

    if (al::isActionEnd(this)) {
        al::validateHitSensors(this);
        al::invalidateHitSensor(this, "Explosion");
        if (mIsRevival) {
            al::setNerve(this, &NrvCatchBomb.Hide);
            return;
        }
        kill();
    }
}

void CatchBomb::exeHide() {
    if (al::isFirstStep(this)) {
        al::hideModelIfShow(this);
        al::invalidateShadowMask(this, "ダミー帽子影");
    }

    if (al::isGreaterEqualStep(this, 60)) {
        al::validateClipping(this);
        al::resetPosition(this, mSpawnPos);
        al::setNerve(this, &NrvCatchBomb.Appear);
    }
}

void CatchBomb::exeAppear() {
    if (al::isFirstStep(this)) {
        al::showModelIfHide(this);
        al::startAction(this, "Appear");
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);
        al::invalidateShadowMask(this, "ダミー帽子影");
        mCap = nullptr;
    }

    calcGravity();
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvCatchBomb.Wait);
}

void CatchBomb::exeThrow() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Throw");
        mCap = nullptr;
    }

    calcGravity();
    if (al::isOnGround(this, 0)) {
        al::setVelocityZeroY(this);
        al::setNerve(this, &NrvCatchBomb.Move);
    }
}

void CatchBomb::initThrow(const al::ActorInitInfo& info) {
    al::initChildActorWithArchiveNameNoPlacementInfo(this, info, "CatchBomb", nullptr);
    al::initNerve(this, &NrvCatchBomb.Throw, 1);
    mCatchState = new LoopAnimState(this, "Catch", false);
    al::initNerveState(this, mCatchState, &NrvCatchBomb.Catch, "キャッチ");
    al::invalidateHitSensor(this, "Explosion");
    al::createAndSetColliderSpecialPurpose(this, "MoveLimit");
    mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);
    makeActorDead();
}

void CatchBomb::appearThrow(const sead::Vector3f& pos, const sead::Vector3f& vel) {
    mExplodeTimer = 0;
    al::setVelocity(this, vel);
    al::resetPosition(this, pos);
    al::setNerve(this, &NrvCatchBomb.Throw);
    al::invalidateClipping(this);
    appear();
}

void CatchBomb::control() {
    mDisregardReceiver->set44(al::getSensorPos(this, "Body") +
                              al::getSensorRadius(this, "Body") * sead::Vector3f::ey);

    bool isUncap = isUncapNerve();
    mDisregardReceiver->setEDC(false, false, isUncap);

    if (!al::isNearPlayerH(this, 2000.0f) ||
        sead::Mathf::abs(al::getPlayerPos(this, 0).y - al::getTrans(this).y) > 200.0f)
        mPlayerFarCounter += 1;
    else
        mPlayerFarCounter = 0;

    if (al::isNerve(this, &NrvCatchBomb.Explode) || al::isNerve(this, &NrvCatchBomb.Hide))
        return;

    if (al::isInDeathArea(this))
        tryShiftExplode(false);

    if (mExplodeTimer > 0) {
        mExplodeTimer--;
        if (mExplodeTimer == 0)
            tryShiftExplode(false);
    }
}
