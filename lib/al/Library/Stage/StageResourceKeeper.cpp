#include "Library/Stage/StageResourceKeeper.h"

#include "Library/Stage/StageResourceList.h"

namespace al {

StageResourceKeeper::StageResourceKeeper() = default;

void StageResourceKeeper::initAndLoadResource(const char* stageName, s32 scenarioNo) {
    mStageName = stageName;
    mScenarioNo = scenarioNo;
    mResourceList = new StageResourceList*[3];
    mResourceList[0] = new StageResourceList(stageName, scenarioNo, "Map");
    mResourceList[1] = new StageResourceList(stageName, scenarioNo, "Design");
    mResourceList[2] = new StageResourceList(stageName, scenarioNo, "Sound");
}

}  // namespace al
