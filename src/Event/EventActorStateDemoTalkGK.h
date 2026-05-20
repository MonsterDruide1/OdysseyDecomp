#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

struct ParamEventActorStateDemoTalkGK {
    constexpr ParamEventActorStateDemoTalkGK() = default;

    constexpr ParamEventActorStateDemoTalkGK(f32 blendStep, s32 easeType)
        : blendStep(blendStep), easeType(easeType) {}

    f32 blendStep = 0.125f;
    s32 easeType = 0;
};

static_assert(sizeof(ParamEventActorStateDemoTalkGK) == 0x8);

class EventActorStateDemoTalkGK : public al::ActorStateBase {
public:
    EventActorStateDemoTalkGK(al::LiveActor* actor, const ParamEventActorStateDemoTalkGK* param);
    void appear() override;
    void exeWait();
    void chaseWaitAnimWeightOne();
    void exeTalk();
    void chaseTalkAnimWeightOne();
    void exeTalkAfter();

private:
    const char* mActionName = "Talk";
    bool mIsWaitActionEnd = false;
    f32 mBlendWeight = 0.0f;
    const ParamEventActorStateDemoTalkGK* mParam = nullptr;
};

static_assert(sizeof(EventActorStateDemoTalkGK) == 0x38);
