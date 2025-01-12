#pragma once

#include <basis/seadTypes.h>

namespace al {

class Scene;
class PlayerHolder;

PlayerHolder* getScenePlayerHolder(const Scene* scene);

}  // namespace al
