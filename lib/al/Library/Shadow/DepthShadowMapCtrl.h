#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>

namespace al {

class Resource;
class ByamlIter;
class GraphicsSystemInfo;
class LiveActor;
class DepthShadowMapInfo;
class ModelDrawerDepthShadowMap;
class ModelDrawerMask;

class DepthShadowMapCtrl {
public:
    DepthShadowMapCtrl(const al::Resource* resource);
    ~DepthShadowMapCtrl();

    void actorModelDrawDepth();
    void actorModelDrawMask();
    void appendDepthShadowMapInfo(const char*, s32, s32, s32, f32, bool, const sead::Vector3f&,
                                  bool, const sead::Vector3f&, const sead::Vector3f&, bool,
                                  const char*, s32, bool, f32, f32, f32, bool, bool, f32, s32, bool,
                                  bool, f32);
    DepthShadowMapInfo* getDepthShadowMapInfo(s32 index);
    u32 getDepthShadowMapNum();
    void hide();
    void init(al::LiveActor* actor, const al::ByamlIter& iter);
    void initAfterPlacement(al::GraphicsSystemInfo* graphicsSystemInfo);
    void initWithoutIter(al::LiveActor* actor, s32);
    void show();
    DepthShadowMapInfo* tryFindDepthShadowMapInfo(const char*);
    void update();
    void updateShapeVisible(const al::LiveActor* actor);

private:
    al::LiveActor* mLiveActor;
    sead::Vector3f lightDir;
    bool mIsAppendSubActor;
    sead::PtrArray<DepthShadowMapInfo> mDepthShadowMaps;
    sead::PtrArray<ModelDrawerDepthShadowMap> mModelDrawerDepthShadowMaps;
    sead::PtrArray<ModelDrawerMask> mModelDrawerMasks;
};

}  // namespace al
