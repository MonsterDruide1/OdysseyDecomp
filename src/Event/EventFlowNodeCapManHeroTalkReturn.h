#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Event/EventFlowNode.h"

namespace al {
class EventFlowNodeInitInfo;
}  // namespace al

class EventFlowNodeCapManHeroTalkReturn : public al::EventFlowNode {
public:
    EventFlowNodeCapManHeroTalkReturn(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;

    void exeTurnToPlayer();
    void exeReturn();
    void exeReactionCapOn();
    void exeEndWait();

private:
    sead::Vector3f mReturnStartTrans = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mDemoStartTrans = {0.0f, 0.0f, 0.0f};
    sead::Quatf mDemoStartQuat;
    s32 mEaseType = 2;
};

static_assert(sizeof(EventFlowNodeCapManHeroTalkReturn) == 0x98);
