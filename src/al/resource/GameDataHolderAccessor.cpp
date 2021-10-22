#include "al/resource/GameDataHolderAccessor.h"

#include "al/scene/ISceneObj.h"
#include "al/scene/SceneObjHolder.h"
#include "al/util/SceneUtil.h"

GameDataHolderAccessor::GameDataHolderAccessor(const al::IUseSceneObjHolder* holder) {
    mSceneObj = al::getSceneObj(holder, 18);
}
GameDataHolderAccessor::GameDataHolderAccessor(const al::SceneObjHolder* holder) {
    mSceneObj = holder->getObj(18);
}
