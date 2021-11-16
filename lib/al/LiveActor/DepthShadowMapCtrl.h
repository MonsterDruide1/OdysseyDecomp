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
    DepthShadowMapCtrl(const al::Resource*);
    ~DepthShadowMapCtrl();

    void actorModelDrawDepth();
    void actorModelDrawMask();
    void appendDepthShadowMapInfo(char const*, int, int, int, float, bool,
                                  sead::Vector3<float> const&, bool, sead::Vector3<float> const&,
                                  sead::Vector3<float> const&, bool, char const*, int, bool, float,
                                  float, float, bool, bool, float, int, bool, bool, float);
    DepthShadowMapInfo* getDepthShadowMapInfo(int);
    unsigned int getDepthShadowMapNum();
    void hide();
    void init(al::LiveActor*, const al::ByamlIter&);
    void initAfterPlacement(al::GraphicsSystemInfo*);
    void initWithoutIter(al::LiveActor*, int);
    void show();
    DepthShadowMapInfo* tryFindDepthShadowMapInfo(const char*);
    void update();
    void updateShapeVisible(const al::LiveActor*);

private:
    al::LiveActor* mLiveActor;
    sead::Vector3<float> lightDir;
    bool mIsAppendSubActor;
    sead::PtrArray<DepthShadowMapInfo> mDepthShadowMaps;
    sead::PtrArray<ModelDrawerDepthShadowMap> mModelDrawerDepthShadowMaps;
    sead::PtrArray<ModelDrawerMask> mModelDrawerMasks;
};

}  // namespace al