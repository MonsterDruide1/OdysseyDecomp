#pragma once

namespace al {
class LiveActor;
struct ActorInitInfo;

void registSupportFreezeSyncGroup(LiveActor* actor, const ActorInitInfo& info);
}  // namespace al
