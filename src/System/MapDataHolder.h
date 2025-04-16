#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>

class GameDataHolder;

namespace nn::ui2d {
class TextureInfo;
}

namespace al {
class IUseSceneObjHolder;
class Resource;
}  // namespace al

struct MapData {
    sead::Matrix44f viewProjMatrix;
    sead::Matrix34f viewMatrix;
    sead::Matrix44f projMatrix;
    nn::ui2d::TextureInfo* texture2dMap;
    void* field_b8;  // TODO unknown type
    s32 partsNum;
    void** parts;  // TODO unknown type
    s32 worldScenarioNum;
    sead::PtrArray<MapData> scenarioMapData;
};

class MapDataHolder {
public:
    MapDataHolder(const GameDataHolder*);
    MapData* loadMapData(al::Resource*, const char*, s32);
    MapData* tryLoadMapData(al::Resource*, const char*, s32);
    const sead::Matrix34f& findViewMtx(s32) const;
    const MapData& findMapData(s32) const;

private:
    sead::PtrArray<MapData> mMapDatas;
    const GameDataHolder* mGameDataHolder;
};

namespace rs {
MapData* findMapData(al::IUseSceneObjHolder*, s32);
}
