#pragma once

#include <basis/seadTypes.h>

#include "Library/Camera/SpecialCameraHolder.h"

namespace al {

class CameraRequestParamHolder;
class CameraSwitchRequester;

class PauseCameraCtrl {
public:
    PauseCameraCtrl(f32 v);

private:
    bool mIsCameraPause = false;
    f32 _4;
};

class SceneCameraViewCtrl {
public:
    SceneCameraViewCtrl();

    CameraSwitchRequester* getSwitchRequester() { return mSwitchRequester; }

private:
    CameraSwitchRequester* mSwitchRequester = nullptr;
    const char* mViewName = "Start";
    void* _10 = nullptr;
};

class SceneCameraCtrl {
public:
    SceneCameraCtrl();
    void init(s32 viewNum);

    SceneCameraViewCtrl* getSceneViewAt(s32 idx) { return &mViewArray[idx]; }

private:
    s32 mViewNumMax = 0;
    SceneCameraViewCtrl* mViewArray = nullptr;
    CameraRequestParamHolder* mRequestParam = nullptr;
    SpecialCameraHolder* mSpecialCameraHolder = nullptr;
};

static_assert(sizeof(PauseCameraCtrl) == 0x8);
static_assert(sizeof(SceneCameraCtrl) == 0x20);
static_assert(sizeof(SceneCameraViewCtrl) == 0x18);

}  // namespace al
