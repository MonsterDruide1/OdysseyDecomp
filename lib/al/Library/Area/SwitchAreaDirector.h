#pragma once

#include <container/seadPtrArray.h>

#include "Library/Area/IUseAreaObj.h"

namespace al {
class AreaObjDirector;
class CameraDirector;
class PlayerHolder;
class SwitchAreaTargetInfo;
class SwitchKeepOnAreaGroup;
class SwitchOnAreaGroup;

class SwitchAreaDirector : public IUseAreaObj {
public:
    static SwitchAreaDirector* tryCreate(AreaObjDirector* areaObjDirector,
                                         const PlayerHolder* playerHolder,
                                         const CameraDirector* cameraDirector,
                                         s32 switchOnAreaGroupCount,
                                         s32 switchKeepOnAreaGroupCount);

    SwitchAreaDirector(AreaObjDirector* areaObjDirector, const PlayerHolder* playerHolder,
                       const CameraDirector* cameraDirector, s32 switchOnAreaGroupCount,
                       s32 switchKeepOnAreaGroupCount);

    AreaObjDirector* getAreaObjDirector() const override;

    void update();
    void endInit();
    void registerSwitchOnAreaGroup(SwitchOnAreaGroup* switchOnAreaGroup);
    void registerSwitchKeepOnAreaGroup(SwitchKeepOnAreaGroup* switchKeepOnAreaGroup);

private:
    AreaObjDirector* mAreaObjDirector;
    const PlayerHolder* mPlayerHolder;
    const CameraDirector* mCameraDirector;
    SwitchAreaTargetInfo* mSwitchAreaTargetInfo = nullptr;
    sead::PtrArray<SwitchOnAreaGroup> mSwitchOnAreaGroups = {};
    sead::PtrArray<SwitchKeepOnAreaGroup> mSwitchKeepOnAreaGroups = {};
    bool mHasSwitchAreas = false;
};

static_assert(sizeof(SwitchAreaDirector) == 0x50);

}  // namespace al
