#pragma once

#include "MapObj/WorldMapParts.h"

namespace al {
struct ActorInitInfo;
}

class PropellerRotateCtrl;

class WorldMapKoopaShip : public WorldMapPartsFloat {
public:
    static WorldMapKoopaShip* create(const char* name, const al::ActorInitInfo& initInfo,
                                     const sead::Matrix34f* worldMtx);

    WorldMapKoopaShip(const char* name);

    void control() override;

private:
    PropellerRotateCtrl** mPropellerRotateCtrls = nullptr;
    s32 mPropellerRotateCtrlNum = 0;
};

static_assert(sizeof(WorldMapKoopaShip) == 0x180);
