#include "Util/MoonRockUtil.h"

#include "Library/LiveActor/LiveActor.h"

#include "System/GameDataFile.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolder.h"
#include "System/GameDataHolderAccessor.h"
#include "System/MoonRockData.h"

namespace rs {
bool isEnableShowDemoAfterOpenMoonRockFirst(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor(actor);
    return accessor->getGameDataFile()->getMoonRockData()->isEnableShowDemoAfterOpenMoonRockFirst();
}

bool isFirstDemoOpenMoonRock(const al::LiveActor* actor) {
    return isEnableShowDemoAfterOpenMoonRockFirst(actor);
}

bool isEnableShowDemoMoonRockMapWorld(const al::LiveActor* actor) {
    MoonRockData* moonRockData =
        GameDataHolderAccessor(actor)->getGameDataFile()->getMoonRockData();
    return moonRockData->isEnableShowDemoMoonRockMapWorld(
        GameDataFunction::getCurrentWorldIdNoDevelop(actor));
}

void showDemoAfterOpenMoonRockFirst(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor(actor);
    accessor->getGameDataFile()->getMoonRockData()->showDemoAfterOpenMoonRockFirst();
}

void showDemoMoonRockMapWorld(const al::LiveActor* actor) {
    MoonRockData* moonRockData =
        GameDataHolderAccessor(actor)->getGameDataFile()->getMoonRockData();
    moonRockData->showDemoMoonRockMapWorld(GameDataFunction::getCurrentWorldIdNoDevelop(actor));
}
}  // namespace rs
