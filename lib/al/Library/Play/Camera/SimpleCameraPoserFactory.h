#pragma once

#include "Library/Camera/CameraPoserFactory.h"

namespace al {
class SimpleCameraPoserFactory : public CameraPoserFactory {
public:
    SimpleCameraPoserFactory(const char* factoryName);
};
}  // namespace al
