#pragma once

#include "Library/Layout/LayoutActor.h"

class MapData;

class MapTerrainLayout : public al::LayoutActor {
public:
    MapTerrainLayout(const char* name);

    bool tryChangePrintWorld(s32 worldId);
    f32 getPaneSize() const;

private:
    MapData* mMapData = nullptr;
};
