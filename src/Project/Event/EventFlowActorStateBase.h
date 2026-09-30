#pragma once

#include "Library/Event/IUseEventFlowData.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {
class EventFlowDataHolder;
class EventFlowMovement;
class LiveActor;

class EventFlowActorStateBase : public NerveStateBase, public IUseEventFlowData {
public:
    EventFlowActorStateBase(const char* name, EventFlowMovement* movement);

    LiveActor* getActor() const;
    EventFlowDataHolder* getEventFlowDataHolder() const override;

protected:
    EventFlowMovement* mMovement = nullptr;
};

static_assert(sizeof(EventFlowActorStateBase) == 0x28);
}  // namespace al
