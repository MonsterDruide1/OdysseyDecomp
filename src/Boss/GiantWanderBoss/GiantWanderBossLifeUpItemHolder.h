#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class GiantWanderBossLifeUpItemHolder {
public:
    GiantWanderBossLifeUpItemHolder(const al::ActorInitInfo& initInfo);

    void tryAppearItem(const al::LiveActor* actor);

private:
    void* mActorGroup = nullptr;
};

static_assert(sizeof(GiantWanderBossLifeUpItemHolder) == 0x8);
