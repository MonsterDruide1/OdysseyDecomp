#pragma once

namespace al {
class CameraDirector;

class IUseCamera {
public:
    virtual CameraDirector* getCameraDirector() const = 0;
};
}  // namespace al
