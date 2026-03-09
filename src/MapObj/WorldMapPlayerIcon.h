#pragma once

#include "MapObj/WorldMapParts.h"

namespace al {
struct ActorInitInfo;
}

class WorldMapPlayerIcon : public WorldMapPartsFloat {
public:
    static WorldMapPlayerIcon* create(const char* name, const al::ActorInitInfo& initInfo,
                                      const sead::Matrix34f* worldMtx);

    WorldMapPlayerIcon(const char* name);

    void movement() override;
    void calcAnim() override;
};

static_assert(sizeof(WorldMapPlayerIcon) == 0x170);
