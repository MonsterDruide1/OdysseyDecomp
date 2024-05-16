#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
LiveActor* createLinksActorFromFactory(const ActorInitInfo& info, const char* linkName,
                                       int linkNum);
bool trySyncStageSwitchAppear(LiveActor* actor);
}  // namespace al
