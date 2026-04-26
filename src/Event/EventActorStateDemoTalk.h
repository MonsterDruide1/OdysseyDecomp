#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class EventActorStateDemoTalk : public al::ActorStateBase {
public:
    EventActorStateDemoTalk(al::LiveActor* actor);

    void appear() override;
    void exeWait();
    void exeTalk();
    void exeTalkAfter();

private:
    const char* mMessageName;
    bool mIsInitialized = false;
};

static_assert(sizeof(EventActorStateDemoTalk) == 0x30);

class ParamEventActorStateDemoTalkGK {
public:
    ParamEventActorStateDemoTalkGK();
    ParamEventActorStateDemoTalkGK(f32 param1, s32 param2);

    f32 mParam1 = 0.125f;
    s32 mParam2 = 0;
};

static_assert(sizeof(ParamEventActorStateDemoTalkGK) == 0x8);
