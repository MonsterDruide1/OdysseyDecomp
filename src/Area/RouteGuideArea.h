#pragma once

#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"
#include "Library/Scene/IUseSceneObjHolder.h"

#include "MapObj/RouteGuideDirector.h"
#include "System/GameDataUtil.h"

class RouteGuideArea : public al::AreaObj {
public:
    RouteGuideArea(const char* name);

    void init(const al::AreaInitInfo& info) override;
    void calcGuidePos(sead::Vector3f* guidePos) const;

private:
    sead::Vector3f mTargetPosition = sead::Vector3f::zero;
    bool mIsGuide3D = false;
};
