#include "Layout/KidsModeLayoutAccessor.h"

#include "Library/Scene/SceneObjUtil.h"

#include "System/GameDataUtil.h"

KidsModeLayoutAccessor::KidsModeLayoutAccessor() {}

const char* KidsModeLayoutAccessor::getSceneObjName() const {
    return "キッズモードレイアウトアクセサ";
}

namespace rs {
void setKidsModeLayoutDisable(const al::IUseSceneObjHolder* user) {
    al::getSceneObj<KidsModeLayoutAccessor>(user)->setKidsModeLayoutDisable();
}

void setKidsModeLayoutEnable(const al::IUseSceneObjHolder* user) {
    al::getSceneObj<KidsModeLayoutAccessor>(user)->setKidsModeLayoutEnable();
}

bool isKidsModeLayoutDisable(const al::IUseSceneObjHolder* user) {
    return !rs::isKidsMode(user) ||
           al::getSceneObj<KidsModeLayoutAccessor>(user)->isKidsModeLayoutDisable();
}

}  // namespace rs
