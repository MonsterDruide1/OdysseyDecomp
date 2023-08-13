#include "game/System/GameDataHolderAccessor.h"

#include "Library/Scene/ISceneObj.h"
#include "Library/Scene/SceneObjHolder.h"
#include "Library/Scene/SceneUtil.h"

GameDataHolderAccessor::GameDataHolderAccessor(const al::IUseSceneObjHolder* holder) {
    mSceneObj = al::getSceneObj(holder, 18);
}
GameDataHolderAccessor::GameDataHolderAccessor(const al::SceneObjHolder* holder) {
    mSceneObj = holder->getObj(18);
}
