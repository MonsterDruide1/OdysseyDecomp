#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>

class GameDataHolder;

namespace nn::ui2d {
class TextureInfo;
}

namespace al {
class Resource;
class IUseSceneObjHolder;
}  // namespace al

struct MapData {
    sead::Matrix44f mViewProjMatrix;
    sead::Matrix34f mViewMatrix;
    sead::Matrix44f mProjMatrix;
    nn::ui2d::TextureInfo* mTexture2dMap;
    void* field_B8;  // TODO unknown type
    s32 mPartsNum;
    void** mParts;  // TODO unknown type
    s32 mWorldScenarioNum;
    sead::PtrArray<MapData> mScenarioMapData;
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
