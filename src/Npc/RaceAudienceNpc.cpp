#include "Npc/RaceAudienceNpc.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Math/MathRandomUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
NERVE_IMPL(RaceAudienceNpc, Wait);
NERVE_IMPL(RaceAudienceNpc, Dance);
NERVE_IMPL(RaceAudienceNpc, Jump);
NERVE_IMPL(RaceAudienceNpc, DanceRandom);

NERVES_MAKE_STRUCT(RaceAudienceNpc, Wait, Dance, Jump, DanceRandom);
}  // namespace

RaceAudienceNpc::RaceAudienceNpc(const char* name) : al::LiveActor(name) {}

void RaceAudienceNpc::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::getArg((s32*)&mAudienceActionType, info, "AudienceActionType");
    switch (mAudienceActionType) {
    case ActionType::Wait:
        al::initNerve(this, &NrvRaceAudienceNpc.Wait, 0);
        break;
    case ActionType::Dance:
        al::initNerve(this, &NrvRaceAudienceNpc.Dance, 0);
        break;
    case ActionType::Jump:
        al::initNerve(this, &NrvRaceAudienceNpc.Jump, 0);
        break;
    }
    makeActorAlive();
}

void RaceAudienceNpc::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "WaitHappy");
}

void RaceAudienceNpc::exeDance() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Excited");
        mDanceTimer = al::getRandom(600) + 600;
    }
    if (al::isGreaterEqualStep(this, mDanceTimer)) {
        if (mAudienceActionType == ActionType::Jump)
            al::setNerve(this, &NrvRaceAudienceNpc.Jump);
        else
            al::setNerve(this, &NrvRaceAudienceNpc.DanceRandom);
    }
}

void RaceAudienceNpc::exeDanceRandom() {
    if (al::isFirstStep(this))
        al::startAction(this, "ExcitedRandom");
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvRaceAudienceNpc.Dance);
}

void RaceAudienceNpc::exeJump() {
    if (al::isFirstStep(this))
        al::startAction(this, "ExcitedJump");
}
