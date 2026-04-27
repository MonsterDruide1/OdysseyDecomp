#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class EventFlowEventData;
class EventFlowExecutor;
class LiveActor;
}  // namespace al

class JangoCapStateTalkDemo : public al::ActorStateBase, public al::IEventFlowEventReceiver {
public:
    JangoCapStateTalkDemo(al::LiveActor* actor, const al::ActorInitInfo& initInfo);

    void init() override;
    void appear() override;
    bool receiveEvent(const al::EventFlowEventData* event) override;

    void exeRequestDemo();
    void exeWipeOut();
    void exeWipeWait();
    void exeWipeIn();

private:
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    const al::ActorInitInfo& mInitInfo;
    sead::Vector3f mDemoPlayerTrans = sead::Vector3f::zero;
    sead::Quatf mDemoPlayerQuat = sead::Quatf::unit;
};

static_assert(sizeof(JangoCapStateTalkDemo) == 0x58);
