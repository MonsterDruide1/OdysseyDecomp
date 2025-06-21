#pragma once

#include <basis/seadTypes.h>

namespace al {
class ISceneObj;
class SceneObjHolder;

class IUseSceneObjHolder {
public:
    virtual SceneObjHolder* getSceneObjHolder() const = 0;

private:
    static const char* sSceneObjName;
};

ISceneObj* createSceneObj(const IUseSceneObjHolder* user, s32 sceneObjId);

}  // namespace al
