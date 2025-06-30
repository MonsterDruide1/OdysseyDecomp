#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class AreaInitInfo;
class PlacementInfo;
class PlacementId;

bool isValidInfo(const PlacementInfo& placementInfo);
bool isPlaced(const ActorInitInfo& initInfo);

void getObjectName(const char** name, const ActorInitInfo& initInfo);
void getObjectName(const char** name, const PlacementInfo& placementInfo);
bool tryGetObjectName(const char** name, const ActorInitInfo& initInfo);
bool tryGetObjectName(const char** name, const PlacementInfo& placementInfo);
bool isObjectName(const ActorInitInfo& initInfo, const char* name);
bool isObjectName(const PlacementInfo& placementInfo, const char* name);
bool isObjectNameSubStr(const ActorInitInfo& initInfo, const char* name);
bool isObjectNameSubStr(const PlacementInfo& placementInfo, const char* name);

void getClassName(const char** name, const ActorInitInfo& initInfo);
void getClassName(const char** name, const PlacementInfo& placementInfo);
bool tryGetClassName(const char** name, const ActorInitInfo& initInfo);
bool tryGetClassName(const char** name, const PlacementInfo& placementInfo);
bool isClassName(const ActorInitInfo& initInfo, const char* name);
bool isClassName(const PlacementInfo& placementInfo, const char* name);

void getDisplayName(const char** name, const ActorInitInfo& initInfo);
void getDisplayName(const char** name, const PlacementInfo& placementInfo);
bool tryGetDisplayName(const char** name, const ActorInitInfo& initInfo);
bool tryGetDisplayName(const char** name, const PlacementInfo& placementInfo);

void getPlacementTargetFile(const char** targetFile, const PlacementInfo& placementInfo);

void getTrans(sead::Vector3f* trans, const ActorInitInfo& initInfo);
void getTrans(sead::Vector3f* trans, const PlacementInfo& placementInfo);
void multZoneMtx(sead::Vector3f* trans, const PlacementInfo& placementInfo);
bool tryGetTrans(sead::Vector3f* trans, const ActorInitInfo& initInfo);
bool tryGetTrans(sead::Vector3f* trans, const PlacementInfo& placementInfo);

void getRotate(sead::Vector3f* rotate, const PlacementInfo& placementInfo);
bool tryGetRotate(sead::Vector3f* rotate, const ActorInitInfo& initInfo);
bool tryGetRotate(sead::Vector3f* rotate, const PlacementInfo& placementInfo);
bool tryGetZoneMatrixTR(sead::Matrix34f* matrix, const PlacementInfo& placementInfo);

void getQuat(sead::Quatf* quat, const ActorInitInfo& initInfo);
void getQuat(sead::Quatf* quat, const PlacementInfo& placementInfo);
bool tryGetQuat(sead::Quatf* quat, const ActorInitInfo& initInfo);
bool tryGetQuat(sead::Quatf* quat, const PlacementInfo& placementInfo);

void getScale(sead::Vector3f* scale, const PlacementInfo& placementInfo);
void getScale(f32* x, f32* y, f32* z, const PlacementInfo& placementInfo);
bool tryGetScale(sead::Vector3f* scale, const ActorInitInfo& initInfo);
bool tryGetScale(sead::Vector3f* scale, const PlacementInfo& placementInfo);

void getSide(sead::Vector3f* side, const ActorInitInfo& initInfo);
void getSide(sead::Vector3f* side, const PlacementInfo& placementInfo);
bool tryGetSide(sead::Vector3f* side, const ActorInitInfo& initInfo);
bool tryGetSide(sead::Vector3f* side, const PlacementInfo& placementInfo);

void getUp(sead::Vector3f* up, const ActorInitInfo& initInfo);
void getUp(sead::Vector3f* up, const PlacementInfo& placementInfo);
bool tryGetUp(sead::Vector3f* up, const ActorInitInfo& initInfo);
bool tryGetUp(sead::Vector3f* up, const PlacementInfo& placementInfo);

void getFront(sead::Vector3f* front, const ActorInitInfo& initInfo);
void getFront(sead::Vector3f* front, const PlacementInfo& placementInfo);
bool tryGetFront(sead::Vector3f* front, const ActorInitInfo& initInfo);
bool tryGetFront(sead::Vector3f* front, const PlacementInfo& placementInfo);

bool tryGetLocalAxis(sead::Vector3f* dir, const ActorInitInfo& initInfo, s32 axis);
bool tryGetLocalAxis(sead::Vector3f* dir, const PlacementInfo& placementInfo, s32 axis);
bool tryGetLocalSignAxis(sead::Vector3f* dir, const ActorInitInfo& initInfo, s32 axis);
bool tryGetLocalSignAxis(sead::Vector3f* dir, const PlacementInfo& placementInfo, s32 axis);

bool tryGetMatrixTR(sead::Matrix34f* matrix, const ActorInitInfo& initInfo);
bool tryGetMatrixTR(sead::Matrix34f* matrix, const PlacementInfo& placementInfo);
bool tryGetMatrixTRS(sead::Matrix34f* matrix, const ActorInitInfo& initInfo);
bool tryGetMatrixTRS(sead::Matrix34f* matrix, const PlacementInfo& placementInfo);
bool tryGetInvertMatrixTR(sead::Matrix34f* matrix, const ActorInitInfo& initInfo);
bool tryGetInvertMatrixTR(sead::Matrix34f* matrix, const PlacementInfo& placementInfo);

void calcMatrixMultParent(sead::Matrix34f* matrix, const ActorInitInfo& initInfo1,
                          const ActorInitInfo& initInfo2);
void calcMatrixMultParent(sead::Matrix34f* matrix, const PlacementInfo& placementInfo1,
                          const PlacementInfo& placementInfo2);

void getArg(s32* arg, const ActorInitInfo& initInfo, const char* key);
void getArg(s32* arg, const PlacementInfo& placementInfo, const char* key);
bool tryGetArg(s32* arg, const ActorInitInfo& initInfo, const char* key);
bool tryGetArg(s32* arg, const PlacementInfo& placementInfo, const char* key);
void getArg(f32* arg, const ActorInitInfo& initInfo, const char* key);
void getArg(f32* arg, const PlacementInfo& placementInfo, const char* key);
bool tryGetArg(f32* arg, const ActorInitInfo& initInfo, const char* key);
bool tryGetArg(f32* arg, const PlacementInfo& placementInfo, const char* key);
void getArg(bool* arg, const ActorInitInfo& initInfo, const char* key);
void getArg(bool* arg, const PlacementInfo& placementInfo, const char* key);
bool tryGetArg(bool* arg, const ActorInitInfo& initInfo, const char* key);
bool tryGetArg(bool* arg, const PlacementInfo& placementInfo, const char* key);

s32 getArgS32(const ActorInitInfo& actorInitInfo, const char* key);
f32 getArgF32(const ActorInitInfo& actorInitInfo, const char* key);
void getArgV3f(sead::Vector3f* arg, const ActorInitInfo& actorInitInfo, const char* key);
void getArgV3f(sead::Vector3f* arg, const PlacementInfo& placementInfo, const char* key);
bool tryGetArgV3f(sead::Vector3f* arg, const ActorInitInfo& actorInitInfo, const char* key);
bool tryGetArgV3f(sead::Vector3f* arg, const PlacementInfo& placementInfo, const char* key);

bool isArgBool(const ActorInitInfo& initInfo, const char* key);
bool isArgBool(const PlacementInfo& placementInfo, const char* key);
bool isArgString(const ActorInitInfo& initInfo, const char* key, const char* arg);
bool isArgString(const PlacementInfo& placementInfo, const char* key, const char* arg);

void getStringArg(const char** arg, const ActorInitInfo& initInfo, const char* key);
void getStringArg(const char** arg, const PlacementInfo& placementInfo, const char* key);
void getStringArg(const char** arg, const AreaInitInfo& initInfo, const char* key);
const char* getStringArg(const ActorInitInfo& initInfo, const char* key);
const char* getStringArg(const PlacementInfo& placementInfo, const char* key);
const char* getStringArg(const AreaInitInfo& initInfo, const char* key);
bool tryGetStringArg(const char** arg, const ActorInitInfo& initInfo, const char* key);
bool tryGetStringArg(const char** arg, const PlacementInfo& initInfo, const char* key);
bool tryGetStringArg(const char** arg, const AreaInitInfo& initInfo, const char* key);
bool tryGetArgV2f(sead::Vector2f* arg, const ActorInitInfo& initInfo, const char* key);
bool tryGetArgV2f(sead::Vector2f* arg, const PlacementInfo& initInfo, const char* key);
bool tryGetArgColor(sead::Color4f* arg, const ActorInitInfo& initInfo, const char* key);
bool tryGetArgColor(sead::Color4f* arg, const PlacementInfo& initInfo, const char* key);

void getLayerConfigName(const char** name, const ActorInitInfo& initInfo);
void getLayerConfigName(const char** name, const PlacementInfo& initInfo);
bool tryGetZoneNameIfExist(const char** name, const PlacementInfo& placementInfo);
void getPlacementId(PlacementId* placementId, const PlacementInfo& placementInfo);

bool tryGetBoolArgOrFalse(const ActorInitInfo& initInfo, const char* key);
s32 getCountPlacementInfo(const PlacementInfo& placementInfo);
void getPlacementInfoByKey(PlacementInfo* outPlacementInfo, const PlacementInfo& placementInfo,
                           const char* key);
bool tryGetPlacementInfoByKey(PlacementInfo* outPlacementInfo, const PlacementInfo& placementInfo,
                              const char* key);
void getPlacementInfoByIndex(PlacementInfo* outPlacementInfo, const PlacementInfo& placementInfo,
                             s32 index);
bool tryGetPlacementInfoByIndex(PlacementInfo* outPlacementInfo, const PlacementInfo& placementInfo,
                                s32 index);
void getPlacementInfoAndKeyNameByIndex(PlacementInfo* outPlacementInfo, const char** outKey,
                                       const PlacementInfo& placementInfo, s32 index);
bool tryGetPlacementInfoAndKeyNameByIndex(PlacementInfo* outPlacementInfo, const char** outKey,
                                          const PlacementInfo& placementInfo, s32 index);

PlacementId* createPlacementId(const ActorInitInfo& initInfo);
PlacementId* createPlacementId(const PlacementInfo& placementInfo);
bool tryGetPlacementId(PlacementId* placementId, const ActorInitInfo& initInfo);
bool tryGetPlacementId(PlacementId* placementId, const PlacementInfo& placementInfo);
void getPlacementId(PlacementId* placementId, const ActorInitInfo& initInfo);
bool isEqualPlacementId(const PlacementId& placementId, const PlacementId& otherPlacementId);
bool isEqualPlacementId(const PlacementInfo& placementInfo,
                        const PlacementInfo& otherPlacementInfo);

bool isExistRail(const ActorInitInfo& initInfo, const char* linkName);

bool tryGetRailIter(PlacementInfo* railPlacementInfo, const PlacementInfo& placementInfo,
                    const char* linkName);
bool tryGetLinksInfo(PlacementInfo* railPlacementInfo, const PlacementInfo& placementInfo,
                     const char* linkName);
bool tryGetMoveParameterRailIter(PlacementInfo* railPlacementInfo,
                                 const PlacementInfo& placementInfo);
bool tryGetRailPointPos(sead::Vector3f* railPoint, const PlacementInfo& placementInfo);
void getRailPointHandlePrev(sead::Vector3f* railPoint, const PlacementInfo& placementInfo);
bool tryGetRailPointHandlePrev(sead::Vector3f* railPoint, const PlacementInfo& placementInfo);
void getRailPointHandleNext(sead::Vector3f* railPoint, const PlacementInfo& placementInfo);
bool tryGetRailPointHandleNext(sead::Vector3f* railPoint, const PlacementInfo& placementInfo);

bool isExistGraphRider(const ActorInitInfo& initInfo);

s32 calcLinkChildNum(const ActorInitInfo& initInfo, const char* linkName);
s32 calcLinkChildNum(const PlacementInfo& placementInfo, const char* linkName);

bool isExistLinkChild(const ActorInitInfo& initInfo, const char* linkName, s32 index);
bool isExistLinkChild(const PlacementInfo& placementInfo, const char* linkName, s32 index);
bool isExistLinkChild(const AreaInitInfo& initInfo, const char* linkName, s32 index);

s32 calcLinkNestNum(const ActorInitInfo& initInfo, const char* linkName);
s32 calcLinkNestNum(const PlacementInfo& placementInfo, const char* linkName);

void getLinksInfo(PlacementInfo* linkPlacementInfo, const PlacementInfo& placementInfo,
                  const char* linkName);
void getLinksInfoByIndex(PlacementInfo* linkPlacementInfo, const PlacementInfo& placementInfo,
                         const char* linkName, s32 index);
void getLinksInfo(PlacementInfo* linkPlacementInfo, const ActorInitInfo& initInfo,
                  const char* linkName);
void getLinksInfoByIndex(PlacementInfo* linkPlacementInfo, const ActorInitInfo& initInfo,
                         const char* linkName, s32 index);
bool tryGetLinksInfo(PlacementInfo* linkPlacementInfo, const ActorInitInfo& initInfo,
                     const char* linkName);

void getLinksMatrix(sead::Matrix34f* matrix, const ActorInitInfo& initInfo, const char* linkName);
void getLinksMatrixByIndex(sead::Matrix34f* matrix, const ActorInitInfo& initInfo,
                           const char* linkName, s32 index);
void getLinkTR(sead::Vector3f* trans, sead::Vector3f* rotate, const PlacementInfo& placementInfo,
               const char* linkName);
void getLinkTR(sead::Vector3f* trans, sead::Vector3f* rotate, const ActorInitInfo& initInfo,
               const char* linkName);
void getLinkTR(sead::Vector3f* trans, sead::Vector3f* rotate, const AreaInitInfo& initInfo,
               const char* linkName);
void getLinksQT(sead::Quatf* quat, sead::Vector3f* trans, const ActorInitInfo& initInfo,
                const char* linkName);
void getLinksQT(sead::Quatf* quat, sead::Vector3f* trans, const PlacementInfo& placementInfo,
                const char* linkName);

bool tryGetLinksQT(sead::Quatf* quat, sead::Vector3f* trans, const ActorInitInfo& initInfo,
                   const char* linkName);
bool tryGetLinksQTS(sead::Quatf* quat, sead::Vector3f* trans, sead::Vector3f* scale,
                    const ActorInitInfo& initInfo, const char* linkName);
bool tryGetLinksMatrixTR(sead::Matrix34f* matrix, const ActorInitInfo& initInfo,
                         const char* linkName);
bool tryGetLinksMatrixTR(sead::Matrix34f* matrix, const AreaInitInfo& initInfo,
                         const char* linkName);
bool tryGetLinksMatrixTRS(sead::Matrix34f* matrix, const ActorInitInfo& initInfo,
                          const char* linkName);
bool tryGetLinksTrans(sead::Vector3f* trans, const ActorInitInfo& initInfo, const char* linkName);
bool tryGetLinksTrans(sead::Vector3f* trans, const PlacementInfo& placementInfo,
                      const char* linkName);
bool tryGetLinksQuat(sead::Quatf* quat, const ActorInitInfo& initInfo, const char* linkName);
bool tryGetLinksTR(sead::Vector3f* trans, sead::Vector3f* rotate, const ActorInitInfo& initInfo,
                   const char* linkName);

void getChildTrans(sead::Vector3f* trans, const PlacementInfo& placementInfo, const char* linkName);
void getChildTrans(sead::Vector3f* trans, const ActorInitInfo& initInfo, const char* linkName);
void getChildTrans(sead::Vector3f* trans, const AreaInitInfo& initInfo, const char* linkName);
void getChildLinkT(sead::Vector3f* trans, const ActorInitInfo& initInfo, const char* linkName,
                   s32 index);
void getChildLinkTR(sead::Vector3f* trans, sead::Vector3f* rotate, const ActorInitInfo& initInfo,
                    const char* linkName, s32 index);

s32 calcMatchNameLinkCount(const PlacementInfo& placementInfo, const char* match);
s32 calcLinkCountClassName(const PlacementInfo& placementInfo, const char* className);

bool tryGetZoneMatrixTR(sead::Matrix34f* matrix, const ActorInitInfo& initInfo);
bool tryGetDisplayOffset(sead::Vector3f* offset, const ActorInitInfo& initInfo);
bool tryGetDisplayOffset(sead::Vector3f* offset, const PlacementInfo& placementInfo);
bool tryGetChildDisplayOffset(sead::Vector3f* offset, const ActorInitInfo& initInfo,
                              const char* linkName);
bool tryGetDisplayRotate(sead::Vector3f* rotate, const ActorInitInfo& initInfo);
bool tryGetDisplayScale(sead::Vector3f* scale, const ActorInitInfo& initInfo);

}  // namespace al

namespace alPlacementFunction {

s32 getCameraId(const al::ActorInitInfo& initInfo);
bool getLinkGroupId(al::PlacementId* groupId, const al::ActorInitInfo& initInfo,
                    const char* linkName);
bool isEnableLinkGroupId(const al::ActorInitInfo& initInfo, const char* linkName);
bool isEnableGroupClipping(const al::ActorInitInfo& initInfo);
bool getClippingGroupId(al::PlacementId* groupId, const al::ActorInitInfo& initInfo);
al::PlacementId* createClippingViewId(const al::PlacementInfo& placementInfo);
bool getClippingViewId(al::PlacementId* viewId, const al::PlacementInfo& placementInfo);
bool getClippingViewId(al::PlacementId* viewId, const al::ActorInitInfo& initInfo);
void getModelName(const char** modelName, const al::ActorInitInfo& initInfo);
void getModelName(const char** modelName, const al::PlacementInfo& placementInfo);
bool tryGetModelName(const char** modelName, const al::PlacementInfo& placementInfo);
bool tryGetModelName(const char** modelName, const al::ActorInitInfo& initInfo);

}  // namespace alPlacementFunction
