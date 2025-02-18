#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace sead {
class Color4f;
}

namespace al {
class LiveActor;
class ShadowMaskBase;
class DepthShadowMapInfo;
class OccSphere;

enum class ShadowMaskDrawCategory : s32 {};

bool isExistShadow(LiveActor* actor);
bool isExistShadowMaskCtrl(LiveActor* actor);
bool isExistShadowMask(LiveActor* actor, const char* maskName);
bool isExistDepthShadow(LiveActor* actor);
bool isExistDepthShadowMapCtrl(LiveActor* actor);
bool isExistAnyShadowCtrl(LiveActor* actor);
bool isHideShadowMask(const LiveActor* actor);
void hideShadow(LiveActor* actor);
void showShadow(LiveActor* actor);
void showShadowMask(LiveActor* actor);
void showShadowMask(LiveActor* actor, ShadowMaskBase* mask);
void showShadowMask(LiveActor* actor, const char* maskName);
void showShadowMask(LiveActor* actor, ShadowMaskDrawCategory drawCategory);
void hideShadowMask(LiveActor* actor);
void hideShadowMask(LiveActor* actor, ShadowMaskBase* mask);
void hideShadowMask(LiveActor* actor, const char* maskName);
void hideShadowMask(LiveActor* actor, ShadowMaskDrawCategory drawCategory);
bool isHideShadowMask(LiveActor* actor, const char* maskName);
void showDepthShadowMap(LiveActor* actor, s32 mapIdx);
void showDepthShadowMap(const LiveActor* actor, DepthShadowMapInfo* map);
void showDepthShadowMap(LiveActor* actor, const char* mapName);
void hideDepthShadowMap(LiveActor* actor, s32 mapIdx);
void hideDepthShadowMap(const LiveActor* actor, DepthShadowMapInfo* map);
void hideDepthShadowMap(LiveActor* actor, const char* mapName);
bool isHideDepthShadowMap(LiveActor* actor, const char* mapName);
bool isHideDepthShadowMap(LiveActor* actor, s32 mapIdx);
s32 getDepthShadowMapNum(LiveActor* actor);
void validateShadowMask(LiveActor* actor);
void validateShadowMask(LiveActor* actor, ShadowMaskBase* mask);
void validateShadowMask(LiveActor* actor, const char* maskName);
void invalidateShadowMask(LiveActor* actor);
void invalidateShadowMask(LiveActor* actor, ShadowMaskBase* mask);
void invalidateShadowMask(LiveActor* actor, const char* maskName);
void validateDepthShadowMap(LiveActor* actor);
void validateDepthShadowMap(LiveActor* actor, DepthShadowMapInfo* map);
void validateDepthShadowMap(LiveActor* actor, const char* mapName);
void invalidateDepthShadowMap(LiveActor* actor);
void invalidateDepthShadowMap(LiveActor* actor, DepthShadowMapInfo* map);
void invalidateDepthShadowMap(LiveActor* actor, const char* mapName);
void invalidateShadowMask(LiveActor* actor, ShadowMaskDrawCategory drawCategory);
void invalidateShadowMaskIntensityAll(LiveActor* actor);
void setShadowMaskFixed(LiveActor* actor, bool isFixed);
void setShadowMaskDropDir(LiveActor* actor, const sead::Vector3f& dropDir);
void setShadowMaskDropDir(LiveActor* actor, const sead::Vector3f& dropDir, const char* maskName);
void setShadowMaskDropDirActorDown(LiveActor* actor);
void setShadowMaskSize(LiveActor* actor, const char* maskName, const sead::Vector3f& size);
void setShadowMaskSize(LiveActor* actor, const char* maskName, f32 sizeX, f32 sizeY, f32 sizeZ);
void setShadowTextureOffset(LiveActor* actor, const char* maskName, f32 offsetX, f32 offsetY);
void onShadowTextureScroll(LiveActor* actor, const char* maskName);
void offShadowTextureScroll(LiveActor* actor, const char* maskName);
void calcShadowMaskSize(sead::Vector3f* size, LiveActor* actor, const char* maskName);
f32 getShadowMaskDropLength(const LiveActor* actor, const char* maskName);
void setShadowMaskDropLength(LiveActor* actor, f32 dropLength);
void setShadowMaskDropLength(LiveActor* actor, f32 dropLength, const char* maskName);
void setShadowMaskDropLengthScaleWithDrawCategory(LiveActor* actor, f32 dropLengthScale,
                                                  ShadowMaskDrawCategory drawCategory);
void setShadowMaskDropLengthWithDrawCategory(LiveActor* actor, f32 dropLength,
                                             ShadowMaskDrawCategory drawCategory);
void setShadowMaskDropLengthEvenWithTarget(ShadowMaskBase* targetMask, const char* maskName,
                                           const sead::Vector3f&);
void setShadowMaskDropLengthEvenWithTarget(ShadowMaskBase* targetMask, const ShadowMaskBase* mask,
                                           const sead::Vector3f&);
void setShadowMaskDropLengthEvenWithDrawCategory(LiveActor*, ShadowMaskDrawCategory drawCategory,
                                                 const LiveActor*, const char* maskName);
void setShadowMaskDropLengthEvenPlaneNormal(const LiveActor* actor, const sead::Vector3f&);
s32 getDepthShadowMapWidth(const LiveActor* actor, const char* mapName);
s32 getDepthShadowMapHeight(const LiveActor* actor, const char* mapName);
void setDepthShadowMapSize(const LiveActor* actor, s32 width, s32 height, const char* mapName);
f32 getDepthShadowMapLength(const LiveActor* actor, const char* mapName);
void setDepthShadowMapLength(const LiveActor* actor, f32 length, const char* mapName);
void setDepthShadowMapLengthFromActorTransFlag(const LiveActor* actor, bool isSetShadowLength,
                                               const char* mapName);
void setDepthShadowMapBoundingBox(const LiveActor* actor, const sead::Vector3f& boundMin,
                                  const sead::Vector3f& boundMax, const char* mapName);
void setDepthShadowMapMaskTypeNone(const LiveActor* actor, const char* mapName);
void setDepthShadowMapMaskTypeSelf(const LiveActor* actor, const char* mapName);
void setEnableDepthShadowMapBottomGradation(const LiveActor* actor, const char* mapName,
                                            bool isEnableBottomGradation);
bool isEnableDepthShadowMapBottomGradation(const LiveActor* actor, const char* mapName);
void setDepthShadowMapBottomGradationLength(const LiveActor* actor, const char* mapName,
                                            f32 length);
f32 getDepthShadowMapBottomGradationLength(const LiveActor* actor, const char* mapName);
bool isAppendSubActorDepthShadowMap(const LiveActor* actor);
f32 getShadowMaskDropLengthMax(const LiveActor* actor);
void setShadowMaskColor(const LiveActor* actor, const char* maskName, const sead::Color4f& color);
const sead::Color4f& getShadowMaskColor(const LiveActor* actor, const char* maskName);
void setShadowMaskIntensity(const LiveActor* actor, const char* maskName, f32 intensity);
f32 getShadowMaskIntensity(const LiveActor* actor, const char* maskName);
f32 getShadowMaskTextureFixedScale(const LiveActor* actor, const char* maskName);
void setShadowMaskTextureFixedScale(const LiveActor* actor, const char* maskName, f32 scale);
const sead::Vector3f& getShadowMaskOffset(const LiveActor* actor, const char* maskName);
void setShadowMaskOffset(const LiveActor* actor, const sead::Vector3f& offset,
                         const char* maskName);
bool isExistOcclusionLightPosPtr(const LiveActor* actor, const char* occSphereName);
void setOcclusionLightPosPtr(const LiveActor* actor, const char* occSphereName,
                             const sead::Vector3f* lightPosPtr);
void setOcclusionAddOffset(const LiveActor* actor, const char* occSphereName,
                           const sead::Vector3f& offset);
void enableOcclusion(const LiveActor* actor, const char* occSphereName);
void disableOcclusion(const LiveActor* actor, const char* occSphereName);
bool isEnableOcclusion(const LiveActor* actor, const char* occSphereName);
void calcSphereDoBoundingInfo(sead::Vector3f*, sead::Vector3f*, f32*, f32*, const LiveActor* actor,
                              const char* occSphereName, f32);
void calcOcclusionSpherePos(sead::Vector3f* pos, const LiveActor* actor, const char* occSphereName);
void changeOcclusionGroup(const LiveActor* actor, const char* occSphereName, const char* groupName);
OccSphere* getOcclusionSphere(const LiveActor* actor, const char* occSphereName);
void setOcclusionIgnoreHostHide(const LiveActor* actor, bool isIgnoreHostHide);
bool findIsInShade(const LiveActor* actor, const sead::Vector3f& pos);
void requestDepthShadowMapLightDir(const LiveActor* actor, const sead::Vector3f& lightDir,
                                   const char* mapName);
void resetRequestDepthShadowMapLightDir(const LiveActor* actor, const char* mapName);
bool isRequestDepthShadowMapLightDir(const LiveActor* actor, const char* mapName);
void onDepthShadowModel(LiveActor* actor);
void offDepthShadowModel(LiveActor* actor);
void updateDepthShadowMapCtrlShapeVisible(LiveActor* actor);
}  // namespace al
