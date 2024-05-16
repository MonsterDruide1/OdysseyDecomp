#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
// bool isAlive(LiveActor* actor);
void initActorSceneInfo(al::LiveActor* actor, al::ActorInitInfo const& info);
void initStageSwitch(al::LiveActor* actor, al::ActorInitInfo const& info);
void initExecutorWatchObj(al::LiveActor* actor, al::ActorInitInfo const& info);
}  // namespace al
