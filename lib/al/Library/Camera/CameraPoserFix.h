#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {
class LiveActor;

class CameraPoserFix : public CameraPoser {
public:
    CameraPoserFix(const char*);

    static const char* getFixAbsoluteCameraName();
    static const char* getFixDoorwayCameraName();

    void initCameraPosAndLookAtPos(const sead::Vector3f& cameraPos,
                                   const sead::Vector3f& lookAtPos);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;

private:
    sead::Vector3f mLookAtPos;
    f32 mDistance;
    f32 mAngleV;
    f32 mAngleH;
    bool mIsCalcNearestAtFromPreAt;
    f32 mPreLookAtPos;
    f32 _160;
    f32 _164;
};

static_assert(sizeof(CameraPoserFix) == 0x168);

}  // namespace al
