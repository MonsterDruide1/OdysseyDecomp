#pragma once

#include <basis/seadTypes.h>

namespace al {

class ISceneObj;
class Scene;
class IUseSceneObjHolder;
class PlayerHolder;

ISceneObj* getSceneObj(const IUseSceneObjHolder*, s32);
PlayerHolder* getScenePlayerHolder(const Scene*);

}  // namespace al
