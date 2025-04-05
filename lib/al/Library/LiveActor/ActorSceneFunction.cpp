#include "Library/LiveActor/ActorSceneFunction.h"

#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/DemoDirector.h"
#include "Library/Scene/SceneStopCtrl.h"
#include "Library/Screen/ScreenCoverCtrl.h"

namespace al {
ActorSceneInfo::ActorSceneInfo() = default;

void stopScene(const LiveActor* actor, s32 stopFrames, s32 delayFrames) {
    stopScene(actor->getSceneInfo()->sceneStopCtrl, stopFrames, delayFrames);
}

bool isStopScene(const LiveActor* actor) {
    return isStopScene(actor->getSceneInfo()->sceneStopCtrl);
}

void stopScene(SceneStopCtrl* ctrl, s32 stopFrames, s32 delayFrames) {
    ctrl->reqeustStopScene(stopFrames, delayFrames);
}

bool isStopScene(SceneStopCtrl* ctrl) {
    return ctrl->isStop();
}

void requestCaptureScreenCover(const LiveActor* actor, s32 coverFrames) {
    actor->getSceneInfo()->screenCoverCtrl->requestCaptureScreenCover(coverFrames);
}

bool requestStartDemo(const LiveActor* actor, const char* name) {
    return actor->getSceneInfo()->demoDirector->requestStartDemo(name);
}

void requestEndDemo(const LiveActor* actor, const char* name) {
    actor->getSceneInfo()->demoDirector->requestEndDemo(name);
}

void addDemoActor(LiveActor* actor) {
    actor->getSceneInfo()->demoDirector->addDemoActor(actor);
}

}  // namespace al
