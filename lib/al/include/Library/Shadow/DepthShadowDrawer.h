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

bool isExistShadow(al::LiveActor*);
bool isExistShadowMaskCtrl(al::LiveActor*);
bool isExistShadowMask(al::LiveActor*, const char*);
bool isExistDepthShadow(al::LiveActor*);
bool isExistDepthShadowMapCtrl(al::LiveActor*);
bool isExistAnyShadowCtrl(al::LiveActor*);
bool isHideShadowMask(const al::LiveActor*);
void hideShadow(al::LiveActor*);
void showShadow(al::LiveActor*);
void showShadowMask(al::LiveActor*);
void showShadowMask(al::LiveActor*, al::ShadowMaskBase*);
void showShadowMask(al::LiveActor*, const char*);
void showShadowMask(al::LiveActor*, al::ShadowMaskDrawCategory);
void hideShadowMask(al::LiveActor*);
void hideShadowMask(al::LiveActor*, al::ShadowMaskBase*);
void hideShadowMask(al::LiveActor*, const char*);
void hideShadowMask(al::LiveActor*, al::ShadowMaskDrawCategory);
bool isHideShadowMask(al::LiveActor*, const char*);
void showDepthShadowMap(al::LiveActor*, s32);
void showDepthShadowMap(const al::LiveActor*, al::DepthShadowMapInfo*);
void showDepthShadowMap(al::LiveActor*, const char*);
void hideDepthShadowMap(al::LiveActor*, s32);
void hideDepthShadowMap(const al::LiveActor*, al::DepthShadowMapInfo*);
void hideDepthShadowMap(al::LiveActor*, const char*);
bool isHideDepthShadowMap(al::LiveActor*, const char*);
bool isHideDepthShadowMap(al::LiveActor*, s32);
void getDepthShadowMapNum(al::LiveActor*);
void validateShadowMask(al::LiveActor*);
void validateShadowMask(al::LiveActor*, al::ShadowMaskBase*);
void validateShadowMask(al::LiveActor*, const char*);
void invalidateShadowMask(al::LiveActor*);
void invalidateShadowMask(al::LiveActor*, al::ShadowMaskBase*);
void invalidateShadowMask(al::LiveActor*, const char*);
void validateDepthShadowMap(al::LiveActor*);
void validateDepthShadowMap(al::LiveActor*, al::DepthShadowMapInfo*);
void validateDepthShadowMap(al::LiveActor*, const char*);
void invalidateDepthShadowMap(al::LiveActor*);
void invalidateDepthShadowMap(al::LiveActor*, al::DepthShadowMapInfo*);
void invalidateDepthShadowMap(al::LiveActor*, const char*);
void invalidateShadowMask(al::LiveActor*, al::ShadowMaskDrawCategory);
void invalidateShadowMaskIntensityAll(al::LiveActor*);
void setShadowMaskFixed(al::LiveActor*, bool);
void setShadowMaskDropDir(al::LiveActor*, const sead::Vector3f&);
void setShadowMaskDropDir(al::LiveActor*, const sead::Vector3f&, const char*);
void setShadowMaskDropDirActorDown(al::LiveActor*);
void setShadowMaskSize(al::LiveActor*, const char*, const sead::Vector3f&);
void setShadowMaskSize(al::LiveActor*, const char*, f32, f32, f32);
void setShadowTextureOffset(al::LiveActor*, const char*, f32, f32);
void onShadowTextureScroll(al::LiveActor*, const char*);
void offShadowTextureScroll(al::LiveActor*, const char*);
void calcShadowMaskSize(sead::Vector3f*, al::LiveActor*, const char*);
f32 getShadowMaskDropLength(const al::LiveActor*, const char*);
void setShadowMaskDropLength(al::LiveActor*, f32);
void setShadowMaskDropLength(al::LiveActor*, f32, const char*);
void setShadowMaskDropLengthScaleWithDrawCategory(al::LiveActor*, f32, al::ShadowMaskDrawCategory);
void setShadowMaskDropLengthWithDrawCategory(al::LiveActor*, f32, al::ShadowMaskDrawCategory);
void setShadowMaskDropLengthEvenWithTarget(al::ShadowMaskBase*, const char*, const sead::Vector3f&);
void setShadowMaskDropLengthEvenWithTarget(al::ShadowMaskBase*, al::ShadowMaskBase const*,
                                           const sead::Vector3f&);
void setShadowMaskDropLengthEvenWithDrawCategory(al::LiveActor*, al::ShadowMaskDrawCategory,
                                                 const al::LiveActor*, const char*);
void setShadowMaskDropLengthEvenPlaneNormal(const al::LiveActor*, const sead::Vector3f&);
s32 getDepthShadowMapWidth(const al::LiveActor*, const char*);
s32 getDepthShadowMapHeight(const al::LiveActor*, const char*);
void setDepthShadowMapSize(const al::LiveActor*, s32, s32, const char*);
s32 getDepthShadowMapLength(const al::LiveActor*, const char*);
void setDepthShadowMapLength(const al::LiveActor*, f32, const char*);
void setDepthShadowMapLengthFromActorTransFlag(const al::LiveActor*, bool, const char*);
void setDepthShadowMapBoundingBox(const al::LiveActor*, const sead::Vector3f&,
                                  const sead::Vector3f&, const char*);
void setDepthShadowMapMaskTypeNone(const al::LiveActor*, const char*);
void setDepthShadowMapMaskTypeSelf(const al::LiveActor*, const char*);
void setEnableDepthShadowMapBottomGradation(const al::LiveActor*, const char*, bool);
bool isEnableDepthShadowMapBottomGradation(const al::LiveActor*, const char*);
void setDepthShadowMapBottomGradationLength(const al::LiveActor*, const char*, f32);
f32 getDepthShadowMapBottomGradationLength(const al::LiveActor*, const char*);
bool isAppendSubActorDepthShadowMap(const al::LiveActor*);
f32 getShadowMaskDropLengthMax(const al::LiveActor*);
void setShadowMaskColor(const al::LiveActor*, const char*, const sead::Color4f&);
sead::Color4f& getShadowMaskColor(const al::LiveActor*, const char*);
void setShadowMaskIntensity(const al::LiveActor*, const char*, f32);
f32 getShadowMaskIntensity(const al::LiveActor*, const char*);
void* getShadowMaskTextureFixedScale(const al::LiveActor*, const char*);
void setShadowMaskTextureFixedScale(const al::LiveActor*, const char*, f32);
sead::Vector3f& getShadowMaskOffset(const al::LiveActor*, const char*);
void setShadowMaskOffset(const al::LiveActor*, const sead::Vector3f&, const char*);
bool isExistOcclusionLightPosPtr(const al::LiveActor*, const char*);
void setOcclusionLightPosPtr(const al::LiveActor*, const char*, const sead::Vector3f*);
void setOcclusionAddOffset(const al::LiveActor*, const char*, const sead::Vector3f&);
void enableOcclusion(const al::LiveActor*, const char*);
void disableOcclusion(const al::LiveActor*, const char*);
bool isEnableOcclusion(const al::LiveActor*, const char*);
void calcSphereDoBoundingInfo(sead::Vector3f*, sead::Vector3f*, f32*, f32*, const al::LiveActor*,
                              const char*, f32);
void calcOcclusionSpherePos(sead::Vector3f*, const al::LiveActor*, const char*);
void changeOcclusionGroup(const al::LiveActor*, const char*, const char*);
al::OccSphere* getOcclusionSphere(const al::LiveActor*, const char*);
void setOcclusionIgnoreHostHide(const al::LiveActor*, bool);
bool findIsInShade(const al::LiveActor*, const sead::Vector3f&);
void requestDepthShadowMapLightDir(const al::LiveActor*, const sead::Vector3f&, const char*);
void resetRequestDepthShadowMapLightDir(const al::LiveActor*, const char*);
bool isRequestDepthShadowMapLightDir(const al::LiveActor*, const char*);
void onDepthShadowModel(al::LiveActor*);
void offDepthShadowModel(al::LiveActor*);
void updateDepthShadowMapCtrlShapeVisible(al::LiveActor*);
}  // namespace al
