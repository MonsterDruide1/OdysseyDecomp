#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class EventFlowExecutor;
}  // namespace al

class TalkPoint : public al::LiveActor {
public:
    TalkPoint(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void exeWait();

private:
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
};

static_assert(sizeof(TalkPoint) == 0x110);
