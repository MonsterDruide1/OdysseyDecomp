#include "Library/Scene/ISceneObj.h"

namespace al {

const char* ISceneObj::getSceneObjName() const {
    return "";
}

void ISceneObj::initAfterPlacementSceneObj(const ActorInitInfo&) {}

void ISceneObj::initSceneObj() {}

}  // namespace al
