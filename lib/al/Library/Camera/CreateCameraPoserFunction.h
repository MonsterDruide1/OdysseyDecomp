#pragma once

namespace al {
class CameraPoser;

template <typename T>
CameraPoser* createCameraPoserFunction(const char* cameraPoserName) {
    return new T(cameraPoserName);
}
}  // namespace al
