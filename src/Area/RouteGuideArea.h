#pragma once

#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"

namespace al {
class IUseSceneObjHolder;
}

namespace rs {
bool isKidsMode(const al::IUseSceneObjHolder*);
void onRouteGuideSystem(const al::IUseSceneObjHolder*);
}  // namespace rs

class RouteGuideArea : public al::AreaObj {
public:
    RouteGuideArea(const char* name);

    void init(const al::AreaInitInfo& info) override;
    void calcGuidePos(sead::Vector3f* guidePos) const;

private:
    sead::Vector3f mTargetPosition = sead::Vector3f::zero;
    bool mIsGuide3D = false;
};
