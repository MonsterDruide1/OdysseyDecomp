#include "Library/Camera/CameraSubTargetBase.h"
#include "Library/Area/IUseAreaObj.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Scene/Scene.h"

al::CameraStopJudge::CameraStopJudge() {
    mIsInObjArea = false;
    mIsNotInObjArea = false;
    mStopJudge = false;
    mAreaObjDirector = nullptr;
}

bool al::CameraStopJudge::isStop() const {
    if (mStopJudge)
        return false;
    return mIsInObjArea || mIsNotInObjArea;
}

void al::CameraStopJudge::update(sead::Vector3f const& position) {
    mIsInObjArea = al::isInAreaObj(this, "CameraStopArea", position);
    return;
}

al::AreaObjDirector* al::CameraStopJudge::getAreaObjDirector() const {
    return mAreaObjDirector;
}