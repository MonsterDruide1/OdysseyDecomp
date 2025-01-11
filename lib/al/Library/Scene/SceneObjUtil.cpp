#include "Library/Scene/SceneObjUtil.h"

#include "Library/Scene/ISceneObj.h"
#include "Library/Scene/IUseSceneObjHolder.h"
#include "Library/Scene/SceneObjHolder.h"

namespace al {
ISceneObj* createSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId) {
    return user->getSceneObjHolder()->create(sceneObjId);
}

void setSceneObj(const IUseSceneObjHolder* user, ISceneObj* obj, s32 sceneObjId) {
    user->getSceneObjHolder()->setSceneObj(obj, sceneObjId);
}

ISceneObj* getSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId) {
    return user->getSceneObjHolder()->getObj(sceneObjId);
}

ISceneObj* tryGetSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId) {
    return user->getSceneObjHolder()->tryGetObj(sceneObjId);
}

bool isExistSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId) {
    return user->getSceneObjHolder()->isExist(sceneObjId);
}

void deleteSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId) {
    delete user->getSceneObjHolder()->getObj(sceneObjId);
}

bool tryDeleteSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId) {
    ISceneObj* sceneObj = tryGetSceneObj(user, sceneObjId);
    if (sceneObj) {
        delete sceneObj;
        return true;
    }
    return false;
}
}  // namespace al
