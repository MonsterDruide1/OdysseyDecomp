#pragma once

#include "Library/Area/IUseAreaObj.h"

namespace al {
class AreaObjDirector;
class SceneCameraInfo;
class AreaObjGroup;

class CameraInSwitchOnAreaDirector : public IUseAreaObj {
public:
    CameraInSwitchOnAreaDirector();
    void init(const SceneCameraInfo* sceneCameraInfo, AreaObjDirector* areaObjDirector);
    void initAfterPlacement();
    void update();
    AreaObjDirector* getAreaObjDirector() const;

private:
    const SceneCameraInfo* mSceneCameraInfo = nullptr;
    AreaObjDirector* mAreaObjDirector = nullptr;
    AreaObjGroup* mAreaObjGroup = nullptr;
};

static_assert(sizeof(CameraInSwitchOnAreaDirector) == 0x20);
}  // namespace al
