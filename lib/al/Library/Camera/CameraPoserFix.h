#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {
class LiveActor;

class CameraPoserFix : public CameraPoser {
public:
    CameraPoserFix(const char* name);

    static const char* getFixAbsoluteCameraName();
    static const char* getFixDoorwayCameraName();

    void initCameraPosAndLookAtPos(const sead::Vector3f& cameraPos,
                                   const sead::Vector3f& lookAtPos);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void update() override;

private:
    sead::Vector3f mLookAtPos = {0.0f, 0.0f, 0.0f};
    f32 mDistance = 1800.0f;
    f32 mAngleV = 30.0f;
    f32 mAngleH = 0.0f;
    bool mIsCalcNearestAtFromPreAt = false;
    sead::Vector3f mPreLookAtPos = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(CameraPoserFix) == 0x168);

}  // namespace al
