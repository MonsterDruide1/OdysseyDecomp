#pragma once

namespace al {
class CameraViewFlag {
public:
    CameraViewFlag() {}

    void resetAllFlag() { mIsTrue = false; }

private:
    bool mIsTrue = false;
};
}  // namespace al
