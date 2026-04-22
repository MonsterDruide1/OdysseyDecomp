#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IUseEventFlowData.h"
#include "Library/Nerve/IUseNerve.h"

namespace al {
struct ActorInitInfo;
class EventFlowDataHolder;
class LiveActor;
class Nerve;
class NerveKeeper;

class EventFlowMovement : public IUseEventFlowData, public IUseNerve {
public:
    EventFlowMovement(const char* name, LiveActor* actor);

    void movement();
    void initNerve(const Nerve* nerve, s32 stateCount);

    virtual void init(const ActorInitInfo& info) = 0;

    virtual void kill() {}

    virtual void appear() {}

    virtual void control() {}

    NerveKeeper* getNerveKeeper() const override { return mNerveKeeper; }

    virtual bool isTurnMovement() const { return false; }

    virtual bool isWaitAtPointMovement() const { return false; }

    EventFlowDataHolder* getEventFlowDataHolder() const override { return mEventFlowDataHolder; }

protected:
    const char* mName = nullptr;
    LiveActor* mActor = nullptr;
    NerveKeeper* mNerveKeeper = nullptr;
    EventFlowDataHolder* mEventFlowDataHolder = nullptr;
};

static_assert(sizeof(EventFlowMovement) == 0x30);
}  // namespace al
