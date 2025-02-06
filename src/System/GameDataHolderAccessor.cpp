#include "System/GameDataHolderAccessor.h"

#include "Library/Scene/SceneObjHolder.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Scene/SceneObjFactory.h"
#include "System/GameDataHolder.h"

GameDataHolderAccessor::GameDataHolderAccessor(const al::IUseSceneObjHolder* holder) {
    mData = al::getSceneObj<GameDataHolder>(holder, SceneObjID_GameDataHolder);
}

GameDataHolderAccessor::GameDataHolderAccessor(const al::SceneObjHolder* holder) {
    mData = reinterpret_cast<GameDataHolder*>(holder->getObj(SceneObjID_GameDataHolder));
}
