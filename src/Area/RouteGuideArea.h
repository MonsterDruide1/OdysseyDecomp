#pragma once

#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"

class RouteGuideArea : public al::AreaObj {
public:
    RouteGuideArea(const char* name);

    void init(const al::AreaInitInfo& areaInitInfo) override;
    void calcGuidePos(sead::Vector3f* guidePos) const;

private:
    sead::Vector3f mGuidePos = {0.0, 0.0, 0.0};
    bool mIsGuide3D = false;
};
