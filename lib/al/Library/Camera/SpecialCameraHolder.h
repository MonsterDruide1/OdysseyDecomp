#pragma once

namespace al {
class CameraTicket;

class SpecialCameraHolder {
public:
    SpecialCameraHolder();

    void allocEntranceCameraBuffer(s32 maxEntries);
    void registerEntranceCamera(CameraTicket* ticket);
    CameraTicket* findEntranceCamera(const char* suffix) const;

private:
    CameraTicket** mEntranceCameras = nullptr;
    s32 mMaxEntranceCameras = 0;
    s32 mNumEntranceCameras = 0;
};

}  // namespace al
