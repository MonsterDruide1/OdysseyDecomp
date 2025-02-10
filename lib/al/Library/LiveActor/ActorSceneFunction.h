#pragma once

#include <basis/seadTypes.h>

namespace al {

class LiveActor;
class SceneStopCtrl;

void stopScene(const LiveActor* actor, s32 stopFrames, s32 delayFrames);
bool isStopScene(const LiveActor* actor);

void stopScene(SceneStopCtrl* ctrl, s32 stopFrames, s32 delayFrames);
bool isStopScene(SceneStopCtrl* ctrl);

void requestCaptureScreenCover(const LiveActor* actor, s32 coverFrames);

bool requestStartDemo(const LiveActor* actor, const char* name);
void requestEndDemo(const LiveActor* actor, const char* name);
void addDemoActor(LiveActor* actor);

}  // namespace al
