#pragma once

#include "Library/Camera/CameraPoserFactory.h"

namespace al {
class CameraPoser;
class CameraPoserParallelSimple;
class CameraPoserFollowSimple;
class CameraPoserQuickTurn;

class SimpleCameraPoserFactory : public CameraPoserFactory {
public:
    SimpleCameraPoserFactory(const char*);
};

}  // namespace al
