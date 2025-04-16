#include "System/GameDataFunction.h"

#include "System/GameDataFile.h"
#include "System/GameDataHolder.h"

namespace GameDataFunction {
bool isGameClear(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isGameClear();
}
}  // namespace GameDataFunction
