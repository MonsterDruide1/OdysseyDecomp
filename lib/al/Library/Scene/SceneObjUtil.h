#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseSceneObjHolder;
class ISceneObj;

ISceneObj* createSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
void setSceneObj(const IUseSceneObjHolder* user, ISceneObj* obj, s32 sceneObjId);
ISceneObj* getSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
ISceneObj* tryGetSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
bool isExistSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
void deleteSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);
bool tryDeleteSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);

template <typename T>
inline T* createSceneObj(const IUseSceneObjHolder* user) {
    return static_cast<T*>(createSceneObj(user, T::sSceneObjId));
}

template <typename T>
inline void setSceneObj(const IUseSceneObjHolder* user, T* obj) {
    setSceneObj(user, obj, T::sSceneObjId);
}

template <typename T>
inline T* getSceneObj(const IUseSceneObjHolder* user) {
    return static_cast<T*>(getSceneObj(user, T::sSceneObjId));
}

template <typename T>
inline T* tryGetSceneObj(const IUseSceneObjHolder* user) {
    return static_cast<T*>(tryGetSceneObj(user, T::sSceneObjId));
}

template <typename T>
inline bool isExistSceneObj(const IUseSceneObjHolder* user) {
    return isExistSceneObj(user, T::sSceneObjId);
}

template <typename T>
inline void deleteSceneObj(const IUseSceneObjHolder* user) {
    deleteSceneObj(user, T::sSceneObjId);
}

template <typename T>
inline bool tryDeleteSceneObj(const IUseSceneObjHolder* user) {
    return tryDeleteSceneObj(user, T::sSceneObjId);
}

}  // namespace al
