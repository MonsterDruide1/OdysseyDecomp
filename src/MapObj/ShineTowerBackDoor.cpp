#include "MapObj/ShineTowerBackDoor.h"

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Sequence/ChangeStageInfo.h"
#include "System/GameDataFunction.h"
#include "Util/PlayerPuppetFunction.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(ShineTowerBackDoor, Wait);
NERVE_IMPL(ShineTowerBackDoor, Enter);
NERVE_IMPL(ShineTowerBackDoor, Reaction);
NERVE_IMPL(ShineTowerBackDoor, ReactionWait);
NERVES_MAKE_STRUCT(ShineTowerBackDoor, Wait, Enter, ReactionWait, Reaction);
}  // namespace

ShineTowerBackDoor::ShineTowerBackDoor(const char* name) : LiveActor(name) {}

void ShineTowerBackDoor::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "ShineTowerBackDoor", nullptr);
    al::initNerve(this, &NrvShineTowerBackDoor.Wait, 0);
    mMtxConnector = al::createMtxConnector(this);
    makeActorAlive();
}

void ShineTowerBackDoor::control() {
    mBindTimer = sead::Mathi::clampMin(mBindTimer - 1, 0);
    al::connectPoseQT(this, mMtxConnector);
}

bool ShineTowerBackDoor::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                    al::HitSensor* self) {
    if (rs::isMsgKoopaRingBeamInvalidTouch(message) ||
        rs::isMsgPlayerDisregardTargetMarker(message) ||
        rs::isMsgPlayerDisregardHomingAttack(message))
        return true;

    if (al::isSensorMapObj(self)) {
        if (_118)
            return false;

        if (al::isMsgPlayerObjHipDropAll(message)) {
            mBindTimer = 3;
            return false;
        }
    }

    if (al::isSensorBindable(self)) {
        if (al::isMsgBindStart(message))
            return mBindTimer > 0;

        if (al::isMsgBindInit(message)) {
            al::invalidateClipping(this);
            mPlayerPuppet = rs::startPuppet(self, other);
            rs::setPuppetVelocity(mPlayerPuppet, sead::Vector3f::ex * -45.0f);
            rs::invalidatePuppetCollider(mPlayerPuppet);
            rs::hidePuppetSilhouette(mPlayerPuppet);
            al::setNerve(this, &NrvShineTowerBackDoor.Enter);
            return true;
        }

        if (al::isMsgBindCancel(message)) {
            mPlayerPuppet = nullptr;
            mBindTimer = 100;
            al::setNerve(this, &NrvShineTowerBackDoor.Wait);
            return true;
        }

        return false;
    }

    if (al::isNerve(this, &NrvShineTowerBackDoor.Enter))
        return false;

    if (al::isMsgFloorTouch(message)) {
        if (al::isNerve(this, &NrvShineTowerBackDoor.ReactionWait)) {
            al::setNerve(this, &NrvShineTowerBackDoor.ReactionWait);
            return true;
        }

        if (al::isNerve(this, &NrvShineTowerBackDoor.Wait)) {
            al::setNerve(this, &NrvShineTowerBackDoor.Reaction);
            return true;
        }
    }

    return false;
}

void ShineTowerBackDoor::attachToHostJoint(const al::LiveActor* actor, const char* jointName) {
    al::attachMtxConnectorToJoint(mMtxConnector, actor, jointName);
    al::resetMtxPosition(this, *al::getJointMtxPtr(actor, jointName));
}

bool ShineTowerBackDoor::isEnter() const {
    return al::isNerve(this, &NrvShineTowerBackDoor.Enter);
}

void ShineTowerBackDoor::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void ShineTowerBackDoor::exeEnter() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "ReactionEnter");
        al::startHitReaction(this, "入る");
    }

    rs::setPuppetVelocity(mPlayerPuppet, sead::Vector3f::ex * -45.0f);
    sead::Vector3f puppetTrans = rs::getPuppetTrans(mPlayerPuppet);
    sead::Vector3f trans = al::getTrans(this);
    trans.y = puppetTrans.y;

    f32 bottom = al::getTrans(this).y - 300.0f;
    if (puppetTrans.y < bottom) {
        trans.y = bottom;
        puppetTrans.y = bottom;
        rs::setPuppetVelocity(mPlayerPuppet, sead::Vector3f::zero);
        rs::hidePuppet(mPlayerPuppet);
    }

    al::lerpVec(&puppetTrans, puppetTrans, trans, 0.5f);
    rs::setPuppetTrans(mPlayerPuppet, puppetTrans);

    if (al::isGreaterEqualStep(this, 75)) {
        GameDataHolder* gameDataHolder = GameDataFunction::getGameDataHolder(this);
        ChangeStageInfo changeStageInfo = ChangeStageInfo(gameDataHolder, "HomeBackDoor",
                                                          GameDataFunction::getHomeShipStageName());
        GameDataFunction::tryChangeNextStage(this, &changeStageInfo);
    }
}

void ShineTowerBackDoor::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "ReactionLand");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvShineTowerBackDoor.ReactionWait);
}

void ShineTowerBackDoor::exeReactionWait() {
    if (al::isGreaterEqualStep(this, 3))
        al::setNerve(this, &NrvShineTowerBackDoor.Wait);
}
