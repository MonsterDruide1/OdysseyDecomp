#include "Event/EventActorMovementRailTraffic.h"

#include "Library/Event/EventFlowActorStateRail.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Npc/TrafficRailWatcherDirector.h"

namespace {
NERVE_IMPL(EventActorMovementRailTraffic, Move);
NERVE_IMPL(EventActorMovementRailTraffic, StopByOtherNpc);
NERVE_IMPL(EventActorMovementRailTraffic, StopAfter);

NERVES_MAKE_NOSTRUCT(EventActorMovementRailTraffic, Move, StopByOtherNpc, StopAfter);
}  // namespace

EventActorMovementRailTraffic::EventActorMovementRailTraffic(const char* name, al::LiveActor* actor)
    : al::EventFlowMovement(name, actor) {}

void EventActorMovementRailTraffic::init(const al::ActorInitInfo& info) {
    initNerve(&Move, 1);

    al::EventFlowActorStateRail* railState = new al::EventFlowActorStateRail("", this);
    railState->initByPlacementInfo(info);
    al::initNerveState(this, railState, &Move, "");
    rs::registerTrafficRailWatcher(mActor, info);
    appear();
}

void EventActorMovementRailTraffic::appear() {
    al::setNerve(this, getNerveKeeper()->getCurrentNerve());
}

void EventActorMovementRailTraffic::exeMove() {
    if (rs::tryStopTrafficRailByOtherNpc(mActor))
        al::setNerve(this, &StopByOtherNpc);
    else
        al::updateNerveState(this);
}

void EventActorMovementRailTraffic::exeStopByOtherNpc() {
    if (al::isFirstStep(this))
        al::tryStartEventActionIfNotPlaying(mActor, this, "Wait");

    if (rs::tryRestartTrafficRailByOtherNpc(mActor))
        al::setNerve(this, &StopAfter);
}

void EventActorMovementRailTraffic::exeStopAfter() {
    al::setNerveAtGreaterEqualStep(this, &Move, 20);
}
