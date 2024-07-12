#pragma once

namespace al {
class ActorInitInfo;
class LiveActor;

void registerAreaHostMtx(LiveActor* actor, const ActorInitInfo& info);
}  // namespace al
