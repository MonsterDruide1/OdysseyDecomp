#pragma once

namespace al {
class LiveActor;
class ActorInitInfo;

class EffectObjFunction {
public:
    static void initActorEffectObj(LiveActor* actor, const ActorInitInfo& info);
    static void initActorEffectObj(LiveActor* actor, const ActorInitInfo& info,
                                   const char* archiveName);
    static void FUN_7100969078(LiveActor* actor, const ActorInitInfo& info);
    static void initActorEffectObjNoArchive(LiveActor* actor, const ActorInitInfo& info,
                                            const char* archiveName);
};
}  // namespace al
