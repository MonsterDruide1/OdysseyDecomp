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
    mAreaObjGroup = tryFindAreaObjGroup(this, "CameraAngleVerticalRequestArea");
}

void CameraAngleVerticalRequester::update(const sead::Vector3f& position) {
    if (mAreaObjGroup != nullptr) {
        AreaObj* areaObj = tryGetAreaObj(mAreaObjGroup, position);
        if (areaObj != mAreaObj) {
            mAreaObj = areaObj;
            mUpdateIndex = -1;
            if (areaObj != nullptr)
                getArg(&mVerticalAngle, *mAreaObj->getPlacementInfo(), "AngleVertical");
        }
        mUpdateIndex++;
    }
}
}  // namespace al
