#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/IUseAreaObj.h"
#include "Library/Camera/CameraFlagCtrl.h"
#include "Library/HostIO/HioNode.h"

namespace al {
struct CameraStartInfo;

class CameraStartParamCtrl : public HioNode, public IUseAreaObj {
public:
    CameraStartParamCtrl();

    void init(AreaObjDirector*, const CameraFlagCtrl*);
    void initAfterPlacement();

    void update(const sead::Vector3f&);

    void tryApplyParam(CameraStartInfo* camStartInfo);

    AreaObjDirector* getAreaObjDirector() const override;

private:
    void* _0[5];
};

static_assert(sizeof(CameraStartParamCtrl) == 0x30);

}  // namespace al
