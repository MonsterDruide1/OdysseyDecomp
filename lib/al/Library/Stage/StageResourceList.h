#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/Scene/ISceneObj.h"

namespace al {

class StageSyncCounter : public IUseExecutor, public ISceneObj {
public:
    StageSyncCounter();

    void execute() override;
    const char* getSceneObjName() const override;
    void initAfterPlacementSceneObj(const ActorInitInfo& initInfo) override;

    s32 getCounter() const { return mCounter; }

private:
    s32 mCounter = 0;
};

}  // namespace al
