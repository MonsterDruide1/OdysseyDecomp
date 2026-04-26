#include "MapObj/TalkPoint.h"

#include <math/seadVector.h>

#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/NpcEventFlowUtil.h"

namespace {
NERVE_IMPL(TalkPoint, Wait);

NERVES_MAKE_NOSTRUCT(TalkPoint, Wait);
}  // namespace

TalkPoint::TalkPoint(const char* name) : al::LiveActor(name) {}

void TalkPoint::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::initExecutorWatchObj(this, info);
    al::initStageSwitch(this, info);
    al::initNerve(this, &Wait, 0);
    al::initActorSeKeeper(this, info, "TalkPoint");
    al::setClippingInfo(this, 300.0f, nullptr);

    const char* eventName = al::getStringArg(info, "EventName");
    mEventFlowExecutor = rs::initEventFlow(this, info, "TalkPoint", eventName);
    al::invalidateUiCollisionCheck(mEventFlowExecutor);

    if (rs::isDefinedEventCamera(mEventFlowExecutor, "Default"))
        rs::initEventCameraObject(mEventFlowExecutor, info, "Default");

    sead::Vector3f balloonLocalOffset = {0.0f, 0.0f, 0.0f};
    al::getArg(&balloonLocalOffset.y, info, "BalloonLocalOffsetY");
    al::setBalloonLocalOffset(mEventFlowExecutor, balloonLocalOffset);
    rs::startEventFlow(mEventFlowExecutor, "Init");

    al::trySyncStageSwitchAppearAndKill(this);
}

void TalkPoint::exeWait() {
    rs::updateEventFlow(mEventFlowExecutor);
}
