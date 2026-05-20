#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
template <class T>
class DeriveActorGroup;
class Scene;
}  // namespace al

class CageShineWatcher;

class StageSceneStateBreakCageShine : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateBreakCageShine(const char* name, al::Scene* scene,
                                  al::DeriveActorGroup<CageShineWatcher>* cageShineWatchers);

    void appear() override;
    void kill() override;

    void exeWait();

    static StageSceneStateBreakCageShine* tryCreate(al::Scene* scene,
                                                    const al::ActorInitInfo& initInfo);

private:
    al::DeriveActorGroup<CageShineWatcher>* mCageShineWatchers = nullptr;
};

static_assert(sizeof(StageSceneStateBreakCageShine) == 0x28);
