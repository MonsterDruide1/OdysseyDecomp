#include "Project/Camera/CameraAngleVerticalRequester.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {

CameraAngleVerticalRequester::CameraAngleVerticalRequester() = default;

AreaObjDirector* CameraAngleVerticalRequester::getAreaObjDirector() const {
    return mAreaObjDirector;
}

void CameraAngleVerticalRequester::init(AreaObjDirector* areaObjDirector) {
    mAreaObjDirector = areaObjDirector;
}

void CameraAngleVerticalRequester::initAfterPlacement() {
    mRequestAreaGroup = tryFindAreaObjGroup(this, "CameraAngleVerticalRequestArea");
}

void CameraAngleVerticalRequester::update(const sead::Vector3f& position) {
    if (mRequestAreaGroup == nullptr)
        return;
    AreaObj* areaObj = tryGetAreaObj(mRequestAreaGroup, position);
    if (areaObj != mRequestArea) {
        mRequestArea = areaObj;
        mFramesUnchanged = -1;
        if (areaObj != nullptr)
            getArg(&mAngleVertical, mRequestArea->getPlacementInfo(), "AngleVertical");
    }
    mFramesUnchanged++;
}
}  // namespace al
