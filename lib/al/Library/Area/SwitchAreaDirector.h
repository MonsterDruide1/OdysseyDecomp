#pragma once

#include <basis/seadTypes.h>

#include "Library/Area/IUseAreaObj.h"

namespace al {
class PlayerHolder;
class CameraDirector;
class SwitchOnAreaGroup;
class SwitchKeepOnAreaGroup;

class SwitchAreaDirector : public IUseAreaObj {
public:
    static SwitchAreaDirector* tryCreate(AreaObjDirector*, const PlayerHolder*,
                                         const CameraDirector*, s32, s32);

    SwitchAreaDirector(AreaObjDirector*, const PlayerHolder*, const CameraDirector*, s32, s32);

    AreaObjDirector* getAreaObjDirector() const override;

    void update();
    void endInit();
    void registerSwitchOnAreaGroup(SwitchOnAreaGroup*);
    void registerSwitchKeepOnAreaGroup(SwitchKeepOnAreaGroup*);

private:
    void* filler[9];
};

static_assert(sizeof(SwitchAreaDirector) == 0x50);

}  // namespace al
