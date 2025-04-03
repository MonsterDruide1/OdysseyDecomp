#include "System/GameDataFunction.h"

#include "System/GameDataFile.h"

namespace GameDataFunction {
bool isGameClear(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isGameClear();
}
}  // namespace GameDataFunction
