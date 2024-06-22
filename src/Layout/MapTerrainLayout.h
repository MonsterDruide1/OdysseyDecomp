#pragma once

#include "Library/Layout/LayoutActor.h"

#include "System/MapDataHolder.h"

class MapTerrainLayout : public al::LayoutActor {
public:
    MapTerrainLayout(const char*);

    bool tryChangePrintWorld(s32);
    f32 getPaneSize() const;

private:
    MapData* mMapData = nullptr;
};
