#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Area/AreaObjDirector.h"
#include "Library/Camera/CameraFlagCtrl.h"

namespace al {
struct CameraStartInfo;

class CameraStartParamCtrl {
public:
    CameraStartParamCtrl();

    void init(AreaObjDirector*, const CameraFlagCtrl*);
    void initAfterPlacement();

    void update(const sead::Vector3f&);

    void tryApplyParam(CameraStartInfo* camStartInfo);

    AreaObjDirector* getAreaObjDirector() const;
};

}  // namespace al
