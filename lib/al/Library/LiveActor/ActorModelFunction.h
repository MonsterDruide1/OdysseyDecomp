#pragma once

#include <gfx/seadColor.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <nn/g3d/ResFile.h>
#include <nn/gfx/gfx_DescriptorSlot.h>

namespace agl {
class UniformBlock;
}

namespace nn::gfx {
class ResTexture;
}

namespace al {
class LayoutTextureRenderObj;
class LiveActor;
class ClippingDirector;

void updateMaterialCodeGround(LiveActor* actor, const char*);
void updateMaterialCodeAll(LiveActor* actor);
void updateMaterialCodeArea(LiveActor* actor);
void updateMaterialCodeWet(LiveActor* actor, bool);
void updateMaterialCodeWater(LiveActor* actor);
void updateMaterialCodeWater(LiveActor* actor, bool);
void updateMaterialCodePuddle(LiveActor* actor);
void updateMaterialCodePuddle(LiveActor* actor, bool);
void resetMaterialCode(LiveActor* actor);
void showModel(LiveActor* actor);
void showModelIfHide(LiveActor* actor);
bool isHideModel(const LiveActor* actor);
void hideModel(LiveActor* actor);
void hideModelIfShow(LiveActor* actor);
bool isExistModel(const LiveActor* actor);
void switchShowHideModelIfNearCamera(LiveActor* actor, f32);
bool blinkModel(LiveActor* actor, s32, s32, s32);
void calcViewModelSystem(LiveActor* actor);
void calcViewModel(LiveActor* actor);
void showSilhouetteModel(LiveActor* actor);
void hideSilhouetteModel(LiveActor* actor);
bool isSilhouetteModelHidden(const LiveActor* actor);
void showSilhouetteModelIfHide(LiveActor* actor);
void hideSilhouetteModelIfShow(LiveActor* actor);
void setModelAlphaMask(LiveActor* actor, f32);
f32 getModelAlphaMask(const LiveActor* actor);
bool isModelAlphaMask(const LiveActor* actor);
void updateModelAlphaMaskCameraDistance(LiveActor* actor, f32, f32, f32, f32);
bool isExistZPrePass(const LiveActor* actor);
bool isEnableZPrePass(const LiveActor* actor);
void validateZPrePass(LiveActor* actor);
void invalidateZPrePass(LiveActor* actor);
void invalidateOcclusionQuery(LiveActor* actor);
void validateOcclusionQuery(LiveActor* actor);
bool isValidOcclusionQuery(const LiveActor* actor);
void setFixedModelFlag(LiveActor* actor);
void tryInitFixedModelGpuBuffer(LiveActor* actor);
void setIgnoreUpdateDrawClipping(LiveActor* actor, bool value);
void setNeedSetBaseMtxAndCalcAnimFlag(LiveActor* actor, bool value);
bool isViewDependentModel(const LiveActor* actor);
bool isNeedUpdateModel(const LiveActor* actor);
void setEnvTextureMirror(LiveActor* actor, s32);
void setEnvTextureProc3D(LiveActor* actor, s32);
void forceApplyCubeMap(LiveActor* actor, const char* cubeMapName);
void setMaterialProgrammable(LiveActor* actor);
bool isIncludePrepassCullingShape(LiveActor* actor);
bool isExistJoint(const LiveActor* actor, const char*);
s32 getJointIndex(const LiveActor* actor, const char*);
sead::Matrix34f* getJointMtxPtr(const LiveActor* actor, const char*);  // return type might be const
sead::Matrix34f* getJointMtxPtrByIndex(const LiveActor* actor, s32);
void getJointLocalTrans(sead::Vector3f*, const LiveActor* actor, const char*);
void calcJointPos(sead::Vector3f*, const LiveActor* actor, const char*);
void calcJointOffsetPos(sead::Vector3f*, const LiveActor* actor, const char*,
                        const sead::Vector3f&);
void calcJointPosByIndex(sead::Vector3f*, const LiveActor* actor, s32);
void calcJointSideDir(sead::Vector3f*, const LiveActor* actor, const char*);
void calcJointUpDir(sead::Vector3f*, const LiveActor* actor, const char*);
void calcJointFrontDir(sead::Vector3f*, const LiveActor* actor, const char*);
void calcJointScale(sead::Vector3f*, const LiveActor* actor, const char*);
void calcJointQuat(sead::Quatf*, const LiveActor* actor, const char*);
void multVecJointMtx(sead::Vector3f*, const sead::Vector3f&, const LiveActor* actor, const char*);
void multVecJointInvMtx(sead::Vector3f*, const sead::Vector3f&, const LiveActor* actor,
                        const char*);
void multMtxJointInvMtx(sead::Matrix34f*, const sead::Matrix34f&, const LiveActor* actor,
                        const char*);
void setJointVisibility(LiveActor* actor, const char*, bool);
bool isJointVisibility(const LiveActor* actor, const char*);
bool isFaceJointXDirDegreeYZ(const LiveActor* actor, const char*, const sead::Vector3f&, f32, f32);
bool isFaceJointYDirDegreeZX(const LiveActor* actor, const char*, const sead::Vector3f&, f32, f32);
bool isFaceJointZDirDegreeXY(const LiveActor* actor, const char*, const sead::Vector3f&, f32, f32);
void calcJointAngleXDirToTargetOnYDir(const LiveActor* actor, const char*, const sead::Vector3f&);
void calcJointAngleXDirToTargetOnZDir(const LiveActor* actor, const char*, const sead::Vector3f&);
void calcJointAngleYDirToTargetOnXDir(const LiveActor* actor, const char*, const sead::Vector3f&);
void calcJointAngleYDirToTargetOnZDir(const LiveActor* actor, const char*, const sead::Vector3f&);
void calcJointAngleZDirToTargetOnXDir(const LiveActor* actor, const char*, const sead::Vector3f&);
void calcJointAngleZDirToTargetOnYDir(const LiveActor* actor, const char*, const sead::Vector3f&);
const char* getMaterialName(const LiveActor* actor, s32);
s32 getMaterialCount(const LiveActor* actor);
bool isExistMaterial(const LiveActor* actor, const char*);
void getMaterialObj(const LiveActor* actor, s32);
void getMaterialObj(const LiveActor* actor, const char*);
s32 getMaterialIndex(const LiveActor* actor, const char*);
bool isExistMaterialTexture(const LiveActor* actor, const char*, const char*);
void getMaterialCategory(const LiveActor* actor, s32);
void tryGetMaterialCategory(const LiveActor* actor, s32);
bool isOnlyMaterialCategoryObject(const LiveActor* actor);
void showMaterial(LiveActor* actor, const char*);
void hideMaterial(LiveActor* actor, const char*);
void showMaterial(LiveActor* actor, s32);
void hideMaterial(LiveActor* actor, s32);
void showMaterialAll(LiveActor* actor);
void tryShowMaterial(LiveActor* actor, s32);
void tryHideMaterial(LiveActor* actor, s32);
void tryShowMaterialAll(LiveActor* actor);
void setModelMaterialParameterF32(const LiveActor* actor, s32, const char*, f32);
void setModelMaterialParameterF32(const LiveActor* actor, const char*, const char*, f32);
void setModelMaterialParameterV2F(const LiveActor* actor, s32, const char*, const sead::Vector2f&);
void setModelMaterialParameterV2F(const LiveActor* actor, const char*, const char*,
                                  const sead::Vector2f&);
void setModelMaterialParameterV3F(const LiveActor* actor, s32, const char*, const sead::Vector3f&);
void setModelMaterialParameterV3F(const LiveActor* actor, const char*, const char*,
                                  const sead::Vector3f&);
void setModelMaterialParameterV4F(const LiveActor* actor, s32, const char*, const sead::Vector4f&);
void setModelMaterialParameterV4F(const LiveActor* actor, const char*, const char*,
                                  const sead::Vector4f&);
void setModelMaterialParameterRgb(const LiveActor* actor, s32, const char*, const sead::Vector3f&);
void setModelMaterialParameterRgb(const LiveActor* actor, const char*, const char*,
                                  const sead::Vector3f&);
void setModelMaterialParameterRgb(const LiveActor* actor, s32, const char*, const sead::Color4f&);
void setModelMaterialParameterRgb(const LiveActor* actor, const char*, const char*,
                                  const sead::Color4f&);
void setModelMaterialParameterRgba(const LiveActor* actor, s32, const char*, const sead::Color4f&);
void setModelMaterialParameterRgba(const LiveActor* actor, const char*, const char*,
                                   const sead::Color4f&);
void setModelMaterialParameterAlpha(const LiveActor* actor, s32, const char*, f32);
void setModelMaterialParameterAlpha(const LiveActor* actor, const char*, const char*, f32);
void setModelMaterialParameterTextureTrans(const LiveActor* actor, const char*, s32,
                                           const sead::Vector2f&);
void getModelMaterialParameterDisplacementScale(const LiveActor* actor, const char*, s32);
void setModelMaterialParameterDisplacementScale(const LiveActor* actor, const char*, s32, f32);
void getModelUniformBlock(const LiveActor* actor, const char*);
void findModelUniformBlock(const LiveActor* actor, const char*);
void swapModelUniformBlock(agl::UniformBlock*);
void flushModelUniformBlock(agl::UniformBlock*);
void getModelDrawCategoryFromShaderAssign(bool*, bool*, bool*, bool*, const LiveActor* actor);
void trySetOcclusionQueryBox(LiveActor* actor, f32);
void trySetOcclusionQueryBox(LiveActor* actor, const sead::BoundBox3f&);
void trySetOcclusionQueryCenter(LiveActor* actor, const sead::Vector3f*);
const char* getModelName(const LiveActor* actor);
bool isModelName(const LiveActor* actor, const char*);
f32 calcModelBoundingSphereRadius(const LiveActor* actor);
void getBoundingSphereCenterAndRadius(sead::Vector3f*, f32*, const LiveActor* actor);
void calcModelBoundingBox(sead::BoundBox3f*, const LiveActor* actor);
void calcModelBoundingBoxMtx(sead::Matrix34f*, const LiveActor* actor);
void submitViewModel(const LiveActor* actor, const sead::Matrix34f&);
void replaceMaterialTextureRef(LiveActor* actor, nn::g3d::TextureRef*, const char*, const char*);
void replaceMaterialResTexture(LiveActor* actor, nn::gfx::ResTexture*, const char*, const char*);
void replaceMaterialResTexture(LiveActor*, const char*, const char*, nn::gfx::DescriptorSlot,
                               const nn::gfx::TextureView*);
void replaceMaterialLayoutTexture(LiveActor* actor, const LayoutTextureRenderObj*, const char*,
                                  const char*);
void recreateModelDisplayList(const LiveActor* actor);
void calcPolygonNum(const LiveActor* actor, s32);
void calcPolygonNumCurrentLod(const LiveActor* actor);
void calcPolygonNumCurrentLodWithoutVisAnim(const LiveActor* actor);
void getLodLevel(const LiveActor* actor);
void getMaterialLodLevel(const LiveActor* actor);
void getLodLevelNoClamp(const LiveActor* actor);
s32 getLodModelCount(const LiveActor* actor);
void forceLodLevel(LiveActor* actor, s32);
void unforceLodLevel(LiveActor* actor);
bool isExistLodModel(const LiveActor* actor);
bool isEnableMaterialLod(const LiveActor* actor);
void validateLodModel(LiveActor* actor);
void invalidateLodModel(LiveActor* actor);
bool isValidateLodModel(const LiveActor* actor);
bool isExistDitherAnimator(const LiveActor* actor);
bool isValidNearDitherAnim(const LiveActor* actor);
void stopDitherAnimAutoCtrl(LiveActor* actor);
void restartDitherAnimAutoCtrl(LiveActor* actor);
void validateDitherAnim(LiveActor* actor);
void invalidateDitherAnim(LiveActor* actor);
void validateFarDitherIfInvalidateClipping(LiveActor* actor);
void setDitherAnimSphereRadius(LiveActor* actor, f32);
void setDitherAnimBoundingBox(LiveActor* actor, const sead::Vector3f&);
void setDitherAnimMaxAlpha(LiveActor* actor, f32);
void setDitherAnimClippingJudgeLocalOffset(LiveActor* actor, const sead::Vector3f&);
void setDitherAnimClippingJudgeParam(LiveActor* actor, const char*);
void resetDitherAnimClippingJudgeParam(LiveActor* actor);
void getDitherAnimMinNearDitherAlpha(const LiveActor* actor);
f32 getDitherAnimNearClipStartDistance(const LiveActor* actor);
f32 getDitherAnimNearClipEndDistance(const LiveActor* actor);
void calcDitherAnimJudgeDistance(const LiveActor* actor);
void createUniqueShader(LiveActor* actor);
bool isJudgedToClipFrustum(const ClippingDirector*, const sead::Vector3f&, f32, f32);
bool isJudgedToClipFrustum(const LiveActor* actor, const sead::Vector3f&, f32, f32);
bool isJudgedToClipFrustum(const LiveActor* actor, f32, f32);
bool isJudgedToClipFrustumWithoutFar(const ClippingDirector*, const sead::Vector3f&, f32, f32);
bool isJudgedToClipFrustumWithoutFar(const LiveActor* actor, const sead::Vector3f&, f32, f32);
bool isJudgedToClipFrustumWithoutFar(const LiveActor* actor, f32, f32);
}  // namespace al
