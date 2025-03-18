#pragma once

#include "System/GameDataHolderAccessor.h"

namespace al {
class IUseSceneObjHolder;
class SceneObjHolder;
}  // namespace al

class GameDataHolderWriter : public GameDataHolderAccessor {
public:
    GameDataHolderWriter(const al::IUseSceneObjHolder* holder) : GameDataHolderAccessor(holder) {}

    GameDataHolderWriter(const al::SceneObjHolder* holder) : GameDataHolderAccessor(holder) {}

    GameDataHolderWriter(GameDataHolder* holder) : GameDataHolderAccessor(holder) {}
};
