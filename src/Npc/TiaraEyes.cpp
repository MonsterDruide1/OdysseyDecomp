#include "Npc/TiaraEyes.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(TiaraEyes, Appear);
NERVE_IMPL_(TiaraEyes, WaitPanicL, Controlled);
NERVE_IMPL_(TiaraEyes, WaitPanicR, Controlled);
NERVE_IMPL_(TiaraEyes, MovePanicL, Controlled);
NERVE_IMPL_(TiaraEyes, MovePanicR, Controlled);
NERVE_IMPL_(TiaraEyes, Reaction, Controlled);
NERVE_IMPL_(TiaraEyes, ReactionCap, Controlled);
NERVE_IMPL(TiaraEyes, Wait);
NERVE_IMPL(TiaraEyes, Disappear);
NERVE_END_IMPL(TiaraEyes, Hide);

NERVES_MAKE_NOSTRUCT(TiaraEyes, Appear, WaitPanicL, WaitPanicR, MovePanicL, MovePanicR, Reaction,
                     ReactionCap, Wait, Disappear, Hide);
}  // namespace

TiaraEyes::TiaraEyes(const char* name) : al::PartsModel(name) {}

void TiaraEyes::init(const al::ActorInitInfo& info) {
    al::initNerve(this, &Appear, 0);
    makeActorDead();
}

void TiaraEyes::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &Appear);
}

void TiaraEyes::startWaitPanicL(f32 frame) {
    if (al::isDead(this))
        appear();

    al::startAction(this, "WaitPanicL");
    al::setActionFrame(this, sead::Mathf::min(al::getActionFrameMax(this, "WaitPanicL"), frame));
    al::setNerve(this, &WaitPanicL);
}

void TiaraEyes::startWaitPanicR(f32 frame) {
    if (al::isDead(this))
        appear();

    al::startAction(this, "WaitPanicR");
    al::setActionFrame(this, sead::Mathf::min(al::getActionFrameMax(this, "WaitPanicR"), frame));
    al::setNerve(this, &WaitPanicR);
}

void TiaraEyes::startMovePanicL(f32 frame) {
    if (al::isDead(this))
        appear();

    al::startAction(this, "MovePanicL");
    al::setActionFrame(this, sead::Mathf::min(al::getActionFrameMax(this, "MovePanicL"), frame));
    al::setNerve(this, &MovePanicL);
}

void TiaraEyes::startMovePanicR(f32 frame) {
    if (al::isDead(this))
        appear();

    al::startAction(this, "MovePanicR");
    al::setActionFrame(this, sead::Mathf::min(al::getActionFrameMax(this, "MovePanicR"), frame));
    al::setNerve(this, &MovePanicR);
}

void TiaraEyes::startReaction() {
    if (al::isDead(this))
        appear();

    al::startAction(this, "Reaction");
    al::setActionFrame(this, sead::Mathf::min(al::getActionFrameMax(this, "Reaction"), 0.0f));
    al::setNerve(this, &Reaction);
}

void TiaraEyes::startReactionCap() {
    if (al::isDead(this))
        appear();

    al::startAction(this, "ReactionCap");
    al::setActionFrame(this, sead::Mathf::min(al::getActionFrameMax(this, "ReactionCap"), 0.0f));
    al::setNerve(this, &ReactionCap);
}

void TiaraEyes::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");

    al::setNerveAtActionEnd(this, &Wait);
}

void TiaraEyes::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    if (!mIsActive)
        al::setNerve(this, &Disappear);
}

void TiaraEyes::exeDisappear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Disappear");

    if (al::isActionEnd(this)) {
        al::hideModelIfShow(this);
        al::setNerve(this, &Hide);
    }
}

void TiaraEyes::exeHide() {
    if (al::isFirstStep(this))
        mHideTimer = 0;

    mHideTimer += mIsActive ? 1 : -mHideTimer;

    if (mHideTimer >= 60)
        al::setNerve(this, &Appear);
}

void TiaraEyes::endHide() {
    al::showModelIfHide(this);
}

void TiaraEyes::exeControlled() {}
