#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Project/Event/EventFlowActorStateBase.h"

namespace al {
struct ActorInitInfo;
class EventFlowMovement;

class EventFlowActorStateRail : public EventFlowActorStateBase {
public:
    EventFlowActorStateRail(const char* name, EventFlowMovement* movement);

    void initByPlacementInfo(const ActorInitInfo& info);
    void appear() override;
    void exeWalk();
    void exeTurn();

private:
    sead::Vector3f mRailOffset = sead::Vector3f::zero;
    s32 mRailMoveType = -1;
};

static_assert(sizeof(EventFlowActorStateRail) == 0x38);
}  // namespace al
