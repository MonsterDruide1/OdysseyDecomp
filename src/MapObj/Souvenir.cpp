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
void Souvenir::init(const al::ActorInitInfo& actorInitInfo) {
    const char* suffix = nullptr;
    al::tryGetStringArg(&suffix, actorInitInfo, "Suffix");
    al::initMapPartsActor(this, actorInitInfo, suffix);
    al::tryGetArg(&mIsReactionPlayerUpperPunch, actorInitInfo, "IsReactionPlayerUpperPunch");
    al::tryGetArg(&mIsThroughCapAttack, actorInitInfo, "IsThroughCapAttack");
    al::tryGetArg(&mRotateYSpeed, actorInitInfo, "RotateYSpeed");
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

bool Souvenir::receiveMsg(const al::SensorMsg* msg, al::HitSensor* self, al::HitSensor* other) {
    if (al::isMsgPlayerDisregard(msg) || rs::isMsgPlayerDisregardHomingAttack(msg) ||
        rs::isMsgPlayerDisregardTargetMarker(msg)) {
        return true;
    }

    if (rs::isMsgCapTouchWall(msg) || rs::isMsgCapAttack(msg) || al::isMsgPlayerTrample(msg) ||
        rs::isMsgPlayerAndCapHipDropAll(msg) || rs::isMsgPlayerAndCapObjHipDropAll(msg) ||
        (al::isMsgPlayerObjUpperPunch(msg) && mIsReactionPlayerUpperPunch) ||
        al::isMsgPlayerRollingAttack(msg) || al::isMsgPlayerObjRollingAttack(msg)) {
        if (!al::isExistAction(this, "ReactionCap"))
            return false;

        const sead::Vector3f& velocity = al::getVelocity(al::getSensorHost(self));
        if (!rs::isMsgCapAttack(msg) && velocity.length() < 5.0f)
            return false;

        if (!al::isNerve(this, &NrvSouvenir.Wait) &&
            (!al::isNerve(this, &NrvSouvenir.ReactionCap) || !al::isGreaterStep(this, 10)))
            return false;

        al::setNerve(this, &NrvSouvenir.ReactionCap);
        if (mIsThroughCapAttack &&
            (rs::isMsgCapAttack(msg) || al::isMsgPlayerObjUpperPunch(msg) ||
             al::isMsgPlayerTrample(msg) || rs::isMsgPlayerAndCapHipDropAll(msg) ||
             rs::isMsgPlayerAndCapObjHipDropAll(msg)))
            return false;

        if (!rs::isMsgPlayerAndCapHipDropAll(msg) && !rs::isMsgPlayerAndCapObjHipDropAll(msg))
            al::startHitReactionHitEffect(this, "ヒット[小]", self, other);
        else
            al::startHitReactionHitEffect(this, "ヒット", self, other);
        return true;
    }

    return false;
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
            getNextAction() != nullptr) {
            return;
        }

        if (mIsWait) {
            al::tryStartAction(this, "Wait");
            return;
        }

        if (al::isExistAction(this, "WaitShop")) {
            al::startActionAtRandomFrame(this, "WaitShop");
            return;
        }
        if (al::isExistAction(this, "Shop")) {
            al::startActionAtRandomFrame(this, "Wait");
            return;
        }
    }
}

inline const char* Souvenir::getNextAction() {
    if (al::isActionPlaying(this, "Wait")) {
        if (!al::isExistAction(this, "ReactionCap"))
            return nullptr;

        return "ReactionCap";
    } else if (al::isActionPlaying(this, "ReactionCap")) {
        if (!al::isExistAction(this, "ReactionCap1"))
            return nullptr;

        return "ReactionCap1";
    } else if (al::isActionPlaying(this, "ReactionCap1")) {
        if (!al::isExistAction(this, "ReactionCap2"))
            return nullptr;

        return "ReactionCap2";
    } else if (al::isActionPlaying(this, "ReactionCap2")) {
        if (!al::isExistAction(this, "ReactionCap3"))
            return nullptr;

        return "ReactionCap3";
    } else if (al::isActionPlaying(this, "ReactionCap3")) {
        if (!al::isExistAction(this, "ReactionCap4"))
            return nullptr;

        return "ReactionCap4";
    } else if (al::isActionPlaying(this, "ReactionCap4"))
        return nullptr;

    if (!al::isExistAction(this, "ReactionCap"))
        return nullptr;

    return "ReactionCap";
}

void Souvenir::exeReactionCap() {
    if (al::isFirstStep(this))
        al::startAction(this, getNextAction() ?: "ReactionCap");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvSouvenir.Wait);
}
