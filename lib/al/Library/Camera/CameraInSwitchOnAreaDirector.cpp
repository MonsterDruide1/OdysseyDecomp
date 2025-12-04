#include "CameraInSwitchOnAreaDirector.h"

#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

namespace al {

CameraInSwitchOnAreaDirector::CameraInSwitchOnAreaDirector() {}

void CameraInSwitchOnAreaDirector::init(const SceneCameraInfo* sceneCameraInfo,
                                        AreaObjDirector* areaObjDirector) {
    mSceneCameraInfo = sceneCameraInfo;
    mAreaObjDirector = areaObjDirector;
}

void CameraInSwitchOnAreaDirector::initAfterPlacement() {
    mAreaObjGroup = tryFindAreaObjGroup(this, "CameraInSwitchOnArea");
}

void CameraInSwitchOnAreaDirector::update() {
    if (mAreaObjGroup == nullptr)
        return;

    s32 areaNum = mAreaObjGroup->getSize();
    for (s32 areaIdx = 0; areaIdx < areaNum; areaIdx++) {
        AreaObj* area = mAreaObjGroup->getAreaObj(areaIdx);

        s32 viewNum = getViewNumMax(mSceneCameraInfo);
        for (s32 viewIdx = 0; viewIdx < viewNum; viewIdx++) {
            if (!isValidView(mSceneCameraInfo, viewIdx))
                continue;

            if (isInAreaPos(area, getCameraAt(mSceneCameraInfo, viewIdx))) {
                tryOnStageSwitch(area, "SwitchCameraInOn");
                break;
            }
        }
    }
}

AreaObjDirector* CameraInSwitchOnAreaDirector::getAreaObjDirector() const {
    return mAreaObjDirector;
}

}  // namespace al
