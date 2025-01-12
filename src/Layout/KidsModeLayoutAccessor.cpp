#include "Layout/KidsModeLayoutAccessor.h"

#include "Library/Scene/SceneObjUtil.h"

#include "Scene/SceneObjFactory.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataUtil.h"

KidsModeLayoutAccessor::KidsModeLayoutAccessor() {}

// NON_MATCHING: KidsModeLayoutAccessor destructor

const char* KidsModeLayoutAccessor::getSceneObjName() const {
    return "キッズモードレイアウトアクセサ";
}

namespace rs {
void setKidsModeLayoutDisable(const al::IUseSceneObjHolder* user) {
    al::getSceneObj<KidsModeLayoutAccessor>(user, SceneObjID_KidsModeLayoutAccessor)
        ->mIsKidsModeLayoutDisable = true;
}

void setKidsModeLayoutEnable(const al::IUseSceneObjHolder* user) {
    al::getSceneObj<KidsModeLayoutAccessor>(user, SceneObjID_KidsModeLayoutAccessor)
        ->mIsKidsModeLayoutDisable = false;
}

bool isKidsModeLayoutDisable(const al::IUseSceneObjHolder* user) {
    return !rs::isKidsMode(user) ||
           al::getSceneObj<KidsModeLayoutAccessor>(user, SceneObjID_KidsModeLayoutAccessor)
               ->mIsKidsModeLayoutDisable;
}

}  // namespace rs
