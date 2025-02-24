#pragma once

namespace al {
class Scene;
}

namespace alSceneFunction {
template <typename T>
al::Scene* createSceneFunc(const char* sceneName) {
    return new T(sceneName);
}
}  // namespace alSceneFunction
