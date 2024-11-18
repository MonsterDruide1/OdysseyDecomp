#include "Enemy/KuriboMini.h"

#include "Enemy/EnemyStateReset.h"
#include "Item/ItemUtil.h"
#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/VectorUtil.h"
#include "Library/Movement/EnemyStateBlowDown.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Se/SeFunction.h"
#include "Project/HitSensor/HitSensor.h"
#include "Util/ActorStateSandGeyser.h"
#include "Util/Sensor.h"
#include "Util/StageSensorMsgFunction.h"
#include "prim/seadRuntimeTypeInfo.h"

namespace {
NERVE_IMPL(KuriboMini, Wait);
NERVE_IMPL(KuriboMini, Reset);
NERVE_IMPL(KuriboMini, Wander);
NERVE_IMPL(KuriboMini, BlowDown);
NERVE_IMPL(KuriboMini, SandGeyser);
NERVE_IMPL(KuriboMini, Hide);
NERVE_IMPL(KuriboMini, PopAppearStart);
NERVE_IMPL(KuriboMini, PressDown);
NERVE_IMPL(KuriboMini, Turn);
NERVE_IMPL(KuriboMini, Find);
NERVE_IMPL(KuriboMini, ChaseReady);
NERVE_IMPL(KuriboMini, Chase);
NERVE_IMPL(KuriboMini, Stop);
NERVE_IMPL(KuriboMini, Attack);
NERVE_IMPL(KuriboMini, Land);
NERVE_IMPL(KuriboMini, Sink);
NERVE_IMPL(KuriboMini, Fall);
NERVE_IMPL(KuriboMini, PopAppear);
NERVE_IMPL(KuriboMini, PopAppearEnd);
NERVE_IMPL(KuriboMini, Bind);
NERVE_IMPL(KuriboMini, Blow);
NERVE_IMPL(KuriboMini, Drown);

NERVES_MAKE_STRUCT(KuriboMini, Wait, Reset, Wander, BlowDown, SandGeyser, Hide, PopAppearStart,
                   PressDown, Turn, Find, ChaseReady, Chase, Stop, Attack, Land, Sink, Fall,
                   PopAppear, PopAppearEnd, Bind, Blow, Drown);
}  // namespace

void KuriboMini::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvKuriboMini.Reset) || al::isNerve(this, &NrvKuriboMini.PressDown) ||
        al::isNerve(this, &NrvKuriboMini.BlowDown) || al::isNerve(this, &NrvKuriboMini.Bind))
        return;

    if (al::isSensorEye(self)) {
        rs::sendMsgKuriboFlick(other, self);
        return;
    }

    if (al::isSensorEnemyAttack(self)) {
        if (al::isNerve(this, &NrvKuriboMini.PopAppearStart) ||
            al::isNerve(this, &NrvKuriboMini.PopAppear) ||
            al::isNerve(this, &NrvKuriboMini.PopAppearEnd))
            return;

        if (al::sendMsgEnemyAttack(other, self)) {
            al::faceToTarget(this, al::getSensorPos(other));
            al::setNerve(this, &NrvKuriboMini.Attack);
        } else {
            rs::sendMsgPushToPlayer(other, self);
        }
        return;
    }

    if (al::isSensorEnemy(other) && al::isSensorName(self, "PushBody")) {
        al::sendMsgPush(other, self);
        return;
    }
}

bool KuriboMini::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(msg) || rs::isMsgPlayerDisregardTargetMarker(msg) ||
        al::isMsgPlayerDisregard(msg)) {
        if (al::isSensorName(self, "Body"))
            return al::isNerve(this, &NrvKuriboMini.PressDown) ||
                   al::isNerve(this, &NrvKuriboMini.BlowDown);
        return true;
    }

    if (al::isNerve(this, &NrvKuriboMini.Reset))
        return false;

    if (al::isNerve(this, &NrvKuriboMini.PressDown) || al::isNerve(this, &NrvKuriboMini.BlowDown)) {
        if (rs::isMsgKillByShineGet(msg)) {
            prepareKillByShineGet();
            return true;
        }
        return false;
    }

    if (rs::isMsgYoshiTongueEatBind(msg) && !al::isNerve(this, &NrvKuriboMini.Bind)) {
        rs::setMsgYoshiTongueEatBindRadiusAndOffset(msg, 45.0f, 45.0f);
        rs::requestHitReactionToAttacker(msg, self, other);
        al::setNerve(this, &NrvKuriboMini.Bind);
        return true;
    }

    if (rs::isMsgYoshiEatBindFinish(msg)) {
        rs::requestHitReactionToAttacker(msg, self, other);
        rs::setAppearItemFactorAndOffsetByMsg(this, msg, other);
        al::appearItem(this);
        kill();
        return true;
    }

    if (rs::isMsgYoshiEatBindCancel(msg)) {
        kill();
        return true;
    }

    if (!al::isNerve(this, &NrvKuriboMini.Bind)) {
        if (rs::isMsgNpcScareByEnemy(msg) || rs::isMsgTankLookOn(msg))
            return true;
        return tryReceiveMsgNormal(msg, other, self);
    }

    return false;
}

void KuriboMini::prepareKillByShineGet() {
    al::deleteEffectAll(this);
    al::resetPosition(this, mStateReset->getPosition());
    al::hideModelIfShow(this);
    al::setNerve(this, &NrvKuriboMini.Reset);
}

inline void prepareBlowDown(KuriboMini* own, al::EnemyStateBlowDown* blowDown, const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    rs::setAppearItemFactorAndOffsetByMsg(own, msg, other);
    sead::Vector3f blowDownDir;
    rs::calcBlowDownDir(&blowDownDir, msg, other, self);
    blowDown->start(blowDownDir);
    rs::requestHitReactionToAttacker(msg, self, other);
    al::setNerve(own, &NrvKuriboMini.BlowDown);
}

bool KuriboMini::tryReceiveMsgNormal(const al::SensorMsg* msg, al::HitSensor* other,
                                     al::HitSensor* self) {
    if (al::isMsgPlayerHipDropKnockDown(msg)) {
        sead::Vector3f blowDownDir;
        rs::calcBlowDownDir(&blowDownDir, msg, other, self);
        al::setVelocity(this, (sead::Vector3f::ey * 15.0f) + (blowDownDir * 7.0f));

        sead::Quatf quat;
        al::makeQuatUpFront(&quat, sead::Vector3f::ey, -blowDownDir);
        al::setQuat(this, quat);
        al::setNerve(this, &NrvKuriboMini.Blow);
        return true;
    }

    if (rs::isMsgCapAttack(msg)) {
        prepareBlowDown(this, mStateBlowDown, msg, other, self);
        return false;  // huh? handle it, but return false?
    }

    if (rs::isMsgPressDown(msg) && al::isSensorName(self, "Body")) {
        al::startAction(this, "PressDown");
        int comboCount = 0;
        if (!rs::tryIncrementComboCount(&comboCount, msg)) {
            rs::setAppearItemFactorAndOffsetByMsg(this, msg, other);
        } else {
            if (comboCount == 32) {
                rs::setAppearItemFactorAndOffsetForCombo(this, msg, other, true);
                al::startSeWithParam((al::LiveActor*) this, "BeatSeqCombo", comboCount, "");
            } else if (comboCount == 8) {
                rs::setAppearItemFactorAndOffsetForCombo(this, msg, other, false);
                al::startSeWithParam((al::LiveActor*) this, "BeatSeqCombo", comboCount, "");
            } else {
                rs::setAppearItemFactorAndOffsetByMsg(this, msg, other);
                if (comboCount >= 2) {
                    // use lambda here to avoid creating a new function
                    // required for matching stack management
                    [this, comboCount](){al::startSeWithParam((al::LiveActor*) this, "BeatSeq", comboCount, "");}();
                }
            }
        }

        rs::requestHitReactionToAttacker(msg, self, other);
        al::setNerve(this, &NrvKuriboMini.PressDown);
        return true;
    }

    if (rs::isMsgBlowDown(msg) || rs::isMsgGrowerAttack(msg) || rs::isMsgCapTrampolineAttack(msg)) {
        prepareBlowDown(this, mStateBlowDown, msg, other, self);
        return true;
    }

    if (rs::isMsgFireDamageAll(msg)) {
        al::startHitReaction(this, "死亡");
        kill();
        return true;
    }

    if (rs::isMsgHackAttack(msg)) {
        prepareBlowDown(this, mStateBlowDown, msg, other, self);
        return true;
    }

    if (mStateSandGeyser->receiveMsgSandGeyser(msg, other)) {
        if (!al::isNerve(this, &NrvKuriboMini.SandGeyser))
            al::setNerve(this, &NrvKuriboMini.SandGeyser);
        return true;
    }

    if (al::isMsgPushAll(msg) && al::isSensorName(self, "Body")) {
        if (al::isNerve(this, &NrvKuriboMini.PopAppearStart))
            return false;

        if(al::isSensorNpc(other)) {
            if (al::tryReceiveMsgPushAndAddVelocity(this, msg, other, self, 7.0f))
                return true;
        } else {
        if (al::tryReceiveMsgPushAndAddVelocity(this, msg, other, self, 5.0f))
            return true;
        }

        if (rs::isMsgKuriboFlick(msg)) {
            al::pushAndAddVelocityH(this, other, self, 5.0f);
            al::setNerve(this, &NrvKuriboMini.Wander);
            return true;
        }
    }

    return false;
}
