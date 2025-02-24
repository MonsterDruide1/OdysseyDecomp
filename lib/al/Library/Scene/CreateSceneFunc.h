#pragma once

namespace al {
class Scene;
}

namespace alSceneFunction {
template <typename T>
al::Scene* createSceneFunc() {
    return new T();
}
}  // namespace alSceneFunction
