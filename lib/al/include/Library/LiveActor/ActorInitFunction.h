#pragma once

namespace al {
class LiveActor;

LiveActor* createLinksActorFromFactory(const ActorInitInfo& info, const char* linkName,
                                       s32 linkNum);
bool trySyncStageSwitchAppear(LiveActor* actor);
}  // namespace al
