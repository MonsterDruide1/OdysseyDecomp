#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class DepthShadowMapInfo;
class OccSphere;
class ShadowMaskBase;
class ShadowMaskDrawCategory;

bool isExistShadow(LiveActor*);
bool isExistShadowMaskCtrl(LiveActor*);
bool isExistShadowMask(LiveActor*, const char*);
bool isExistDepthShadow(LiveActor*);
bool isExistDepthShadowMapCtrl(LiveActor*);
bool isExistAnyShadowCtrl(LiveActor*);
bool isHideShadowMask(const LiveActor*);
void hideShadow(LiveActor*);
void showShadow(LiveActor*);
void showShadowMask(LiveActor*);
void showShadowMask(LiveActor*, ShadowMaskBase*);
void showShadowMask(LiveActor*, const char*);
void showShadowMask(LiveActor*, ShadowMaskDrawCategory);
void hideShadowMask(LiveActor*);
void hideShadowMask(LiveActor*, ShadowMaskBase*);
void hideShadowMask(LiveActor*, const char*);
void hideShadowMask(LiveActor*, ShadowMaskDrawCategory);
bool isHideShadowMask(LiveActor*, const char*);
void showDepthShadowMap(LiveActor*, s32);
void showDepthShadowMap(const LiveActor*, DepthShadowMapInfo*);
void showDepthShadowMap(LiveActor*, const char*);
void hideDepthShadowMap(LiveActor*, s32);
void hideDepthShadowMap(const LiveActor*, DepthShadowMapInfo*);
void hideDepthShadowMap(LiveActor*, const char*);
bool isHideDepthShadowMap(LiveActor*, const char*);
bool isHideDepthShadowMap(LiveActor*, s32);
void getDepthShadowMapNum(LiveActor*);
void validateShadowMask(LiveActor*);
void validateShadowMask(LiveActor*, ShadowMaskBase*);
void validateShadowMask(LiveActor*, const char*);
void invalidateShadowMask(LiveActor*);
void invalidateShadowMask(LiveActor*, ShadowMaskBase*);
void invalidateShadowMask(LiveActor*, const char*);
void validateDepthShadowMap(LiveActor*);
void validateDepthShadowMap(LiveActor*, DepthShadowMapInfo*);
void validateDepthShadowMap(LiveActor*, const char*);
void invalidateDepthShadowMap(LiveActor*);
void invalidateDepthShadowMap(LiveActor*, DepthShadowMapInfo*);
void invalidateDepthShadowMap(LiveActor*, const char*);
void invalidateShadowMask(LiveActor*, ShadowMaskDrawCategory);
void invalidateShadowMaskIntensityAll(LiveActor*);
void setShadowMaskFixed(LiveActor*, bool);
void setShadowMaskDropDir(LiveActor*, const sead::Vector3f&);
void setShadowMaskDropDir(LiveActor*, const sead::Vector3f&, const char*);
void setShadowMaskDropDirActorDown(LiveActor*);
void setShadowMaskSize(LiveActor*, const char*, const sead::Vector3f&);
void setShadowMaskSize(LiveActor*, const char*, f32, f32, f32);
void setShadowTextureOffset(LiveActor*, const char*, f32, f32);
void onShadowTextureScroll(LiveActor*, const char*);
void offShadowTextureScroll(LiveActor*, const char*);
void calcShadowMaskSize(sead::Vector3f*, LiveActor*, const char*);
f32 getShadowMaskDropLength(const LiveActor*, const char*);
void setShadowMaskDropLength(LiveActor*, f32);
void setShadowMaskDropLength(LiveActor*, f32, const char*);
void setShadowMaskDropLengthScaleWithDrawCategory(LiveActor*, f32, ShadowMaskDrawCategory);
void setShadowMaskDropLengthWithDrawCategory(LiveActor*, f32, ShadowMaskDrawCategory);
void setShadowMaskDropLengthEvenWithTarget(ShadowMaskBase*, const char*, const sead::Vector3f&);
void setShadowMaskDropLengthEvenWithTarget(ShadowMaskBase*, const ShadowMaskBase*,
                                           const sead::Vector3f&);
void setShadowMaskDropLengthEvenWithDrawCategory(LiveActor*, ShadowMaskDrawCategory,
                                                 const LiveActor*, const char*);
void setShadowMaskDropLengthEvenPlaneNormal(const LiveActor*, const sead::Vector3f&);
s32 getDepthShadowMapWidth(const LiveActor*, const char*);
s32 getDepthShadowMapHeight(const LiveActor*, const char*);
void setDepthShadowMapSize(const LiveActor*, s32, s32, const char*);
s32 getDepthShadowMapLength(const LiveActor*, const char*);
void setDepthShadowMapLength(const LiveActor*, f32, const char*);
void setDepthShadowMapLengthFromActorTransFlag(const LiveActor*, bool, const char*);
void setDepthShadowMapBoundingBox(const LiveActor*, const sead::Vector3f&, const sead::Vector3f&,
                                  const char*);
void setDepthShadowMapMaskTypeNone(const LiveActor*, const char*);
void setDepthShadowMapMaskTypeSelf(const LiveActor*, const char*);
void setEnableDepthShadowMapBottomGradation(const LiveActor*, const char*, bool);
bool isEnableDepthShadowMapBottomGradation(const LiveActor*, const char*);
void setDepthShadowMapBottomGradationLength(const LiveActor*, const char*, f32);
f32 getDepthShadowMapBottomGradationLength(const LiveActor*, const char*);
bool isAppendSubActorDepthShadowMap(const LiveActor*);
f32 getShadowMaskDropLengthMax(const LiveActor*);
void setShadowMaskColor(const LiveActor*, const char*, const sead::Color4f&);
const sead::Color4f& getShadowMaskColor(const LiveActor*, const char*);
void setShadowMaskIntensity(const LiveActor*, const char*, f32);
f32 getShadowMaskIntensity(const LiveActor*, const char*);
f32 getShadowMaskTextureFixedScale(const LiveActor*, const char*);
void setShadowMaskTextureFixedScale(const LiveActor*, const char*, f32);
const sead::Vector3f& getShadowMaskOffset(const LiveActor*, const char*);
void setShadowMaskOffset(const LiveActor*, const sead::Vector3f&, const char*);
bool isExistOcclusionLightPosPtr(const LiveActor*, const char*);
void setOcclusionLightPosPtr(const LiveActor*, const char*, const sead::Vector3f*);
void setOcclusionAddOffset(const LiveActor*, const char*, const sead::Vector3f&);
void enableOcclusion(const LiveActor*, const char*);
void disableOcclusion(const LiveActor*, const char*);
bool isEnableOcclusion(const LiveActor*, const char*);
void calcSphereDoBoundingInfo(sead::Vector3f*, sead::Vector3f*, f32*, f32*, const LiveActor*,
                              const char*, f32);
void calcOcclusionSpherePos(sead::Vector3f*, const LiveActor*, const char*);
void changeOcclusionGroup(const LiveActor*, const char*, const char*);
OccSphere* getOcclusionSphere(const LiveActor*, const char*);
void setOcclusionIgnoreHostHide(const LiveActor*, bool);
bool findIsInShade(const LiveActor*, const sead::Vector3f&);
void requestDepthShadowMapLightDir(const LiveActor*, const sead::Vector3f&, const char*);
void resetRequestDepthShadowMapLightDir(const LiveActor*, const char*);
bool isRequestDepthShadowMapLightDir(const LiveActor*, const char*);
void onDepthShadowModel(LiveActor*);
void offDepthShadowModel(LiveActor*);
void updateDepthShadowMapCtrlShapeVisible(LiveActor*);
}  // namespace al
