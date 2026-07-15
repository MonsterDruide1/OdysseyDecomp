#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveExecutor.h"

#include "Npc/SessionMusicianBgmController.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class SessionBgmCtrlObj : public al::LiveActor {
public:
    SessionBgmCtrlObj(const char* name);

    void init(const al::ActorInitInfo& initInfo);
    void control();

private:
    SessionMusicianBgmController* mBgmController = nullptr;
};

static_assert(sizeof(SessionBgmCtrlObj) == 0x110);
