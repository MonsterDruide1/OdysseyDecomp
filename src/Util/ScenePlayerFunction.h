#pragma once

namespace al {
class Scene;
}

namespace ScenePlayerFunction {
void forceShowPlayer(const al::Scene* scene);
void forceHidePlayer(const al::Scene* scene);
void startSnapShotMode(const al::Scene* scene);
void endSnapShotMode(const al::Scene* scene);
void updatePlayerDither(const al::Scene* scene);
}  // namespace ScenePlayerFunction
