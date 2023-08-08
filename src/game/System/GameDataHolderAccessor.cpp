#include "game/System/GameDataHolderAccessor.h"

#include "al/Library/Scene/ISceneObj.h"
#include "al/Library/Scene/SceneObjHolder.h"
#include "al/Library/Scene/SceneUtil.h"

GameDataHolderAccessor::GameDataHolderAccessor(const al::IUseSceneObjHolder* holder) {
    mSceneObj = al::getSceneObj(holder, 18);
}
GameDataHolderAccessor::GameDataHolderAccessor(const al::SceneObjHolder* holder) {
    mSceneObj = holder->getObj(18);
}
