#include "MapObj/BossKnuckleFix.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(BossKnuckleFix, Wait);
NERVE_IMPL(BossKnuckleFix, ReactionLarge);
NERVE_IMPL(BossKnuckleFix, Reaction);

NERVES_MAKE_STRUCT(BossKnuckleFix, Wait, ReactionLarge, Reaction);
}  // namespace

BossKnuckleFix::BossKnuckleFix(const char* name) : al::LiveActor(name) {}

void BossKnuckleFix::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "BossKnuckleBody", "Fix");
    al::initNerve(this, &NrvBossKnuckleFix.Wait, 0);
    // NOTE: color of embedded grand shine is hardcoded to 5 (Sand)
    al::startMclAnimAndSetFrameAndStop(al::getSubActor(this, "グランドシャイン"), "Color", 5.0f);
    al::trySyncStageSwitchKill(this);
    makeActorAlive();

    mCollisionObj = new al::CollisionObj(info, al::getModelResource(this), "MoveLimit",
                                         al::getHitSensor(this, "Body"), nullptr, nullptr);
    al::setCollisionPartsSpecialPurposeName(mCollisionObj, "MoveLimit");
    al::setTrans(mCollisionObj, al::getTrans(this));
    al::setRotate(mCollisionObj, {0.0f, 90.0f, 0.0f});
    mCollisionObj->makeActorAlive();
}

bool BossKnuckleFix::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(message))
        return true;

    if (al::isNerve(this, &NrvBossKnuckleFix.Wait) &&
        (rs::isMsgSphinxRideAttackTouch(message) || rs::isMsgPlayerAndCapHipDropAll(message))) {
        rs::requestHitReactionToAttacker(message, self, other);

        mReactionCount++;
        if (mReactionCount >= 3) {
            mReactionCount = 0;
            al::setNerve(this, &NrvBossKnuckleFix.ReactionLarge);
        } else
            al::setNerve(this, &NrvBossKnuckleFix.Reaction);

        return !rs::isMsgPlayerAndCapHipDropAll(message);
    }

    return false;
}

void BossKnuckleFix::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "MapWait");
}

void BossKnuckleFix::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "MapReaction");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvBossKnuckleFix.Wait);
}

void BossKnuckleFix::exeReactionLarge() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "MapReactionLarge");
        al::tryOnStageSwitch(this, "ReactionOn");
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvBossKnuckleFix.Wait);
}
