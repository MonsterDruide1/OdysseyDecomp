#pragma once

#include <basis/seadTypes.h>

namespace al {
class StageResourceList;

class StageResourceKeeper {
public:
    StageResourceKeeper();

    void initAndLoadResource(const char* stageName, s32 scenarioNo);

    enum class ResourceType : s32 { Map, Stage, Sound };

    StageResourceList* getStageResourceList(s32 resourceType) const {
        return mResourceList[resourceType];
    }

    StageResourceList* getMapStageInfo() const { return mResourceList[(s32)ResourceType::Map]; }

    StageResourceList* getDesignStageInfo() const {
        return mResourceList[(s32)ResourceType::Stage];
    }

    StageResourceList* getSoundStageInfo() const { return mResourceList[(s32)ResourceType::Sound]; }

private:
    StageResourceList** mResourceList = nullptr;
    const char* mStageName = nullptr;
    s32 mScenarioNo = -1;
};

}  // namespace al
