#pragma once

#include <basis/seadTypes.h>

namespace al {

class CameraSwitchRequester;

class PauseCameraCtrl {
public:
    PauseCameraCtrl(f32 v);

private:
    bool mIsPause = false;
    f32 _4;
};

static_assert(sizeof(PauseCameraCtrl) == 0x8);

class SceneCameraViewCtrl {
public:
    SceneCameraViewCtrl();

    CameraSwitchRequester* getSwitchRequester() { return mSwitchRequester; }

private:
    CameraSwitchRequester* mSwitchRequester = nullptr;
    const char* _8;
    void* _10 = nullptr;
};

static_assert(sizeof(SceneCameraViewCtrl) == 0x18);

class SceneCameraCtrl {
public:
    SceneCameraCtrl();
    void init(s32 viewNum);

    SceneCameraViewCtrl* getSceneViewAt(s32 idx) { return &mViewArray[idx]; }

private:
    s32 mViewNumMax = 0;
    SceneCameraViewCtrl* mViewArray = nullptr;
    void* _10 = nullptr;
    void* _18 = nullptr;
};

static_assert(sizeof(SceneCameraCtrl) == 0x20);

}  // namespace al
