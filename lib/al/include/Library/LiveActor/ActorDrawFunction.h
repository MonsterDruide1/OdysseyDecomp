#pragma once

namespace al {
class LiveActor;

bool isAlive(const LiveActor* actor);
void initActorSceneInfo(LiveActor* actor, const ActorInitInfo& info);
void initStageSwitch(LiveActor* actor, const ActorInitInfo& info);
void initExecutorWatchObj(LiveActor* actor, const ActorInitInfo& info);
}  // namespace al
