#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class EventFlowExecutor;
class LiveActor;
}  // namespace al

struct NpcEventStateScareActionParam {
    NpcEventStateScareActionParam(const char*);
    NpcEventStateScareActionParam(const char*, const char*, const char*);

    const char* _0;
    const char* _8;
    const char* _10;
};

class NpcEventStateScare : public al::ActorStateBase {
public:
    NpcEventStateScare(al::LiveActor*, const NpcEventStateScareActionParam*);
    void setActionParam(const NpcEventStateScareActionParam*);
    bool tryStart(const al::EventFlowExecutor*);
    void kill() override;
    void exeScareStart();
    void exeScare();
    void exeScareEnd();

private:
    al::EventFlowExecutor* _20;
    NpcEventStateScareActionParam* _28;
};

static_assert(sizeof(NpcEventStateScareActionParam) == 0x18);
static_assert(sizeof(NpcEventStateScare) == 0x30);
