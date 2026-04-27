#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
class PlacementInfo;
}  // namespace al

class SessionMusicianWarpAgent {
public:
    SessionMusicianWarpAgent(al::LiveActor* actor, const al::ActorInitInfo& info);

    bool tryGetWarpTargetInfo(al::PlacementInfo* placementInfo);
    bool tryStartWarp();

private:
    u8 _0[0x20];
};

static_assert(sizeof(SessionMusicianWarpAgent) == 0x20);
