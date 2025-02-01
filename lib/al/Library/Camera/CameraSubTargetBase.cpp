#include "Library/Camera/CameraSubTargetBase.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Area/IUseAreaObj.h"
#include "Library/Scene/Scene.h"

al::CameraStopJudge::CameraStopJudge() {
    mIsInObjArea = false;
    mIsNotInObjArea = false;
    mIsStop = false;
    mAreaObjDirector = nullptr;
}

bool al::CameraStopJudge::isStop() const {
    if (mIsStop)
        return false;
    return mIsInObjArea || mIsNotInObjArea;
}

void al::CameraStopJudge::update(const sead::Vector3f& position) {
    mIsInObjArea = al::isInAreaObj(this, "CameraStopArea", position);
    return;
}

al::AreaObjDirector* al::CameraStopJudge::getAreaObjDirector() const {
    return mAreaObjDirector;
}
