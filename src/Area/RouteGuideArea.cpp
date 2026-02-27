#include "Area/RouteGuideArea.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Placement/PlacementFunction.h"

RouteGuideArea::RouteGuideArea(const char* name) : al::AreaObj(name) {}

void RouteGuideArea::init(const al::AreaInitInfo& info) {
    al::AreaObj::init(info);
    al::tryGetArg(&mIsGuide3D, info, "IsGuide3D");
    al::tryGetLinksTrans(&mTargetPosition, info, "TargetPosition");
    if (rs::isKidsMode(this))
        rs::onRouteGuideSystem(this);
}

void RouteGuideArea::calcGuidePos(sead::Vector3f* guidePos) const {
    guidePos->set(mTargetPosition);
}
