#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class Scene;
}
class StageSceneStateSkipDemo;

class StageSceneStateCarryMeat : public al::NerveStateBase {
public:
    StageSceneStateCarryMeat(const char* name, al::Scene* scene);
    void setState(StageSceneStateSkipDemo* skipDemo);
    void appear() override;
    void kill() override;
    void exeFindMeat();
    void exeCarryMeat();
    void exeSkipDemoFindMeat();
    void exeSkipDemoCarryMeat();

private:
    al::Scene* mScene = nullptr;
    StageSceneStateSkipDemo* mSkipDemo = nullptr;
};

static_assert(sizeof(StageSceneStateCarryMeat) == 0x28);
