#include "MapObj/PoleClimbParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(PoleClimbParts, Wait);
NERVE_IMPL(PoleClimbParts, Break);
NERVE_IMPL(PoleClimbParts, Reaction);

NERVES_MAKE_STRUCT(PoleClimbParts, Wait, Break, Reaction);
}  // namespace

PoleClimbParts::PoleClimbParts(const char* name) : al::LiveActor(name) {}

void PoleClimbParts::init(const al::ActorInitInfo& info) {
    const char* suffix = nullptr;
    al::tryGetStringArg(&suffix, info, "Suffix");
    al::initMapPartsActor(this, info, suffix);

    if (!al::isExistAction(this, "Reaction")) {
        makeActorDead();
        return;
    }

    mIsBreak = al::isClassName(info, "PoleClimbPartsBreak");
    al::initNerve(this, &NrvPoleClimbParts.Wait, 0);

    if (mIsBreak) {
        if (!al::isExistAction(this, "Break"))
            makeActorDead();

        mBreakJudgeFunction = rs::getBreakJudgeFunction("標準攻撃");
    }

    al::trySyncStageSwitchAppearAndKill(this);
}

bool PoleClimbParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (al::isNerve(this, &NrvPoleClimbParts.Break))
        return false;

    if (rs::isMsgPlayerPoleClimbReaction(message)) {
        if (al::isNerve(this, &NrvPoleClimbParts.Wait))
            al::setNerve(this, &NrvPoleClimbParts.Reaction);
        return true;
    }

    if (mIsBreak && mBreakJudgeFunction(message, other, self)) {
        rs::requestHitReactionToAttacker(message, self, other);
        rs::sendMsgBreakFloorToPlayer(this);
        al::invalidateCollisionParts(this);
        al::setNerve(this, &NrvPoleClimbParts.Break);
        return true;
    }

    return false;
}

void PoleClimbParts::movement() {
    if (al::isNerve(this, &NrvPoleClimbParts.Wait))
        return;

    al::LiveActor::movement();
}

void PoleClimbParts::calcAnim() {
    if (al::isNerve(this, &NrvPoleClimbParts.Wait))
        return;

    al::LiveActor::calcAnim();
}

void PoleClimbParts::exeWait() {}

void PoleClimbParts::exeBreak() {
    if (al::isFirstStep(this)) {
        al::LiveActor* brokenModel = al::tryGetSubActor(this, "壊れモデル");
        if (brokenModel)
            brokenModel->appear();

        al::LiveActor* remainsModel = al::tryGetSubActor(this, "残留モデル");
        if (remainsModel)
            remainsModel->appear();

        if (al::isExistAction(this, "Break")) {
            al::startAction(this, "Break");
            return;
        }
    }

    kill();
}

void PoleClimbParts::exeReaction() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Reaction");
        return;
    }

    if (al::isIntervalStep(this, al::getActionFrameMax(this, "Reaction"), 0)) {
        al::stopAction(this);
        al::setNerve(this, &NrvPoleClimbParts.Wait);
    }
}
