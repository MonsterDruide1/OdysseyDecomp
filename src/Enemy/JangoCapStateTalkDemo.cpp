#include "Enemy/JangoCapStateTalkDemo.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerDemoUtil.h"

namespace {
NERVE_IMPL(JangoCapStateTalkDemo, RequestDemo)
NERVE_IMPL(JangoCapStateTalkDemo, WipeOut)
NERVE_IMPL(JangoCapStateTalkDemo, WipeWait)
NERVE_IMPL(JangoCapStateTalkDemo, WipeIn)

NERVES_MAKE_NOSTRUCT(JangoCapStateTalkDemo, RequestDemo, WipeOut, WipeWait, WipeIn);
}  // namespace

JangoCapStateTalkDemo::JangoCapStateTalkDemo(al::LiveActor* actor,
                                             const al::ActorInitInfo& initInfo)
    : al::ActorStateBase("ジャンゴ帽子会話デモ", actor), mInitInfo(initInfo) {}

void JangoCapStateTalkDemo::init() {
    initNerve(&RequestDemo, 0);
    mEventFlowExecutor = rs::initEventFlow(mActor, mInitInfo, "Jango", nullptr);
    al::initEventReceiver(mEventFlowExecutor, this);
    al::tryGetLinksQT(&mDemoPlayerQuat, &mDemoPlayerTrans, mInitInfo, "TalkDemoPlayerPos");
}

void JangoCapStateTalkDemo::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &RequestDemo);
}

bool JangoCapStateTalkDemo::receiveEvent(const al::EventFlowEventData* event) {
    al::LiveActor* actor = mActor;
    const al::EventFlowEventData* eventData = event;

    if (al::isEventName(eventData, "WipeOutEnd")) {
        rs::replaceDemoPlayer(actor, mDemoPlayerTrans, mDemoPlayerQuat);
        al::setNerve(this, &WipeWait);
        return true;
    }

    if (al::isEventName(eventData, "WipeInEnd")) {
        rs::endEventCutSceneDemo(actor);
        al::tryOnStageSwitch(actor, "CapGetOn");
        kill();
        return true;
    }

    return false;
}

void JangoCapStateTalkDemo::exeRequestDemo() {
    if (rs::tryStartEventCutSceneDemo(mActor)) {
        rs::startEventFlow(mEventFlowExecutor, "WipeOut");
        al::setNerve(this, &WipeOut);
    }
}

void JangoCapStateTalkDemo::exeWipeOut() {
    rs::updateEventFlow(mEventFlowExecutor);
}

void JangoCapStateTalkDemo::exeWipeWait() {
    if (al::isGreaterEqualStep(this, 60)) {
        rs::startEventFlow(mEventFlowExecutor, "WipeIn");
        al::setNerve(this, &WipeIn);
    }
}

void JangoCapStateTalkDemo::exeWipeIn() {
    rs::updateEventFlow(mEventFlowExecutor);
}
