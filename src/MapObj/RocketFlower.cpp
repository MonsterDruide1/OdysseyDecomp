#include "MapObj/RocketFlower.h"

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/RocketFlowerFunction.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(RocketFlower, Wait);
NERVE_IMPL(RocketFlower, Attach);
NERVE_IMPL(RocketFlower, WaitFollow);
NERVE_IMPL(RocketFlower, Follow);
NERVE_IMPL(RocketFlower, WaitAttach);

NERVES_MAKE_STRUCT(RocketFlower, Wait, Attach, WaitFollow, Follow, WaitAttach);
}  // namespace

RocketFlower::RocketFlower(const char* name) : al::LiveActor(name) {}

void RocketFlower::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &NrvRocketFlower.Wait, 0);
    makeActorAlive();

    mMtxConnector = al::tryCreateMtxConnector(this, info);

    mFlowerSub = new al::LiveActor("ロケットフラワーの花");
    al::initChildActorWithArchiveNameNoPlacementInfo(mFlowerSub, info, "RocketFlowerDash", nullptr);
    al::startAction(mFlowerSub, "Wait");
    mFlowerSub->makeActorDead();

    RocketFlowerFunction::createRocketFlowerEquipWatcherIfNotExist(this, info);
    al::setHitSensorPosPtr(this, "Equip", al::getTransPtr(mFlowerSub));
    al::startAction(this, "Wait");
}

void RocketFlower::initAfterPlacement() {
    if (mMtxConnector)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void RocketFlower::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::isSensorName(self, "Equip"))
        return;

    if ((!al::isNerve(this, &NrvRocketFlower.Follow) || al::isNewNerve(this)) &&
        !al::isNerve(this, &NrvRocketFlower.WaitAttach)) {
        return;
    }

    if (rs::sendMsgRocketFlowerExtension(other, self))
        al::setNerve(this, &NrvRocketFlower.Attach);
}

bool RocketFlower::isEnableEquip() const {
    if (al::isNerve(this, &NrvRocketFlower.Follow) && !al::isNewNerve(this))
        return true;

    return al::isNerve(this, &NrvRocketFlower.WaitAttach);
}

bool RocketFlower::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                              al::HitSensor* self) {
    if (al::isMsgPlayerSpinAttack(message)) {
        if (al::isNerve(this, &NrvRocketFlower.Wait)) {
            al::invalidateClipping(this);
            al::setNerve(this, &NrvRocketFlower.WaitFollow);
        }
    } else if (rs::isMsgCapItemGet(message)) {
        if (al::isNerve(this, &NrvRocketFlower.Wait)) {
            al::invalidateClipping(this);
            al::setNerve(this, &NrvRocketFlower.Follow);
            return true;
        }
    } else if (al::isMsgPlayerObjTouch(message) || rs::isMsgBlowObjAttack(message) ||
               rs::isMsgFireDamageAll(message) || rs::isMsgHammerBrosHammerEnemyAttack(message) ||
               rs::isMsgHammerBrosHammerHackAttack(message) || rs::isMsgHosuiAttack(message) ||
               al::isMsgEnemyAttack(message) || al::isMsgKickStoneAttack(message) ||
               rs::isMsgRadishAttack(message) || rs::isMsgSeedAttack(message) ||
               rs::isMsgTankBullet(message) || rs::isMsgGamaneBulletThrough(message) ||
               rs::isMsgHackAttackPoison(message) || rs::isMsgYoshiTongueAttack(message)) {
        if (al::isSensorMapObj(self)) {
            if (mReactionFrame == 0)
                al::startAction(this, "Reaction");

            mReactionFrame = 30;
        }
    } else if (rs::isMsgCapAttack(message)) {
        if (al::isSensorMapObj(self) && !al::isNerve(this, &NrvRocketFlower.Wait) &&
            !al::isNerve(this, &NrvRocketFlower.WaitFollow)) {
            if (mReactionFrame == 0)
                al::startAction(this, "Reaction");

            mReactionFrame = 30;
        }
    } else if (al::isMsgPlayerPutOnEquipment(message) && al::isSensorName(self, "Equip") &&
               ((al::isNerve(this, &NrvRocketFlower.Follow) && !al::isNewNerve(this)) ||
                al::isNerve(this, &NrvRocketFlower.WaitAttach))) {
        if (RocketFlowerFunction::requestEquipRocketFlower(this, other)) {
            al::setNerve(this, &NrvRocketFlower.Attach);
            return true;
        }

        al::invalidateHitSensor(this, "Equip");
        al::validateClipping(this);
        mFlowerSub->kill();
        al::startAction(this, "Appear");
        mReactionFrame = 30;
        al::startHitReaction(this, "復帰");
        al::setNerve(this, &NrvRocketFlower.Wait);
    }

    return false;
}

void RocketFlower::terminateFollow() {
    al::invalidateHitSensor(this, "Equip");
    al::validateClipping(this);
    mFlowerSub->kill();
    al::startAction(this, "Appear");
    mReactionFrame = 30;
    al::startHitReaction(this, "復帰");
    al::setNerve(this, &NrvRocketFlower.Wait);
}

void RocketFlower::exeWait() {
    if (al::isFirstStep(this)) {
        al::startVisAnim(this, "Show");
        al::invalidateHitSensor(this, "Equip");
    }
}

void RocketFlower::exeWaitFollow() {
    if (trySyncFlyingCapPos()) {
        al::setNerve(this, &NrvRocketFlower.Follow);
        return;
    }

    if (!al::isGreaterEqualStep(this, 30))
        return;

    al::validateClipping(this);
    al::setNerve(this, &NrvRocketFlower.Wait);
}

bool RocketFlower::trySyncFlyingCapPos() {
    sead::Vector3f flyingCapPos;
    if (!rs::tryGetFlyingCapPos(&flyingCapPos, this))
        return false;

    flyingCapPos += 50.0f * sead::Vector3f::ey;
    al::resetPosition(mFlowerSub, flyingCapPos);

    sead::Vector3f toPlayerHead = rs::getPlayerHeadPos(this) - flyingCapPos;
    if (al::tryNormalizeOrZero(&toPlayerHead)) {
        sead::Quatf quat;
        quat.makeVectorRotation(sead::Vector3f::ez, toPlayerHead);
        al::setQuat(mFlowerSub, quat);
    }

    return true;
}

void RocketFlower::exeFollow() {
    if (al::isFirstStep(this)) {
        al::startVisAnim(this, "Hide");
        mFlowerSub->appear();
        al::startAction(mFlowerSub, "Wait");
        al::startAction(this, "ReactionCap");
        mReactionFrame = 30;
        al::startHitReaction(this, "花が取れた");
        mFollowLostFrame = 0;
    }

    if (trySyncFlyingCapPos()) {
        mFollowLostFrame = 0;
        return;
    }

    if (rs::isEquipCapCatched(this)) {
        al::validateHitSensor(this, "Equip");
        al::setNerve(this, &NrvRocketFlower.WaitAttach);
        return;
    }

    u32 followLostFrame = mFollowLostFrame;
    if (followLostFrame >= 16) {
        al::invalidateHitSensor(this, "Equip");
        al::validateClipping(this);
        mFlowerSub->kill();
        al::startAction(this, "Appear");
        mReactionFrame = 30;
        al::startHitReaction(this, "復帰");
        al::setNerve(this, &NrvRocketFlower.Wait);
        followLostFrame = mFollowLostFrame;
    }

    mFollowLostFrame = followLostFrame + 1;
}

void RocketFlower::appearFlowerSub() {
    al::startVisAnim(this, "Hide");
    mFlowerSub->appear();
    al::startAction(mFlowerSub, "Wait");
    al::startAction(this, "ReactionCap");
    mReactionFrame = 30;
    al::startHitReaction(this, "花が取れた");
}

void RocketFlower::exeWaitAttach() {
    rs::tryCalcPlayerModelHeadJointPos(al::getTransPtr(mFlowerSub), this);

    if (!al::isGreaterEqualStep(this, 20))
        return;

    al::invalidateHitSensor(this, "Equip");
    al::validateClipping(this);
    mFlowerSub->kill();
    al::startAction(this, "Appear");
    mReactionFrame = 30;
    al::startHitReaction(this, "復帰");
    al::setNerve(this, &NrvRocketFlower.Wait);
}

void RocketFlower::exeAttach() {
    if (al::isFirstStep(this))
        al::startAction(mFlowerSub, "Dash");
}

void RocketFlower::setFollowFlowerPose(const sead::Quatf& quat, const sead::Vector3f& trans) {
    al::setQuat(mFlowerSub, quat);
    al::resetPosition(mFlowerSub, trans);
}

void RocketFlower::disappear() {
    al::startHitReaction(mFlowerSub, "消滅");
}

void RocketFlower::disappearForce() {
    al::tryKillEmitterAndParticleAll(mFlowerSub);
    al::invalidateHitSensor(this, "Equip");
    al::validateClipping(this);
    mFlowerSub->kill();
    al::startAction(this, "Wait");
    mReactionFrame = 30;
    al::setNerve(this, &NrvRocketFlower.Wait);
}

void RocketFlower::control() {
    if (al::isActionOneTime(this) && al::isActionEnd(this))
        al::startAction(this, "Wait");

    if (mReactionFrame != 0)
        mReactionFrame--;

    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}
