#include "Event/EventFlowNodeCapManHeroTalkReturn.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerDemoUtil.h"

namespace {
NERVE_IMPL(EventFlowNodeCapManHeroTalkReturn, Return);
NERVE_IMPL(EventFlowNodeCapManHeroTalkReturn, TurnToPlayer);
NERVE_IMPL(EventFlowNodeCapManHeroTalkReturn, ReactionCapOn);
NERVE_IMPL(EventFlowNodeCapManHeroTalkReturn, EndWait);
NERVES_MAKE_NOSTRUCT(EventFlowNodeCapManHeroTalkReturn, Return, TurnToPlayer, ReactionCapOn,
                     EndWait);
}  // namespace

EventFlowNodeCapManHeroTalkReturn::EventFlowNodeCapManHeroTalkReturn(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCapManHeroTalkReturn::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    initNerve(&Return, 0);
    mDemoStartTrans = al::getTrans(getActor());
    sead::Vector3f front;
    al::calcFrontDir(&front, getActor());
    al::makeQuatUpFront(&mDemoStartQuat, sead::Vector3f::ey, front);
}

void EventFlowNodeCapManHeroTalkReturn::start() {
    al::EventFlowNode::start();
    rs::startCloseNpcDemoEventTalkMessage(getActor());
    mReturnStartTrans = al::getTrans(getActor());
    al::setNerve(this, &TurnToPlayer);
}

void EventFlowNodeCapManHeroTalkReturn::exeTurnToPlayer() {
    if (al::isFirstStep(this))
        al::startAction(getActor(), "Turn");

    al::LiveActor* player = al::tryFindNearestPlayerActor(getActor());
    if (al::turnToTarget(getActor(), player, 3.0f))
        al::setNerve(this, &Return);
}

void EventFlowNodeCapManHeroTalkReturn::exeReturn() {
    if (al::isFirstStep(this))
        al::startAction(getActor(), "DemoTalkCapManHeroDisappear");

    f32 frameMax = al::getActionFrameMax(getActor(), "DemoTalkCapManHeroDisappear");
    f32 rate = al::getActionFrame(getActor()) / frameMax;
    if (rate > 1.0f)
        rate = 1.0f;
    else if (rate < 0.0f)
        rate = 0.0f;
    f32 easeRate = al::easeByType(rate, mEaseType);

    al::LiveActor* player = al::tryFindNearestPlayerActor(getActor());
    sead::Vector3f playerTrans = al::getTrans(player);
    sead::Vector3f trans;
    al::lerpVec(&trans, mReturnStartTrans, playerTrans, easeRate);
    al::setTrans(getActor(), trans);

    if (al::isActionEnd(getActor())) {
        al::hideModelIfShow(getActor());
        rs::invalidateWatchTarget(getActor());
        al::setNerve(this, &ReactionCapOn);
    }
}

void EventFlowNodeCapManHeroTalkReturn::exeReactionCapOn() {
    if (al::isFirstStep(this))
        rs::startActionDemoPlayer(getActor(), "ReactionCapOn");

    if (rs::isActionEndDemoPlayer(getActor())) {
        rs::startActionDemoPlayer(getActor(), "Wait");
        al::setNerve(this, &EndWait);
    }
}

void EventFlowNodeCapManHeroTalkReturn::exeEndWait() {
    if (al::isGreaterEqualStep(this, 0))
        end();
}
