#pragma once

#include "Library/Factory/Factory.h"

namespace alSceneFunction {
class Scene;

using SceneCreatorFunction = Scene* (*)(const char* sceneName);

class SceneFactory : public al::Factory<SceneCreatorFunction> {
public:
    SceneFactory(const char* factoryName);
};
}  // namespace alSceneFunction
