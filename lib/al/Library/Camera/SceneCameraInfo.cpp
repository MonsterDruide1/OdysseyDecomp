#include "Library/Camera/SceneCameraInfo.h"

#include "Library/Camera/CameraViewInfo.h"

namespace al {

SceneCameraInfo::SceneCameraInfo(s32 maxViewNum) : mViewNumMax(maxViewNum) {
    mViewArray = new CameraViewInfo*[maxViewNum];
    for (s32 i = 0; i < mViewNumMax; i++)
        mViewArray[i] = nullptr;
}

void SceneCameraInfo::initViewInfo(CameraViewInfo* cameraViewInfo) {
    mViewArray[cameraViewInfo->getIndex()] = cameraViewInfo;
}

const char* SceneCameraInfo::getViewName(s32 num) const {
    if (num == 0)
        return "TV";
    else if (num == 1)
        return "サブ";  // Sub/Lower
    else
        return "メイン";  // Main
}

}  // namespace al
