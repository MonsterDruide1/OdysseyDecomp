#pragma once

#include <basis/seadTypes.h>

namespace al {

class CameraRequestParamHolder;
class CameraSwitchRequester;
class SpecialCameraHolder;

class PauseCameraCtrl {
public:
    PauseCameraCtrl(f32 v);

private:
    bool mIsCameraPause = false;
    f32 _4;
};

static_assert(sizeof(PauseCameraCtrl) == 0x8);

class SceneCameraViewCtrl {
public:
    SceneCameraViewCtrl();

    const CameraSwitchRequester* getSwitchRequester() const { return mSwitchRequester; }

private:
    CameraSwitchRequester* mSwitchRequester = nullptr;
    const char* mViewName = "Start";
    void* _10 = nullptr;
};

static_assert(sizeof(SceneCameraViewCtrl) == 0x18);

class SceneCameraCtrl {
public:
    SceneCameraCtrl();
    void init(s32 viewNum);

    const SceneCameraViewCtrl& getSceneViewAt(s32 idx) const { return mViewArray[idx]; }

private:
    s32 mViewNum = 0;
    SceneCameraViewCtrl* mViewArray = nullptr;
    CameraRequestParamHolder* mRequestParamHolder = nullptr;
    SpecialCameraHolder* mSpecialCameraHolder = nullptr;
};

static_assert(sizeof(SceneCameraCtrl) == 0x20);

}  // namespace al
