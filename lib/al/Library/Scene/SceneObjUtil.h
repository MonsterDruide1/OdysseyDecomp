#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseSceneObjHolder;
class ISceneObj;

ISceneObj* createSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
void setSceneObj(const IUseSceneObjHolder* user, ISceneObj*, s32 sceneObjId);
ISceneObj* getSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
ISceneObj* tryGetSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
bool isExistSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
void deleteSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
bool tryDeleteSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);

template <typename T>
inline T* getSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId) {
    return static_cast<T*>(getSceneObj(user, sceneObjId));
}

}  // namespace al
