#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class SessionMusicianBgmController : public al::NerveExecutor {
public:
    SessionMusicianBgmController(al::LiveActor* actor, const al::ActorInitInfo& info,
                                 bool isIndoor);

    void exeWait();
    bool tryStartBgmSituation();
    void exeInArea();
    void exeOutArea();

private:
    u8 _10[0x30];
};

static_assert(sizeof(SessionMusicianBgmController) == 0x40);
