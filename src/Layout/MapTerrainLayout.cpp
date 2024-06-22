#include "Layout/MapTerrainLayout.h"

#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"

#include "System/MapDataHolder.h"

MapTerrainLayout::MapTerrainLayout(const char* name) : al::LayoutActor(name) {}

bool MapTerrainLayout::tryChangePrintWorld(s32 worldId) {
    if (worldId < 0)
        return false;

    mMapData = rs::findMapData(this, worldId);
    if (!mMapData)
        return false;

    al::setPaneTexture(this, "PicMap00", mMapData->mTexture2dMap);
    al::setPaneLocalScale(this, "PicMap00", sead::Vector2f(256.0f, 256.0f));

    return true;
}

f32 MapTerrainLayout::getPaneSize() const {
    return 2048.0f;
}
