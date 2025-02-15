#include "Library/Camera/CameraStopJudge.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Area/IUseAreaObj.h"

namespace al {

CameraStopJudge::CameraStopJudge() = default;

bool CameraStopJudge::isStop() const {
    if (mIsInvalidStopJudgeByDemo)
        return false;
    return mIsInCameraStopArea || _9;
}

void CameraStopJudge::update(const sead::Vector3f& position) {
    mIsInCameraStopArea = isInAreaObj(this, "CameraStopArea", position);
}

AreaObjDirector* al::CameraStopJudge::getAreaObjDirector() const {
    return mAreaObjDirector;
}
}  // Namespace al
