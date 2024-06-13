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
    DepthShadowMapCtrl(const Resource* resource);
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
    void init(LiveActor* actor, const ByamlIter& iter);
    void initAfterPlacement(GraphicsSystemInfo* graphicsSystemInfo);
    void initWithoutIter(LiveActor* actor, s32);
    void show();
    DepthShadowMapInfo* tryFindDepthShadowMapInfo(const char*);
    void update();
    void updateShapeVisible(const LiveActor* actor);

private:
    LiveActor* mLiveActor;
    sead::Vector3f mLightDir;
    bool mIsAppendSubActor;
    sead::PtrArray<DepthShadowMapInfo> mDepthShadowMaps;
    sead::PtrArray<ModelDrawerDepthShadowMap> mModelDrawerDepthShadowMaps;
    sead::PtrArray<ModelDrawerMask> mModelDrawerMasks;
};

}  // namespace al
