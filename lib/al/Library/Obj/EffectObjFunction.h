#pragma once

namespace al {
class LiveActor;
struct ActorInitInfo;

namespace EffectObjFunction {
void initActorEffectObj(LiveActor* actor, const ActorInitInfo& info);
void initActorEffectObj(LiveActor* actor, const ActorInitInfo& info, const char* archiveName);
void initActorEffectObjNoArchive(LiveActor* actor, const ActorInitInfo& info);
void initActorEffectObjNoArchive(LiveActor* actor, const ActorInitInfo& info,
                                 const char* effectKeeperName);
}  // namespace EffectObjFunction
}  // namespace al
