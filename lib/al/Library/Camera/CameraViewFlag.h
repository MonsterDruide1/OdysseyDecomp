#pragma once

namespace al {

class CameraViewFlag {
public:
    CameraViewFlag();

    void resetAllFlag();

    void setInvalidCameraBlur(bool invalid) { mIsInvalidCameraBlur = invalid; }

    bool isInvalidCameraBlur() const { return mIsInvalidCameraBlur; }

private:
    bool mIsInvalidCameraBlur = false;
};

}  // namespace al
