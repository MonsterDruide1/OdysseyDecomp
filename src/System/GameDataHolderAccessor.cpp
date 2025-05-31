#include "System/GameDataHolderAccessor.h"

#include "Library/Scene/SceneObjHolder.h"
#include "Library/Scene/SceneObjUtil.h"

#include "System/GameDataHolder.h"

GameDataHolderAccessor::GameDataHolderAccessor(const al::IUseSceneObjHolder* holder) {
    mData = al::getSceneObj<GameDataHolder>(holder);
}

GameDataHolderAccessor::GameDataHolderAccessor(const al::SceneObjHolder* holder) {
    mData = holder->getObj<GameDataHolder>();
}
