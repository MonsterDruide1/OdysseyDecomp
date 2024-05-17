#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
// bool isAlive(LiveActor const* actor);
void initActorSceneInfo(LiveActor* actor, ActorInitInfo const& info);
void initStageSwitch(LiveActor* actor, ActorInitInfo const& info);
void initExecutorWatchObj(LiveActor* actor, ActorInitInfo const& info);
}  // namespace al
