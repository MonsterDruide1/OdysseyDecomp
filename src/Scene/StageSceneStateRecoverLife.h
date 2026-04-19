#pragma once

#include "Library/Nerve/NerveStateBase.h"

class StageScene;
class StageSceneLayout;

class StageSceneStateRecoverLife : public al::NerveStateBase {
public:
    StageSceneStateRecoverLife(const char*, StageScene*, StageSceneLayout*);

    void appear() override;
    void kill() override;

    void exeDemoLifeRecover();

private:
    StageScene* mStageScene = nullptr;
    StageSceneLayout* mStageSceneLayout = nullptr;
};

static_assert(sizeof(StageSceneStateRecoverLife) == 0x28);
