#include "Scene/SceneObjFactory.h"

#include "Library/Scene/SceneObjHolder.h"

namespace al {
class ISceneObj;
}  // namespace al

static al::ISceneObj* sceneObjCreator(s32 id) {
    // TODO
    return nullptr;
}

al::SceneObjHolder* SceneObjFactory::createSceneObjHolder() {
    return new al::SceneObjHolder(&sceneObjCreator, SceneObjID_Max);
}
