#include "System/GameDataHolderAccessor.h"

#include "Library/Scene/SceneObjHolder.h"
#include "Library/Scene/SceneUtil.h"

GameDataHolderAccessor::GameDataHolderAccessor(const al::IUseSceneObjHolder* holder) {
    mData = reinterpret_cast<GameDataHolder*>(al::getSceneObj(holder, 18));
}

GameDataHolderAccessor::GameDataHolderAccessor(const al::SceneObjHolder* holder) {
    mData = reinterpret_cast<GameDataHolder*>(holder->getObj(18));
}
