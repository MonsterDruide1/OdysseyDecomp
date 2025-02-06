#include "Library/LiveActor/ActorSceneFunction.h"

#include "Library/Demo/DemoDirector.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/SceneStopCtrl.h"
#include "Library/Screen/ScreenCoverCtrl.h"

namespace al {
ActorSceneInfo::ActorSceneInfo() = default;

void stopScene(const LiveActor* actor, s32 stopFrames, s32 delayFrames) {
    stopScene(actor->getSceneInfo()->mSceneStopCtrl, stopFrames, delayFrames);
}

bool isStopScene(const LiveActor* actor) {
    return isStopScene(actor->getSceneInfo()->mSceneStopCtrl);
}

void stopScene(SceneStopCtrl* ctrl, s32 stopFrames, s32 delayFrames) {
    ctrl->reqeustStopScene(stopFrames, delayFrames);
}

bool isStopScene(SceneStopCtrl* ctrl) {
    return ctrl->isStop();
}

void requestCaptureScreenCover(const LiveActor* actor, s32 coverFrames) {
    actor->getSceneInfo()->mScreenCoverCtrl->reqeustCaptureScreenCover(coverFrames);
}

bool requestStartDemo(const LiveActor* actor, const char* name) {
    return actor->getSceneInfo()->mDemoDirector->requestStartDemo(name);
}

bool requestEndDemo(const LiveActor* actor, const char* name) {
    return actor->getSceneInfo()->mDemoDirector->requestEndDemo(name);
}

void addDemoActor(LiveActor* actor) {
    actor->getSceneInfo()->mDemoDirector->addDemoActor(actor);
}

}  // namespace al
