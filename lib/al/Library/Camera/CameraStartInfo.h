#pragma once

#include <basis/seadTypes.h>

#include "Library/Camera/CameraTicket.h"

namespace al {

struct CameraStartInfo {
    CameraTicket::Priority prePriorityType;
    const char* preCameraName;
    f32 preCameraSwingAngleH;
    f32 preCameraSwingAngleV;
    f32 preCameraMaxSwingAngleH;
    f32 preCameraMaxSwingAngleV;
    bool isInvalidCollidePreCamera;
    bool isInvalidKeepPreCameraDistance;
    bool isInvalidKeepPreCameraDistanceIfNoCollide;
    bool isValidResetPreCameraPose;
    bool isValidKeepPreSelfCameraPose;
    bool isGrounded;
    bool isExistAreaAngleH;
    f32 areaAngleH;
    bool isExistAreaAngleV;
    f32 areaAngleV;
    bool isExistNextPoseByPreCamera;
    f32 nextAngleHByPreCamera;
    f32 nextAngleVByPreCamera;
};

static_assert(sizeof(CameraStartInfo) == 0x40);

}  // namespace al
