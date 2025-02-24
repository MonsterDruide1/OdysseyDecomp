#pragma once

#include "Library/Factory/Factory.h"

namespace al {
class Scene;
}

namespace alSceneFunction {
using SceneCreatorFunction = al::Scene* (*)();

class SceneFactory : public al::Factory<SceneCreatorFunction> {
public:
    SceneFactory(const char* factoryName);
};
}  // namespace alSceneFunction
