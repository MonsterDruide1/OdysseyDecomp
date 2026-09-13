#include "MapObj/GrowFlowerPotStateReactionWater.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/GrowFlowerPot.h"

namespace {
NERVE_IMPL(GrowFlowerPotStateReactionWater, Start);
NERVE_IMPL(GrowFlowerPotStateReactionWater, Reaction);
NERVE_IMPL(GrowFlowerPotStateReactionWater, ReactionGrow);
NERVE_IMPL(GrowFlowerPotStateReactionWater, End);

NERVES_MAKE_NOSTRUCT(GrowFlowerPotStateReactionWater, Start, Reaction, ReactionGrow, End);
}  // namespace

GrowFlowerPotStateReactionWater::GrowFlowerPotStateReactionWater(GrowFlowerPot* pot)
    : al::HostStateBase<GrowFlowerPot>("ホウスイの水との反応ステート", pot) {
    initNerve(&Start, 0);
}

void GrowFlowerPotStateReactionWater::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Start);
}

void GrowFlowerPotStateReactionWater::control() {
    mReactionTimer--;
}

void GrowFlowerPotStateReactionWater::exeStart() {
    if (al::isFirstStep(this)) {
        al::startAction(getHost(), "ReactionWaterStart");
        getHost()->tryStartActionReactionParts();
    }

    getHost()->setIsReactionWater(true);
    if (al::isActionEnd(getHost())) {
        getHost()->endPartsReactionTrans();
        if (getHost()->isGrowAlready())
            al::setNerve(this, &Reaction);
        else
            al::setNerve(this, &ReactionGrow);
    }
}

void GrowFlowerPotStateReactionWater::exeReaction() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(getHost(), "ReactionWater");

    getHost()->setIsReactionWater(true);
    getHost()->tryStartActionReactionParts();
    if (mReactionTimer <= 0) {
        getHost()->endPartsReactionTrans();
        al::setNerve(this, &End);
    }
}

void GrowFlowerPotStateReactionWater::exeReactionGrow() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(getHost(), "ReactionWater");
        getHost()->startGrow();
    }

    if (mReactionTimer >= 1) {
        if (!getHost()->isGrowAlready()) {
            getHost()->growLevelFromHosui();
            getHost()->calcGrowLevelTarget();
        }
    }

    if (mReactionTimer <= 0 && !getHost()->isEnableGrow()) {
        al::setNerve(this, &End);
        return;
    }

    if (!getHost()->updateGrow())
        return;

    getHost()->endGrow();
    if (mReactionTimer >= 1 && getHost()->isGrowAlready()) {
        al::setNerve(this, &Reaction);
        return;
    }

    if (getHost()->isEnableGrow()) {
        getHost()->tryStartActionWaitParts();
        al::setNerve(this, &Start);
        al::setNerve(this, &ReactionGrow);
        return;
    }

    al::setNerve(this, &End);
}

void GrowFlowerPotStateReactionWater::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(getHost(), "ReactionWaterEnd");

    getHost()->setIsReactionWater(true);
    if (al::isActionEnd(getHost())) {
        getHost()->endPartsReactionTrans();
        kill();
    }
}

void GrowFlowerPotStateReactionWater::requestReaction() {
    mReactionTimer = 10;
}
