#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class EventFlowExecutor;
}  // namespace al

class ShineTowerNpc : public al::LiveActor {
public:
    ShineTowerNpc(const char* name);
    void init(const al::ActorInitInfo& actorInitInfo) override;
    void noBalloon();
    void startBalloon();
    void exeWait();
    void exeGoKoopa();
    void exeNoBalloon();

private:
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    s32 mRemainingShineCount = 0;
};

s32 getRemainingShineCount(al::LiveActor* actor);
