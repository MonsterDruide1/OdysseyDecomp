#pragma once

namespace al {

class ISceneObj;
class Scene;
class IUseSceneObjHolder;
class PlayerHolder;

ISceneObj* getSceneObj(const IUseSceneObjHolder*, int);
PlayerHolder* getScenePlayerHolder(const Scene*);

}  // namespace al
