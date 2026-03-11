#include "Scene/SceneEventNotifyFunction.h"

#include "Scene/SceneEventFunctorListener.h"
#include "Scene/SceneEventNotifier.h"

namespace rs {
void listenSnapShotModeOnOff(const al::IUseSceneObjHolder* user, const al::FunctorBase& actionOnOn,
                             const al::FunctorBase& actionOnOff) {
    listenSceneEvent(user, "SnapShotModeOn", new SceneEventFunctorListener(actionOnOn));
    listenSceneEvent(user, "SnapShotModeOff", new SceneEventFunctorListener(actionOnOff));
}
}  // namespace rs

namespace SceneEventNotifyFunction {
void notifySnapShotModeOn(const al::IUseSceneObjHolder* user) {
    rs::notifySceneEvent(user, "SnapShotModeOn");
}

void notifySnapShotModeOff(const al::IUseSceneObjHolder* user) {
    rs::notifySceneEvent(user, "SnapShotModeOff");
}
}  // namespace SceneEventNotifyFunction
