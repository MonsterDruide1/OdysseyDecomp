#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

namespace al {

class Resource;
class ByamlIter;
class GraphicsSystemInfo;
class LiveActor;
class DepthShadowMapCtrl;
class DepthShadowMapInfo;
class ModelDrawerDepthShadowMap;
class ModelDrawerMask;

class DepthShadowMapInfo {
public:
    DepthShadowMapInfo(const char*);

    void setShadowTextureSize(s32, s32);
    DepthShadowMapCtrl* getDepthShadowMapCtrl();

    const char* getName() const { return mName; }

    s32 getWidth() const { return mShadowTextureWidth; }

    s32 getHeight() const { return mShadowTextureHeight; }

    f32 getLength() const { return mLength; }

    void setLength(f32 length) { mLength = length; }

    void setLengthFromActorTrans(bool isLengthFromActorTrans) {
        mIsLengthFromActorTrans = isLengthFromActorTrans;
    }

    void setLightDir(const sead::Vector3f& lightDir) { mLightDir.set(lightDir); }

    void setRequestLightDir(bool isRequest) { mIsRequestLightDir = isRequest; }

    bool isRequestLightDir() const { return mIsRequestLightDir; }

    void setBoundingBox(const sead::Vector3f& boundMin, const sead::Vector3f& boundMax) {
        mBoundingBoxMin.x = boundMin.x;
        mBoundingBoxMin.y = boundMin.y;
        mBoundingBoxMin.z = boundMin.z;
        mBoundingBoxMax.x = boundMax.x;
        mBoundingBoxMax.y = boundMax.y;
        mBoundingBoxMax.z = boundMax.z;
    }

    void setMaskType(s32 maskType) { mMaskType = maskType; }

    void setEnableBottomGradation(bool isEnable) { mIsEnableBottomGradation = isEnable; }

    bool isEnableBottomGradation() const { return mIsEnableBottomGradation; }

    void setBottomGradationLength(f32 length) { mBottomGradationLength = length; }

    f32 getBottomGradationLength() const { return mBottomGradationLength; }

    bool isHidden() const { return mIsHidden; }

    void setHidden(bool isHidden) { mIsHidden = isHidden; }

    bool isValid() const { return mIsValid; }

    void setValid(bool isValid) { mIsValid = isValid; }

private:
    const char* mName;
    s32 mShadowTextureWidth;
    s32 mShadowTextureHeight;
    s32 mBlurIteration;
    f32 mLength;
    bool mIsLengthFromActorTrans;
    u8 _19[3];
    sead::Vector3f mLightDir;
    bool mIsRequestLightDir;
    u8 _29[3];
    sead::Vector3f mBoundingBoxMin;
    sead::Vector3f mBoundingBoxMax;
    s32 mMaskType;
    bool _48;
    u8 _49[3];
    f32 _4c;
    f32 _50;
    f32 _54;
    f32 _58;
    s32 _5c;
    s32 _60;
    u16 _64;
    u8 _66[2];
    void* _68;
    const char* mJointName;
    bool mIsEnableBottomGradation;
    u8 _79[3];
    f32 mBottomGradationLength;
    bool mIsHidden;
    bool mIsValid;
    u8 _82[2];
    f32 _84;
    bool _88;
    u8 _89[7];
    const LiveActor* mHostActor;
    void* mShadowTextureInfo;
};

static_assert(sizeof(DepthShadowMapInfo) == 0xa0);

class DepthShadowMapCtrl : public HioNode {
public:
    DepthShadowMapCtrl(const Resource* resource);
    virtual ~DepthShadowMapCtrl();

    void actorModelDrawDepth() const;
    void actorModelDrawMask() const;
    void appendDepthShadowMapInfo(const char*, s32, s32, s32, f32, bool, const sead::Vector3f&,
                                  bool, const sead::Vector3f&, const sead::Vector3f&, bool,
                                  const char*, s32, bool, f32, f32, f32, bool, bool, f32, s32, bool,
                                  bool, f32);
    DepthShadowMapInfo* getDepthShadowMapInfo(s32 index) const;
    s32 getDepthShadowMapNum() const;
    void hide();
    void init(LiveActor* actor, const ByamlIter& iter);
    void initAfterPlacement(GraphicsSystemInfo* graphicsSystemInfo);
    void initWithoutIter(LiveActor* actor, s32);
    void show();
    DepthShadowMapInfo* tryFindDepthShadowMapInfo(const char*) const;
    void update();
    void updateShapeVisible(const LiveActor* actor);

    void setAppendSubActor(bool isAppendSubActor) { mIsAppendSubActor = isAppendSubActor; }

    bool isAppendSubActor() const { return mIsAppendSubActor; }

private:
    LiveActor* mLiveActor;
    sead::Vector3f mLightDir;
    bool mIsAppendSubActor;
    sead::PtrArray<DepthShadowMapInfo> mDepthShadowMaps;
    sead::PtrArray<ModelDrawerDepthShadowMap> mModelDrawerDepthShadowMaps;
    sead::PtrArray<ModelDrawerMask> mModelDrawerMasks;
};

static_assert(sizeof(DepthShadowMapCtrl) == 0x50);

}  // namespace al
