#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class Scene;
}  // namespace al

class StageScene;
class StageSceneLayout;

class StageSceneStateRecoverLife : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateRecoverLife(const char* name, StageScene* stageScene, StageSceneLayout* layout);

    void appear() override;
    void kill() override;

    void exeDemoLifeRecover();

private:
    StageSceneLayout* mLayout = nullptr;
};

static_assert(sizeof(StageSceneStateRecoverLife) == 0x28);
