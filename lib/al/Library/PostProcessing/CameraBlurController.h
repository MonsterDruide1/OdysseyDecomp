#pragma once

namespace al {
class LiveActor;
}

namespace alCameraBlurFunction {
void setCameraBlurName(const al::LiveActor* actor, const char* name);
void resetCameraBlurName(const al::LiveActor* actor);
void invalidateCameraBlur(const al::LiveActor* actor);
void validateCameraBlur(const al::LiveActor* actor);
}  // namespace alCameraBlurFunction
