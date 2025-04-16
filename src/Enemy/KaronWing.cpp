#include "Enemy/KaronWing.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorParamMove.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Player/PlayerUtil.h"

#include "Enemy/EnemyCap.h"
#include "Enemy/EnemyStateHackStart.h"
#include "Enemy/EnemyStateReviveInsideScreen.h"
#include "Enemy/EnemyStateSwoon.h"
#include "Enemy/FlyerStateWander.h"
#include "Enemy/KaronWingStateHack.h"
#include "Util/Hack.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(KaronWing, Attack);
NERVE_IMPL(KaronWing, Break);
NERVE_IMPL(KaronWing, Chase);
NERVE_IMPL(KaronWing, DamageCap);
NERVE_IMPL(KaronWing, Find);
NERVE_IMPL(KaronWing, Hack);
NERVE_IMPL(KaronWing, HackStart);
NERVE_IMPL(KaronWing, Revive);
NERVE_IMPL(KaronWing, ReviveAppear);
NERVE_IMPL(KaronWing, Swoon);
NERVE_IMPL(KaronWing, Turn);
NERVE_IMPL(KaronWing, Wait);
NERVE_IMPL(KaronWing, Wander);

NERVES_MAKE_STRUCT(KaronWing, Wait, Hack, Swoon, Break, Revive, HackStart, Wander, Attack,
                   DamageCap, Turn, Find, Chase, ReviveAppear);
}  // namespace

const al::ActorParamMove cMoveParam{0.1f, 0.0f, 0.95f, 0.7f};
static FlyerStateWanderParam cWanderParam{30, 540, 180, "EnemyFly", &cMoveParam};

// NON_MATCHING: creating the `EnemyStateSwoonInitParam` (https://decomp.me/scratch/CDB2W)
void KaronWing::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    bool wearingCap = true;
    al::tryGetArg(&wearingCap, info, "IsWearingCap");
    if (wearingCap)
        mEnemyCap = rs::tryCreateEnemyCap(this, info, "EnemyCapKoopa");

    al::initNerve(this, &NrvKaronWing.Wait, 8);

    mStateHack = new KaronWingStateHack(this, info, &mPlayerHack);
    al::initNerveState(this, mStateHack, &NrvKaronWing.Hack, "キャプチャー");
    mStateSwoon = new EnemyStateSwoon(this, "SwoonStart", "Swoon", "SwoonEnd", false, true);

    EnemyStateSwoonInitParam swoonParam{"Trampled",      "BreakWait", "Recover",
                                        "BreakReaction", "Break",     "BreakGroundHit"};
    swoonParam.hasStartLandAnimation = true;
    swoonParam.hasLockOnDelay = true;
    swoonParam.isCancelLoopOnProhibitedArea = false;
    swoonParam.swoonDuration = 180;
    swoonParam.endSignDelay = 60;
    swoonParam.endSignAnimName = "RecoverSign";

    mStateSwoon->initParams(swoonParam);
    al::initNerveState(this, mStateSwoon, &NrvKaronWing.Swoon, "気絶");

    mStateBreak = new EnemyStateSwoon(this, "SwoonStart", "Swoon", "SwoonEnd", false, true);
    mStateBreak->initParams(swoonParam);
    al::initNerveState(this, mStateBreak, &NrvKaronWing.Break, "壊れ");

    EnemyStateReviveInsideScreen* stateRevive = new EnemyStateReviveInsideScreen(this);
    al::initNerveState(this, stateRevive, &NrvKaronWing.Revive, "画面内復活");
    mSpawnTrans.set(al::getTrans(this));

    mStateHackStart = new EnemyStateHackStart(this, nullptr, nullptr);
    al::initNerveState(this, mStateHackStart, &NrvKaronWing.HackStart, "キャプチャー開始");

    mStateWander = new FlyerStateWander(this, &cWanderParam);
    al::initNerveState(this, mStateWander, &NrvKaronWing.Wander, "うろつき");

    mCapTargetInfo = rs::createCapTargetInfoWithPlayerCollider(this, mStateHack, nullptr);
    if (!al::trySyncStageSwitchAppear(this))
        makeActorAlive();
}

void KaronWing::control() {
    if (al::isNerve(this, &NrvKaronWing.HackStart))
        return;

    if (!al::isNerve(this, &NrvKaronWing.Revive) &&
        (al::isCollidedFloorCode(this, "DamageFire") || al::isCollidedFloorCode(this, "Needle") ||
         al::isCollidedFloorCode(this, "Poison"))) {
        al::startHitReaction(this, "死亡");
        al::setNerve(this, &NrvKaronWing.Revive);
    } else if (al::isInDeathArea(this) && !al::isNerve(this, &NrvKaronWing.Revive)) {
        al::startHitReaction(this, "死亡");
        al::setNerve(this, &NrvKaronWing.Revive);
    }
}

void KaronWing::updateCollider() {
    if (al::isNerve(this, &NrvKaronWing.Hack)) {
        mStateHack->updateCollider();
        return;
    }
    al::LiveActor::updateCollider();
}

void KaronWing::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvKaronWing.Revive))
        return;

    if (mPlayerHack) {
        rs::sendMsgHackerNoReaction(mPlayerHack, other, self);
        mStateHack->attackSensor(self, other);
        return;
    }

    if (al::isSensorEnemyAttack(self) && !al::isNerve(this, &NrvKaronWing.HackStart) &&
        !al::isNerve(this, &NrvKaronWing.Hack) && !al::isNerve(this, &NrvKaronWing.Swoon) &&
        !al::isNerve(this, &NrvKaronWing.Break)) {
        if (al::sendMsgEnemyAttack(other, self)) {
            al::faceToTarget(this, al::getSensorPos(other));
            setNerve(this, &NrvKaronWing.Attack);
            return;
        }

        al::sendMsgPush(other, self) || rs::sendMsgPushToPlayer(other, self);
    }
}

bool KaronWing::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                           al::HitSensor* self) {
    if (al::isNerve(this, &NrvKaronWing.Revive)) {
        return rs::isMsgPlayerDisregardHomingAttack(message) ||
               rs::isMsgPlayerDisregardTargetMarker(message) || al::isMsgPlayerDisregard(message);
    }

    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo))
        return true;

    if (al::isNerve(this, &NrvKaronWing.Hack) && mStateHack->receiveMsg(message, other, self)) {
        if (mStateHack->isEndCancel())
            al::setNerve(this, &NrvKaronWing.Swoon);
        else if (mStateHack->isEndReset()) {
            al::startHitReaction(this, "消滅");
            al::setNerve(this, &NrvKaronWing.Revive);
        } else if (mStateHack->isEndDamage())
            kill();
        return true;
    }

    if (rs::tryReceiveMsgNpcScareByEnemyIgnoreTargetHack(message, mCapTargetInfo))
        return true;

    if (!al::isNerve(this, &NrvKaronWing.Revive) && !al::isNerve(this, &NrvKaronWing.HackStart) &&
        !al::isNerve(this, &NrvKaronWing.Hack)) {
        if (rs::isMsgCapEnableLockOn(message))
            return !rs::isOnEnemyCap(mEnemyCap);
        if (rs::isMsgCapCancelLockOn(message))
            return true;
        if (mStateSwoon->tryReceiveMsgStartLockOn(message))
            return !rs::isOnEnemyCap(mEnemyCap);
        if (mStateSwoon->tryReceiveMsgStartHack(message)) {
            al::invalidateClipping(this);
            al::setNerve(this, &NrvKaronWing.HackStart);
            mPlayerHack = mStateHackStart->tryStart(message, other, self);
            return true;
        }
    }

    if (al::isNerve(this, &NrvKaronWing.Swoon) || al::isNerve(this, &NrvKaronWing.Break)) {
        if (mStateSwoon->tryReceiveMsgEnableLockOn(message))
            return true;
        if (mStateSwoon->tryReceiveMsgStartHack(message)) {
            al::setNerve(this, &NrvKaronWing.HackStart);
            mPlayerHack = mStateHackStart->tryStart(message, other, self);
            return true;
        }
        if (mStateSwoon->tryReceiveMsgEndSwoon(message))
            return true;
        if (rs::isMsgTankExplosion(message))
            (al::isNerve(this, &NrvKaronWing.Swoon) ? mStateSwoon : mStateBreak)->requestTrampled();
    }

    if (al::isNerve(this, &NrvKaronWing.ReviveAppear) || al::isNerve(this, &NrvKaronWing.Wait) ||
        al::isNerve(this, &NrvKaronWing.Turn) || al::isNerve(this, &NrvKaronWing.Wander) ||
        al::isNerve(this, &NrvKaronWing.Find) || al::isNerve(this, &NrvKaronWing.Chase)) {
        if (mStateSwoon->tryReceiveMsgAttack(message)) {
            if (rs::tryStartEnemyCapBlowDown(mEnemyCap, other)) {
                mDamageStartY = al::getTrans(this).y;
                al::setVelocityBlowAttackAndTurnToTarget(this, rs::getPlayerBodyPos(this), 7.0f,
                                                         25.0f);
                al::setNerve(this, &NrvKaronWing.DamageCap);
            }
            return true;
        }

        if (rs::isMsgPressDown(message) || rs::isMsgBlowDown(message)) {
            rs::tryStartEnemyCapBlowDown(mEnemyCap, other);
            rs::requestHitReactionToAttacker(message, self, other);
            al::setNerve(this, &NrvKaronWing.Break);
            return true;
        }
    }

    if (!mPlayerHack && !al::isNerve(this, &NrvKaronWing.Swoon) &&
        !al::isNerve(this, &NrvKaronWing.Break) &&
        al::tryReceiveMsgPushAndAddVelocity(this, message, other, self, 3.0f))
        return true;

    return false;
}

void KaronWing::exeWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        al::startAction(this, "FlyWait");
    }

    al::scaleVelocity(this, 0.7f);
    if (al::isGreaterEqualStep(this, 60))
        al::setNerve(this, &NrvKaronWing.Wander);
}

void KaronWing::exeWander() {
    al::updateNerveState(this);
    if (al::isNearPlayer(this, 1500.0f))
        al::setNerve(this, &NrvKaronWing.Turn);
}

void KaronWing::exeTurn() {
    if (al::isFirstStep(this))
        al::startAction(this, "EnemyFly");

    sead::Vector3f playerPos = al::getPlayerPos(this, 0);
    al::turnToTarget(this, playerPos, 2.3f);
    if (al::isFaceToTargetDegreeH(this, playerPos, al::getFront(this), 2.3f)) {
        al::setNerve(this, &NrvKaronWing.Find);
        return;
    }

    al::scaleVelocity(this, 0.7f);
}

void KaronWing::exeFind() {
    if (al::isFirstStep(this))
        al::startAction(this, "Find");

    if (al::isActionEnd(this)) {
        al::setNerve(this, &NrvKaronWing.Chase);
        return;
    }

    al::scaleVelocity(this, 0.7f);
}

void KaronWing::exeChase() {
    if (al::isFirstStep(this))
        al::startAction(this, "FlyChase");

    if (!al::isNearPlayer(this, 2000.0f)) {
        al::setNerve(this, &NrvKaronWing.Wait);
        return;
    }

    al::flyAndTurnToTarget(this, al::getPlayerPos(this, 0), 4.0f, 0.0f, 0.7f, 2.3f);
}

void KaronWing::exeRevive() {
    if (al::updateNerveStateAndNextNerve(this, &NrvKaronWing.ReviveAppear)) {
        mStateHack->resetFlyLimit(mSpawnTrans);
        rs::tryAppearEnemyCap(mEnemyCap);
        al::startVisAnim(this, "CapOn");
    }
}

void KaronWing::exeReviveAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "AppearStart");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvKaronWing.Wait);
}

void KaronWing::exeAttack() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "AttackHit");
        al::setVelocityZero(this);
    }

    if (al::isActionEnd(this)) {
        al::setNerve(this, &NrvKaronWing.Wait);
        return;
    }

    if (!al::isOnGround(this, 0))
        al::addVelocityToGravity(this, 1.0f);
    al::scaleVelocity(this, al::isOnGround(this, 0) ? 0.7f : 0.97f);
}

void KaronWing::exeSwoon() {
    if (al::isFirstStep(this))
        al::invalidateClipping(this);

    if (al::updateNerveState(this)) {
        al::startHitReaction(this, "消滅");
        al::setNerve(this, &NrvKaronWing.Revive);
        return;
    }

    if (!al::isOnGround(this, 0))
        al::addVelocityToGravity(this, 1.0f);
    al::scaleVelocity(this, al::isOnGround(this, 0) ? 0.7f : 0.97f);
}

void KaronWing::exeBreak() {
    if (al::isFirstStep(this))
        al::invalidateClipping(this);

    if (al::updateNerveState(this)) {
        al::setNerve(this, &NrvKaronWing.Wait);
        return;
    }

    if (!al::isOnGround(this, 0))
        al::addVelocityToGravity(this, 1.0f);
    al::scaleVelocity(this, al::isOnGround(this, 0) ? 0.7f : 0.97f);
}

void KaronWing::exeHackStart() {
    al::updateNerveStateAndNextNerve(this, &NrvKaronWing.Hack);
}

void KaronWing::exeHack() {
    al::updateNerveStateAndNextNerve(this, &NrvKaronWing.Swoon);
}

void KaronWing::exeDamageCap() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::startAction(this, "DamageCap");
    }

    if (!al::isOnGround(this, 0))
        al::addVelocityToGravity(this, 1.0f);
    al::scaleVelocity(this, al::isOnGround(this, 0) ? 0.7f : 0.97f);

    if (al::isActionEnd(this)) {
        const sead::Vector3f& trans = al::getTrans(this);
        const sead::Vector3f& velocity = al::getVelocity(this);
        if (trans.y + velocity.y <= mDamageStartY || al::isOnGround(this, 0))
            al::setNerve(this, &NrvKaronWing.Wait);
    }
}
