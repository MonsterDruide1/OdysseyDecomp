#pragma once

namespace al {
class FunctorBase;
class IUseSceneObjHolder;
}  // namespace al

namespace rs {
void listenSnapShotModeOnOff(const al::IUseSceneObjHolder* user, const al::FunctorBase& actionOnOn,
                             const al::FunctorBase& actionOnOff);
}  // namespace rs

namespace SceneEventNotifyFunction {
void notifySnapShotModeOn(const al::IUseSceneObjHolder* user);
void notifySnapShotModeOff(const al::IUseSceneObjHolder* user);
}  // namespace SceneEventNotifyFunction
