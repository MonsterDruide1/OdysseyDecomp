#pragma once

#include <basis/seadTypes.h>

#include "Library/Camera/CameraTicket.h"

namespace al {

struct CameraStartInfo {
    CameraTicket::Priority mPrePriorityType;
    const char* mPreCameraName;
    f32 mPreCameraSwingAngleH;
    f32 mPreCameraSwingAngleV;
    f32 mPreCameraMaxSwingAngleH;
    f32 mPreCameraMaxSwingAngleV;
    bool mIsInvalidCollidePreCamera;
    bool mIsInvalidKeepPreCameraDistance;
    bool mIsInvalidKeepPreCameraDistanceIfNoCollide;
    bool mIsValidResetPreCameraPose;
    bool mIsValidKeepPreSelfCameraPose;
    bool mIsGrounded;
    bool mIsExistAreaAngleH;
    f32 mAreaAngleH;
    bool mIsExistAreaAngleV;
    f32 mAreaAngleV;
    bool mIsExistNextPoseByPreCamera;
    f32 mNextAngleHByPreCamera;
    f32 mNextAngleVByPreCamera;
};

static_assert(sizeof(CameraStartInfo) == 0x40);

}  // namespace al
