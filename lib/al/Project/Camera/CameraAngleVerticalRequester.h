#pragma once

#include <math/seadVector.h>

#include "Library/Area/IUseAreaObj.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class AreaObjDirector;
class AreaObjGroup;
class AreaObj;
class PlacementInfo;

class CameraAngleVerticalRequester : public HioNode, public IUseAreaObj {
public:
    CameraAngleVerticalRequester();
    AreaObjDirector* getAreaObjDirector() const override;
    void init(AreaObjDirector* areaObjDirector);
    void initAfterPlacement();
    void update(const sead::Vector3f& position);

private:
    s32 mUpdateIndex;
    f32 mVerticalAngle;
    const AreaObj* mAreaObj;
    const AreaObjGroup* mAreaObjGroup;
    AreaObjDirector* mAreaObjDirector;
};
}  // namespace al
