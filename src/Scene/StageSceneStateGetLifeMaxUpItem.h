#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class Scene;
}
class StageSceneLayout;

class StageSceneStateGetLifeMaxUpItem : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateGetLifeMaxUpItem(const char* name, al::Scene* scene,
                                    StageSceneLayout* stageSceneLayout);
    void appear() override;
    void kill() override;
    void exeDemoGetLifeMaxUpItem();

private:
    StageSceneLayout* mLayout = nullptr;
};
