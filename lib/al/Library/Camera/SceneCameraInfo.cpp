#include "Library/Camera/SceneCameraInfo.h"

#include "Library/Camera/CameraViewInfo.h"

namespace al {

SceneCameraInfo::SceneCameraInfo(s32 num) : mViewNumMax(num), mViewArray(new CameraViewInfo*[num]) {
    for (s32 i = 0; i < mViewNumMax; i++)
        mViewArray[i] = nullptr;
}

void SceneCameraInfo::initViewInfo(CameraViewInfo* cameraViewInfo) {
    mViewArray[cameraViewInfo->getIndex()] = cameraViewInfo;
}

const char* SceneCameraInfo::getViewName(s32 num) const {
    return num ? num == 1 ? "サブ" : "TV" : "メイン";  // Sub, TV, Main
}

}  // namespace al
