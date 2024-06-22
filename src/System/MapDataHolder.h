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
    void* field_B8;
    s32 mPartsNum;
    void** mParts;
    s32 mWorldScenarioNum;
    sead::PtrArray<MapData> mScenarioMapData;
};

class MapDataHolder {
public:
    MapDataHolder(const GameDataHolder*);
    void loadMapData(al::Resource*, const char*, s32);
    bool tryLoadMapData(al::Resource*, const char*, s32);
    void findViewMtx(s32);
    void findMapData(s32);

private:
    sead::PtrArray<MapData> mMapDatas;
    const GameDataHolder* mGameDataHolder;
};

namespace rs {
MapData* findMapData(al::IUseSceneObjHolder*, s32);
}
