#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {

class StageSyncCounter : public IUseExecutor, public ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_alStageSyncCounter;

    StageSyncCounter();

    void execute() override { mCounter++; }

    const char* getSceneObjName() const override { return "ステージ同期カウンタ"; }

    void initAfterPlacementSceneObj(const ActorInitInfo& info) override;

    s32 getCounter() const { return mCounter; }

private:
    s32 mCounter = 0;
};

}  // namespace al
