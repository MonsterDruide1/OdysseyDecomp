#include "Layout/KidsModeLayoutAccessor.h"

#include "Library/Scene/SceneObjUtil.h"

#include "Scene/SceneObjFactory.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataUtil.h"

KidsModeLayoutAccessor::KidsModeLayoutAccessor() {}

const char* KidsModeLayoutAccessor::getSceneObjName() const {
    return "キッズモードレイアウトアクセサ";
}

namespace rs {
void setKidsModeLayoutDisable(const al::IUseSceneObjHolder* user) {
    al::getSceneObj<KidsModeLayoutAccessor>(user, SceneObjID_KidsModeLayoutAccessor)
        ->setKidsModeLayoutDisable();
}

void setKidsModeLayoutEnable(const al::IUseSceneObjHolder* user) {
    al::getSceneObj<KidsModeLayoutAccessor>(user, SceneObjID_KidsModeLayoutAccessor)
        ->setKidsModeLayoutEnable();
}

bool isKidsModeLayoutDisable(const al::IUseSceneObjHolder* user) {
    return !rs::isKidsMode(user) ||
           al::getSceneObj<KidsModeLayoutAccessor>(user, SceneObjID_KidsModeLayoutAccessor)
               ->isKidsModeLayoutDisable();
}

}  // namespace rs
