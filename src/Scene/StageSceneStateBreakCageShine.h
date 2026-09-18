#pragma once

#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class Scene;
}  // namespace al

class CageShineWatcher;

class StageSceneStateBreakCageShine : public al::HostStateBase<al::Scene> {
public:
    static StageSceneStateBreakCageShine* tryCreate(al::Scene* scene,
                                                    const al::ActorInitInfo& actorInitInfo);

    StageSceneStateBreakCageShine(const char* name, al::Scene* scene,
                                  al::DeriveActorGroup<CageShineWatcher>* cageShineWatcherGroup);
    void appear() override;
    void kill() override;
    void exeWait();

private:
    al::DeriveActorGroup<CageShineWatcher>* mCageShineWatcherGroup = nullptr;
};

static_assert(sizeof(StageSceneStateBreakCageShine) == 0x28);
