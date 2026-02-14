#include "MapObj/Souvenir.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Souvenir, Wait);
NERVE_IMPL(Souvenir, ReactionCap);

NERVES_MAKE_STRUCT(Souvenir, Wait, ReactionCap);
}  // namespace

Souvenir::Souvenir(const char* name) : al::LiveActor(name) {}

// Non-matching: regswap (https://decomp.me/scratch/8qJEm)
void Souvenir::init(const al::ActorInitInfo& info) {
    const char* suffix = nullptr;
    al::tryGetStringArg(&suffix, info, "Suffix");
    al::initMapPartsActor(this, info, suffix);
    al::tryGetArg(&mIsReactionPlayerUpperPunch, info, "IsReactionPlayerUpperPunch");
    al::tryGetArg(&mIsThroughCapAttack, info, "IsThroughCapAttack");
    al::tryGetArg(&mRotateYSpeed, info, "RotateYSpeed");
    al::initNerve(this, &NrvSouvenir.Wait, 0);
    makeActorAlive();
}

void Souvenir::control() {
    if (mRotateYSpeed != 0.0f) {
        sead::Quatf* quat = al::getQuatPtr(this);
        al::rotateQuatYDirDegree(quat, *quat, mRotateYSpeed);
    }
}

void Souvenir::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!mIsThroughCapAttack && al::isSensorMapObj(self))
        rs::sendMsgPushToPlayer(other, self);
}

bool Souvenir::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message) || rs::isMsgPlayerDisregardHomingAttack(message) ||
        rs::isMsgPlayerDisregardTargetMarker(message))
        return true;

    if (rs::isMsgCapTouchWall(message) || rs::isMsgCapAttack(message) ||
        al::isMsgPlayerTrample(message) || rs::isMsgPlayerAndCapHipDropAll(message) ||
        rs::isMsgPlayerAndCapObjHipDropAll(message) ||
        (al::isMsgPlayerObjUpperPunch(message) && mIsReactionPlayerUpperPunch) ||
        al::isMsgPlayerRollingAttack(message) || al::isMsgPlayerObjRollingAttack(message)) {
        if (!al::isExistAction(this, "ReactionCap"))
            return false;

        const sead::Vector3f& velocity = al::getVelocity(al::getSensorHost(other));
        if (!rs::isMsgCapAttack(message) && velocity.length() < 5.0f)
            return false;

        if (!al::isNerve(this, &NrvSouvenir.Wait) &&
            (!al::isNerve(this, &NrvSouvenir.ReactionCap) || !al::isGreaterStep(this, 10)))
            return false;

        al::setNerve(this, &NrvSouvenir.ReactionCap);
        if (mIsThroughCapAttack &&
            (rs::isMsgCapAttack(message) || al::isMsgPlayerObjUpperPunch(message) ||
             al::isMsgPlayerTrample(message) || rs::isMsgPlayerAndCapHipDropAll(message) ||
             rs::isMsgPlayerAndCapObjHipDropAll(message)))
            return false;

        if (!rs::isMsgPlayerAndCapHipDropAll(message) &&
            !rs::isMsgPlayerAndCapObjHipDropAll(message))
            al::startHitReactionHitEffect(this, "ヒット[小]", other, self);
        else
            al::startHitReactionHitEffect(this, "ヒット", other, self);
        return true;
    }

    return false;
}

static const char* getNextAction(al::LiveActor* actor) {
    if (al::isActionPlaying(actor, "Wait")) {
        if (!al::isExistAction(actor, "ReactionCap"))
            return nullptr;

        return "ReactionCap";
    } else if (al::isActionPlaying(actor, "ReactionCap")) {
        if (!al::isExistAction(actor, "ReactionCap1"))
            return nullptr;

        return "ReactionCap1";
    } else if (al::isActionPlaying(actor, "ReactionCap1")) {
        if (!al::isExistAction(actor, "ReactionCap2"))
            return nullptr;

        return "ReactionCap2";
    } else if (al::isActionPlaying(actor, "ReactionCap2")) {
        if (!al::isExistAction(actor, "ReactionCap3"))
            return nullptr;

        return "ReactionCap3";
    } else if (al::isActionPlaying(actor, "ReactionCap3")) {
        if (!al::isExistAction(actor, "ReactionCap4"))
            return nullptr;

        return "ReactionCap4";
    } else if (al::isActionPlaying(actor, "ReactionCap4"))
        return nullptr;

    if (!al::isExistAction(actor, "ReactionCap"))
        return nullptr;

    return "ReactionCap";
}

void Souvenir::exeWait() {
    if (al::isFirstStep(this)) {
        if (((al::isExistAction(this, "ReactionCap") && al::isActionPlaying(this, "ReactionCap")) ||
             (al::isExistAction(this, "ReactionCap1") &&
              al::isActionPlaying(this, "ReactionCap1")) ||
             (al::isExistAction(this, "ReactionCap2") &&
              al::isActionPlaying(this, "ReactionCap2")) ||
             (al::isExistAction(this, "ReactionCap3") &&
              al::isActionPlaying(this, "ReactionCap3")) ||
             (al::isExistAction(this, "ReactionCap4") &&
              al::isActionPlaying(this, "ReactionCap4"))) &&
            getNextAction(this))
            return;

        if (mIsWait) {
            al::tryStartAction(this, "Wait");
            return;
        }

        if (al::isExistAction(this, "WaitShop")) {
            al::startActionAtRandomFrame(this, "WaitShop");
            return;
        }
        if (al::isExistAction(this, "Wait")) {
            al::startActionAtRandomFrame(this, "Wait");
            return;
        }
    }
}

void Souvenir::exeReactionCap() {
    if (al::isFirstStep(this))
        al::startAction(this, getNextAction(this) ?: "ReactionCap");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvSouvenir.Wait);
}
