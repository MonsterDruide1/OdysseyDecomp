#include "Library/Placement/PlacementFunction.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Math/Axis.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

bool isValidInfo(const PlacementInfo& placementInfo) {
    return placementInfo.getPlacementIter().isValid();
}

bool isPlaced(const ActorInitInfo& initInfo) {
    return isValidInfo(initInfo.getPlacementInfo());
}

void getObjectName(const char** name, const ActorInitInfo& initInfo) {
    getObjectName(name, initInfo.getPlacementInfo());
}

void getObjectName(const char** name, const PlacementInfo& placementInfo) {
    tryGetObjectName(name, placementInfo);
}

bool tryGetObjectName(const char** name, const ActorInitInfo& initInfo) {
    return tryGetObjectName(name, initInfo.getPlacementInfo());
}

bool tryGetObjectName(const char** name, const PlacementInfo& placementInfo) {
    const char* obj = "";
    if (tryGetStringArg(&obj, placementInfo, "UnitConfigName")) {
        *name = obj;
        return true;
    }
    return false;
}

bool isObjectName(const ActorInitInfo& initInfo, const char* name) {
    return isObjectName(initInfo.getPlacementInfo(), name);
}

bool isObjectName(const PlacementInfo& placementInfo, const char* name) {
    const char* obj;
    return tryGetObjectName(&obj, placementInfo) && isEqualString(obj, name);
}

bool isObjectNameSubStr(const ActorInitInfo& initInfo, const char* name) {
    return isObjectNameSubStr(initInfo.getPlacementInfo(), name);
}

bool isObjectNameSubStr(const PlacementInfo& placementInfo, const char* name) {
    const char* obj;
    return tryGetObjectName(&obj, placementInfo) && isEqualSubString(obj, name);
}

void getClassName(const char** name, const ActorInitInfo& initInfo) {
    getClassName(name, initInfo.getPlacementInfo());
}

void getClassName(const char** name, const PlacementInfo& placementInfo) {
    tryGetClassName(name, placementInfo);
}

bool tryGetClassName(const char** name, const ActorInitInfo& initInfo) {
    return tryGetClassName(name, initInfo.getPlacementInfo());
}

bool tryGetClassName(const char** name, const PlacementInfo& placementInfo) {
    PlacementInfo unitConfig;
    if (!tryGetPlacementInfoByKey(&unitConfig, placementInfo, "UnitConfig"))
        return false;
    return tryGetStringArg(name, unitConfig, "ParameterConfigName");
}

void getDisplayName(const char** name, const ActorInitInfo& initInfo) {
    getDisplayName(name, initInfo.getPlacementInfo());
}

void getDisplayName(const char** name, const PlacementInfo& placementInfo) {
    tryGetDisplayName(name, placementInfo);
}

bool tryGetDisplayName(const char** name, const ActorInitInfo& initInfo) {
    return tryGetDisplayName(name, initInfo.getPlacementInfo());
}

bool tryGetDisplayName(const char** name, const PlacementInfo& placementInfo) {
    PlacementInfo unitConfig;
    if (!tryGetPlacementInfoByKey(&unitConfig, placementInfo, "UnitConfig"))
        return false;
    return tryGetStringArg(name, unitConfig, "DisplayName");
}

void getPlacementTargetFile(const char** targetFile, const PlacementInfo& placementInfo) {
    PlacementInfo unitConfig;
    if (!tryGetPlacementInfoByKey(&unitConfig, placementInfo, "UnitConfig"))
        return;
    tryGetStringArg(targetFile, unitConfig, "PlacementTargetFile");
}

void getTrans(sead::Vector3f* trans, const ActorInitInfo& initInfo) {
    getTrans(trans, initInfo.getPlacementInfo());
}

void getTrans(sead::Vector3f* trans, const PlacementInfo& placementInfo) {
    tryGetTrans(trans, placementInfo);
}

void multZoneMtx(sead::Vector3f* trans, const PlacementInfo& placementInfo) {
    sead::Matrix34f mtx;
    if (tryGetZoneMatrixTR(&mtx, placementInfo))
        trans->mul(mtx);
}

bool tryGetTrans(sead::Vector3f* trans, const ActorInitInfo& initInfo) {
    return tryGetTrans(trans, initInfo.getPlacementInfo());
}

bool tryGetTrans(sead::Vector3f* trans, const PlacementInfo& placementInfo) {
    if (!tryGetArgV3f(trans, placementInfo, "Translate"))
        return false;
    multZoneMtx(trans, placementInfo);
    return true;
}

void getRotate(sead::Vector3f* rotate, const ActorInitInfo& initInfo) {
    tryGetRotate(rotate, initInfo);
}

void getRotate(sead::Vector3f* rotate, const PlacementInfo& placementInfo) {
    tryGetRotate(rotate, placementInfo);
}

bool tryGetRotate(sead::Vector3f* rotate, const ActorInitInfo& initInfo) {
    return tryGetRotate(rotate, initInfo.getPlacementInfo());
}

bool tryGetRotate(sead::Vector3f* rotate, const PlacementInfo& placementInfo) {
    if (!tryGetArgV3f(rotate, placementInfo, "Rotate"))
        return false;

    sead::Matrix34f mtx;
    if (tryGetZoneMatrixTR(&mtx, placementInfo)) {
        sead::Matrix34f rot, rot2;
        sead::Vector3f vec1 = {sead::Mathf::deg2rad(rotate->x), sead::Mathf::deg2rad(rotate->y),
                               sead::Mathf::deg2rad(rotate->z)};
        rot.makeRT(vec1, sead::Vector3f::zero);
        rot2.setMul(mtx, rot);
        rot2.getRotation(*rotate);
        *rotate = {sead::Mathf::rad2deg(rotate->x), sead::Mathf::rad2deg(rotate->y),
                   sead::Mathf::rad2deg(rotate->z)};
    }
    return true;
}

bool tryGetZoneMatrixTR(sead::Matrix34f* matrix, const PlacementInfo& placementInfo) {
    ByamlIter zone = placementInfo.getZoneIter();
    if (!zone.isValid())
        return false;

    sead::Vector3f translate = sead::Vector3f::zero;
    if (!tryGetByamlV3f(&translate, zone, "Translate"))
        return false;

    sead::Vector3f rotate = sead::Vector3f::zero;
    if (!tryGetByamlV3f(&rotate, zone, "Rotate"))
        return false;

    matrix->makeRT({sead::Mathf::rad2deg(rotate.x), sead::Mathf::rad2deg(rotate.y),
                   sead::Mathf::rad2deg(rotate.z)}, translate);
    return true;
}

void getQuat(sead::Quatf* quat, const ActorInitInfo& initInfo) {
    tryGetQuat(quat, initInfo);
}

void getQuat(sead::Quatf* quat, const PlacementInfo& placementInfo) {
    tryGetQuat(quat, placementInfo);
}

bool tryGetQuat(sead::Quatf* quat, const ActorInitInfo& initInfo) {
    return tryGetQuat(quat, initInfo.getPlacementInfo());
}

bool tryGetQuat(sead::Quatf* quat, const PlacementInfo& placementInfo) {
    sead::Vector3f rotate = sead::Vector3f::zero;
    if (!tryGetRotate(&rotate, placementInfo)) {
        *quat = sead::Quatf::unit;
        return false;
    }
    quat->setRPY(sead::Mathf::deg2rad(rotate.x), sead::Mathf::deg2rad(rotate.y),
                 sead::Mathf::deg2rad(rotate.z));
    return true;
}

void getScale(sead::Vector3f* scale, const ActorInitInfo& initInfo) {
    tryGetScale(scale, initInfo);
}

void getScale(sead::Vector3f* scale, const PlacementInfo& placementInfo) {
    tryGetScale(scale, placementInfo);
}

bool tryGetScale(sead::Vector3f* scale, const ActorInitInfo& initInfo) {
    return tryGetScale(scale, initInfo.getPlacementInfo());
}

bool tryGetScale(sead::Vector3f* scale, const PlacementInfo& placementInfo) {
    return tryGetArgV3f(scale, placementInfo, "Scale");
}

void getSide(sead::Vector3f* side, const ActorInitInfo& initInfo) {
    tryGetSide(side, initInfo);
}

void getSide(sead::Vector3f* side, const PlacementInfo& placementInfo) {
    tryGetSide(side, placementInfo);
}

bool tryGetSide(sead::Vector3f* side, const ActorInitInfo& initInfo) {
    return tryGetSide(side, initInfo.getPlacementInfo());
}

bool tryGetSide(sead::Vector3f* side, const PlacementInfo& placementInfo) {
    sead::Quatf quat = sead::Quatf::unit;
    if (!tryGetQuat(&quat, placementInfo))
        return false;
    calcQuatSide(side, quat);
    return true;
}

void getUp(sead::Vector3f* up, const ActorInitInfo& initInfo) {
    tryGetUp(up, initInfo);
}

void getUp(sead::Vector3f* up, const PlacementInfo& placementInfo) {
    tryGetUp(up, placementInfo);
}

bool tryGetUp(sead::Vector3f* up, const ActorInitInfo& initInfo) {
    return tryGetUp(up, initInfo.getPlacementInfo());
}

bool tryGetUp(sead::Vector3f* up, const PlacementInfo& placementInfo) {
    sead::Quatf quat = sead::Quatf::unit;
    if (!tryGetQuat(&quat, placementInfo))
        return false;
    calcQuatUp(up, quat);
    return true;
}

void getFront(sead::Vector3f* front, const ActorInitInfo& initInfo) {
    tryGetFront(front, initInfo);
}

void getFront(sead::Vector3f* front, const PlacementInfo& placementInfo) {
    tryGetFront(front, placementInfo);
}

bool tryGetFront(sead::Vector3f* front, const ActorInitInfo& initInfo) {
    return tryGetFront(front, initInfo.getPlacementInfo());
}

bool tryGetFront(sead::Vector3f* front, const PlacementInfo& placementInfo) {
    sead::Quatf quat = sead::Quatf::unit;
    if (!tryGetQuat(&quat, placementInfo))
        return false;
    calcQuatFront(front, quat);
    return true;
}

bool tryGetLocalAxis(sead::Vector3f* front, const ActorInitInfo& initInfo, s32 axis) {
    return tryGetLocalAxis(front, initInfo.getPlacementInfo(), axis);
}

bool tryGetLocalAxis(sead::Vector3f* front, const PlacementInfo& placementInfo, s32 axis) {
    // does not align with Math/Axis.h
    switch (axis) {
    case 0:
        return tryGetSide(front, placementInfo);
    case 1:
        return tryGetUp(front, placementInfo);
    case 2:
        return tryGetFront(front, placementInfo);
    default:
        return false;
    }
}

bool tryGetLocalSignAxis(sead::Vector3f* front, const ActorInitInfo& initInfo, s32 axis) {
    return tryGetLocalSignAxis(front, initInfo.getPlacementInfo(), axis);
}

bool tryGetLocalSignAxis(sead::Vector3f* front, const PlacementInfo& placementInfo, s32 axis) {
    switch ((Axis)sead::Mathi::abs(axis)) {
    case Axis::X:
        tryGetSide(front, placementInfo);
        break;
    case Axis::Y:
        tryGetUp(front, placementInfo);
        break;
    case Axis::Z:
        tryGetFront(front, placementInfo);
        break;
    default:
        return false;
    }
    if (axis < 0)
        *front *= -1;
    return true;
}

bool tryGetMatrixTR(sead::Matrix34f* matrix, const ActorInitInfo& initInfo) {
    return tryGetMatrixTR(matrix, initInfo.getPlacementInfo());
}

bool tryGetMatrixTR(sead::Matrix34f* matrix, const PlacementInfo& placementInfo) {
    sead::Vector3f trans = sead::Vector3f::zero;
    sead::Vector3f rotate = sead::Vector3f::zero;
    if(!tryGetTrans(&trans, placementInfo)) return false;
    if(!tryGetRotate(&rotate, placementInfo)) return false;
    matrix->makeRT({sead::Mathf::deg2rad(rotate.x), sead::Mathf::deg2rad(rotate.y),
                    sead::Mathf::deg2rad(rotate.z)}, trans);
    return true;
}

bool tryGetMatrixTRS(sead::Matrix34f* matrix, const ActorInitInfo& initInfo) {
    return tryGetMatrixTRS(matrix, initInfo.getPlacementInfo());
}

bool tryGetMatrixTRS(sead::Matrix34f* matrix, const PlacementInfo& placementInfo) {
    sead::Vector3f trans = sead::Vector3f::zero;
    sead::Vector3f rotate = sead::Vector3f::zero;
    sead::Vector3f scale = sead::Vector3f::ones;
    if(!tryGetTrans(&trans, placementInfo)) return false;
    if(!tryGetRotate(&rotate, placementInfo)) return false;
    if(!tryGetScale(&scale, placementInfo)) return false;
    matrix->makeSRT(scale, {sead::Mathf::deg2rad(rotate.x), sead::Mathf::deg2rad(rotate.y),
                    sead::Mathf::deg2rad(rotate.z)}, trans);
    return true;
}

bool tryGetInvertMatrixTR(sead::Matrix34f* matrix, const ActorInitInfo& initInfo) {
    return tryGetInvertMatrixTR(matrix, initInfo.getPlacementInfo());
}

bool tryGetInvertMatrixTR(sead::Matrix34f* matrix, const PlacementInfo& placementInfo) {
    sead::Matrix34f mtx;
    if (!tryGetMatrixTR(&mtx, placementInfo))
        return false;
    matrix->setInverse(mtx);
    return true;
}

void calcMatrixMultParent(sead::Matrix34f* matrix, const ActorInitInfo& initInfo1,
                          const ActorInitInfo& initInfo2) {
    calcMatrixMultParent(matrix, initInfo1.getPlacementInfo(), initInfo2.getPlacementInfo());
}

void calcMatrixMultParent(sead::Matrix34f* matrix, const PlacementInfo& placementInfo1,
                          const PlacementInfo& placementInfo2) {
    sead::Matrix34f mtx1 = sead::Matrix34f::ident;
    tryGetMatrixTR(&mtx1, placementInfo1);
    sead::Matrix34f mtx2 = sead::Matrix34f::ident;
    tryGetMatrixTR(&mtx2, placementInfo2);
    matrix->setMul(mtx1, mtx2);
}

void getArg(s32* arg, const ActorInitInfo& initInfo, const char* key) {
    getArg(arg, initInfo.getPlacementInfo(), key);
}

void getArg(s32* arg, const PlacementInfo& placementInfo, const char* key) {
    tryGetArg(arg, placementInfo, key);
}

bool tryGetArg(s32* arg, const ActorInitInfo& initInfo, const char* key) {
    return tryGetArg(arg, initInfo.getPlacementInfo(), key);
}

bool tryGetArg(s32* arg, const PlacementInfo& placementInfo, const char* key) {
    return placementInfo.getPlacementIter().tryGetIntByKey(arg, key);
}

void getArg(f32* arg, const ActorInitInfo& initInfo, const char* key) {
    getArg(arg, initInfo.getPlacementInfo(), key);
}

void getArg(f32* arg, const PlacementInfo& placementInfo, const char* key) {
    tryGetArg(arg, placementInfo, key);
}

bool tryGetArg(f32* arg, const ActorInitInfo& initInfo, const char* key) {
    return tryGetArg(arg, initInfo.getPlacementInfo(), key);
}

bool tryGetArg(f32* arg, const PlacementInfo& placementInfo, const char* key) {
    return placementInfo.getPlacementIter().tryGetFloatByKey(arg, key);
}

void getArg(bool* arg, const ActorInitInfo& initInfo, const char* key) {
    getArg(arg, initInfo.getPlacementInfo(), key);
}

void getArg(bool* arg, const PlacementInfo& placementInfo, const char* key) {
    tryGetArg(arg, placementInfo, key);
}

bool tryGetArg(bool* arg, const ActorInitInfo& initInfo, const char* key) {
    return tryGetArg(arg, initInfo.getPlacementInfo(), key);
}

bool tryGetArg(bool* arg, const PlacementInfo& placementInfo, const char* key) {
    return placementInfo.getPlacementIter().tryGetBoolByKey(arg, key);
}

s32 getArgS32(const ActorInitInfo& actorInitInfo, const char* key) {
    int arg = 0;
    getArg(&arg, actorInitInfo, key);
    return arg;
}

f32 getArgF32(const ActorInitInfo& actorInitInfo, const char* key) {
    f32 arg = 0.0f;
    getArg(&arg, actorInitInfo, key);
    return arg;
}

void getArgV3f(sead::Vector3f* arg, const ActorInitInfo& actorInitInfo, const char* key) {
    tryGetArgV3f(arg, actorInitInfo, key);
}

void getArgV3f(sead::Vector3f* arg, const PlacementInfo& placementInfo, const char* key) {
    tryGetArgV3f(arg, placementInfo, key);
}

bool tryGetArgV3f(sead::Vector3f* arg, const ActorInitInfo& actorInitInfo, const char* key) {
    return tryGetArgV3f(arg, actorInitInfo.getPlacementInfo(), key);
}

bool tryGetArgV3f(sead::Vector3f* arg, const PlacementInfo& placementInfo, const char* key) {
    return al::tryGetByamlV3f(arg, placementInfo.getPlacementIter(), key);
}

bool isArgBool(const ActorInitInfo& initInfo, const char* key) {
    bool arg = false;
    getArg(&arg, initInfo, key);
    return arg;
}

bool isArgBool(const PlacementInfo& placementInfo, const char* key) {
    bool arg = false;
    getArg(&arg, placementInfo, key);
    return arg;
}

bool isArgString(const ActorInitInfo& initInfo, const char* key, const char* arg) {
    return isArgString(initInfo.getPlacementInfo(), key, arg);
}

bool isArgString(const PlacementInfo& placementInfo, const char* key, const char* arg) {
    return isEqualString(getStringArg(placementInfo, key), arg);
}

void getStringArg(const char** arg, const ActorInitInfo& initInfo, const char* key) {
    getStringArg(arg, initInfo.getPlacementInfo(), key);
}

void getStringArg(const char** arg, const PlacementInfo& placementInfo, const char* key) {
    tryGetStringArg(arg, placementInfo, key);
}

void getStringArg(const char** arg, const AreaInitInfo& initInfo, const char* key) {
    getStringArg(arg, initInfo.getPlacementInfo(), key);
}

const char* getStringArg(const ActorInitInfo& initInfo, const char* key) {
    return getStringArg(initInfo.getPlacementInfo(), key);
}

const char* getStringArg(const PlacementInfo& placementInfo, const char* key) {
    const char* str = "";
    if (!placementInfo.getPlacementIter().tryGetStringByKey(&str, key) || isEqualString("", str))
        return nullptr;
    return str;
}

const char* getStringArg(const AreaInitInfo& initInfo, const char* key) {
    return getStringArg(initInfo.getPlacementInfo(), key);
}

bool tryGetStringArg(const char** arg, const ActorInitInfo& initInfo, const char* key) {
    return tryGetStringArg(arg, initInfo.getPlacementInfo(), key);
}

bool tryGetStringArg(const char** arg, const PlacementInfo& initInfo, const char* key) {
    const char* str = "";
    if (!initInfo.getPlacementIter().tryGetStringByKey(&str, key) || isEqualString("", str))
        return false;
    *arg = str;
    return true;
}

bool tryGetStringArg(const char** arg, const AreaInitInfo& initInfo, const char* key) {
    return tryGetStringArg(arg, initInfo.getPlacementInfo(), key);
}

bool tryGetArgV2f(sead::Vector2f* arg, const ActorInitInfo& initInfo, const char* key) {
    return tryGetArgV2f(arg, initInfo.getPlacementInfo(), key);
}

bool tryGetArgV2f(sead::Vector2f* arg, const PlacementInfo& initInfo, const char* key) {
    return al::tryGetByamlV2f(arg, initInfo.getPlacementIter(), key);
}

bool tryGetArgColor(sead::Color4f* arg, const ActorInitInfo& initInfo, const char* key) {
    return tryGetArgColor(arg, initInfo.getPlacementInfo(), key);
}

bool tryGetArgColor(sead::Color4f* arg, const PlacementInfo& initInfo, const char* key) {
    return al::tryGetByamlColor(arg, initInfo.getPlacementIter(), key);
}

void getLayerConfigName(const char** name, const ActorInitInfo& initInfo) {
    getLayerConfigName(name, initInfo.getPlacementInfo());
}

void getLayerConfigName(const char** name, const PlacementInfo& initInfo) {
    return getStringArg(name, initInfo, "LayerConfigName");
}

bool tryGetZoneNameIfExist(const char** name, const PlacementInfo& placementInfo) {
    // TODO
}

void getPlacementId(PlacementId* placementId, const PlacementInfo& placementInfo) {
    // TODO
}

bool tryGetBoolArgOrFalse(const ActorInitInfo& initInfo, const char* key) {
    // TODO
}

s32 getCountPlacementInfo(const PlacementInfo& placementInfo) {
    // TODO
}

void getPlacementInfoByKey(PlacementInfo* outPlacementInfo, const PlacementInfo& placementInfo, const char* key) {
    tryGetPlacementInfoByKey(outPlacementInfo, placementInfo, key);
}

bool tryGetPlacementInfoByKey(PlacementInfo* outPlacementInfo, const PlacementInfo& placementInfo,
                              const char* key) {
    ByamlIter iter;
    if (!placementInfo.getPlacementIter().tryGetIterByKey(&iter, key))
        return false;
    outPlacementInfo->set(iter, placementInfo.getZoneIter());
    return true;
}

void getPlacementInfoByIndex(PlacementInfo* outPlacementInfo, const PlacementInfo& placementInfo, s32 index) {
    // TODO
}

bool tryGetPlacementInfoByIndex(PlacementInfo* outPlacementInfo, const PlacementInfo& placementInfo, s32 index) {
    // TODO
}

void getPlacementInfoAndKeyNameByIndex(PlacementInfo* outPlacementInfo, const char** outKey,
                                       const PlacementInfo&, s32 index) {
    // TODO
}

bool tryGetPlacementInfoAndKeyNameByIndex(PlacementInfo* outPlacementInfo, const char** outKey,
                                          const PlacementInfo&, s32 index) {
    // TODO
}

PlacementId* createPlacementId(const ActorInitInfo& initInfo) {
    // TODO
}

PlacementId* createPlacementId(const PlacementInfo& placementInfo) {
    // TODO
}

bool tryGetPlacementId(PlacementId* placementId, const ActorInitInfo& initInfo) {
    // TODO
}

bool tryGetPlacementId(PlacementId* placementId, const PlacementInfo& placementInfo) {
    // TODO
}

void getPlacementId(PlacementId* placementId, const ActorInitInfo& initInfo) {
    // TODO
}

bool isEqualPlacementId(PlacementId* placementId, const PlacementId& otherPlacementId) {
    // TODO
}

bool isEqualPlacementId(PlacementId* placementId, const PlacementInfo& placementInfo) {
    // TODO
}

bool isExistRail(const ActorInitInfo& initInfo, const char* linkName) {
    // TODO
}

bool tryGetRailIter(PlacementInfo* railPlacementInfo, const PlacementInfo& placementInfo,
                    const char* linkName) {
    // TODO
}

bool tryGetLinksInfo(PlacementInfo* railPlacementInfo, const PlacementInfo& placementInfo,
                     const char* linkName) {
    // TODO
}

bool tryGetMoveParameterRailIter(PlacementInfo* railPlacementInfo,
                                 const PlacementInfo& placementInfo) {
    // TODO
}

bool tryGetRailPointPos(sead::Vector3f* railPoint, const PlacementInfo& placementInfo) {
    // TODO
}

void getRailPointHandlePrev(sead::Vector3f* railPoint, const PlacementInfo& placementInfo) {
    // TODO
}

void tryGetRailPointHandlePrev(sead::Vector3f* railPoint, const PlacementInfo& placementInfo) {
    // TODO
}

void getRailPointHandleNext(sead::Vector3f* railPoint, const PlacementInfo& placementInfo) {
    // TODO
}

void tryGetRailPointHandleNext(sead::Vector3f* railPoint, const PlacementInfo& placementInfo) {
    // TODO
}

bool isExistGraphRider(const ActorInitInfo& initInfo) {
    // TODO
}

s32 calcLinkChildNum(const ActorInitInfo& initInfo, const char* linkName) {
    // TODO
}

s32 calcLinkChildNum(const PlacementInfo& placementInfo, const char* linkName) {
    // TODO
}

bool isExistLinkChild(const ActorInitInfo& initInfo, const char* linkName, s32 index) {
    // TODO
}

bool isExistLinkChild(const PlacementInfo& placementInfo, const char* linkName, s32 index) {
    // TODO
}

bool isExistLinkChild(const AreaInitInfo& initInfo, const char* linkName, s32 index) {
    // TODO
}

s32 calcLinkNestNum(const ActorInitInfo& initInfo, const char* linkName) {
    // TODO
}

s32 calcLinkNestNum(const PlacementInfo& placementInfo, const char* linkName) {
    // TODO
}

void getLinksInfo(PlacementInfo* linkPlacementInfo, const PlacementInfo& placementInfo,
                  const char* linkName) {
    // TODO
}

void getLinksInfoByIndex(PlacementInfo* linkPlacementInfo, const PlacementInfo& placementInfo,
                         const char* linkName, s32) {
    // TODO
}

void getLinksInfo(PlacementInfo* linkPlacementInfo, const ActorInitInfo& initInfo,
                  const char* linkName) {
    // TODO
}

void getLinksInfoByIndex(PlacementInfo* linkPlacementInfo, const ActorInitInfo& initInfo,
                         const char* linkName, s32) {
    // TODO
}

bool tryGetLinksInfo(PlacementInfo* linkPlacementInfo, const ActorInitInfo& initInfo,
                     const char* linkName) {
    // TODO
}

void getLinksMatrix(sead::Matrix34f* matrix, const ActorInitInfo& initInfo, const char* linkName) {
    // TODO
}

void getLinksMatrixByIndex(sead::Matrix34f*, const ActorInitInfo& initInfo, const char* linkName,
                           s32) {
    // TODO
}

void getLinkTR(sead::Vector3f* trans, sead::Vector3f* rotate, const PlacementInfo& placementInfo,
               const char* linkName) {
    // TODO
}

void getLinkTR(sead::Vector3f* trans, sead::Vector3f* rotate, const ActorInitInfo& initInfo,
               const char* linkName) {
    // TODO
}

void getLinkTR(sead::Vector3f* trans, sead::Vector3f* rotate, const AreaInitInfo& initInfo,
               const char* linkName) {
    // TODO
}

void getLinksQT(sead::Quatf* quat, sead::Vector3f* trans, const ActorInitInfo& initInfo,
                const char* linkName) {
    // TODO
}

void getLinksQT(sead::Quatf* quat, sead::Vector3f* trans, const PlacementInfo& placementInfo,
                const char* linkName) {
    // TODO
}

bool tryGetLinksQT(sead::Quatf*, sead::Vector3f*, const ActorInitInfo& initInfo,
                   const char* linkName) {
    // TODO
}

bool tryGetLinksQTS(sead::Quatf*, sead::Vector3f*, sead::Vector3f*, const ActorInitInfo& initInfo,
                    const char* linkName) {
    // TODO
}

bool tryGetLinksMatrixTR(sead::Matrix34f* matrix, const ActorInitInfo& initInfo,
                         const char* linkName) {
    // TODO
}

bool tryGetLinksMatrixTR(sead::Matrix34f* matrix, const AreaInitInfo& initInfo,
                         const char* linkName) {
    // TODO
}

bool tryGetLinksMatrixTRS(sead::Matrix34f* matrix, const ActorInitInfo& initInfo,
                          const char* linkName) {
    // TODO
}

bool tryGetLinksTrans(sead::Vector3f* trans, const ActorInitInfo& initInfo, const char* linkName) {
    // TODO
}

bool tryGetLinksTrans(sead::Vector3f* trans, const PlacementInfo& placementInfo,
                      const char* linkName) {
    // TODO
}

bool tryGetLinksQuat(sead::Quatf* quat, const ActorInitInfo& initInfo, const char* linkName) {
    // TODO
}

bool tryGetLinksTR(sead::Vector3f* trans, sead::Vector3f* rotate, const ActorInitInfo& initInfo,
                   const char* linkName) {
    // TODO
}

void getChildTrans(sead::Vector3f* trans, const PlacementInfo& placementInfo,
                   const char* linkName) {
    // TODO
}

void getChildTrans(sead::Vector3f* trans, const ActorInitInfo& initInfo, const char* linkName) {
    // TODO
}

void getChildTrans(sead::Vector3f* trans, const AreaInitInfo& initInfo, const char* linkName) {
    // TODO
}

void getChildLinkT(sead::Vector3f* trans, const ActorInitInfo& initInfo, const char* linkName,
                   s32 index) {
    // TODO
}

void getChildLinkTR(sead::Vector3f* trans, sead::Vector3f* rotate, const ActorInitInfo& initInfo,
                    const char* linkName, s32 index) {
    // TODO
}

s32 calcMatchNameLinkCount(const PlacementInfo& placementInfo, const char* linkName) {
    // TODO
}

s32 calcLinkCountClassName(const PlacementInfo& placementInfo, const char* linkName) {
    // TODO
}

bool tryGetZoneMatrixTR(sead::Matrix34f* matrix, const ActorInitInfo& initInfo) {
    // TODO
}

bool tryGetDisplayOffset(sead::Vector3f* offset, const ActorInitInfo& initInfo) {
    // TODO
}

bool tryGetDisplayOffset(sead::Vector3f* offset, const PlacementInfo& placementInfo) {
    // TODO
}

bool tryGetChildDisplayOffset(sead::Vector3f* offset, const ActorInitInfo& initInfo,
                              const char* linkName) {
    // TODO
}

bool tryGetDisplayRotate(sead::Vector3f* rotate, const ActorInitInfo& initInfo) {
    // TODO
}

bool tryGetDisplayScale(sead::Vector3f* scale, const ActorInitInfo& initInfo) {
    // TODO
}

}  // namespace al

namespace alPlacementFunction {

s32 getCameraId(const al::ActorInitInfo& initInfo) {
    // TODO
}

bool getLinkGroupId(al::PlacementId* groupId, const al::ActorInitInfo& initInfo,
                    const char* linkName) {
    // TODO
}

bool isEnableLinkGroupId(const al::ActorInitInfo& initInfo, const char* linkName) {
    // TODO
}

bool isEnableGroupClipping(const al::ActorInitInfo& initInfo) {
    // TODO
}

void getClippingGroupId(al::PlacementId* groupId, const al::ActorInitInfo& initInfo) {
    // TODO
}

void createClippingViewId(const al::PlacementInfo& placementInfo) {
    // TODO
}

void getClippingViewId(al::PlacementId* viewId, const al::PlacementInfo& placementInfo) {
    // TODO
}

void getClippingViewId(al::PlacementId* viewId, const al::ActorInitInfo& initInfo) {
    // TODO
}

void getModelName(const char** modelName, const al::ActorInitInfo& initInfo) {
    // TODO
}

void getModelName(const char** modelName, const al::PlacementInfo& placementInfo) {
    // TODO
}

bool tryGetModelName(const char** modelName, const al::PlacementInfo& placementInfo) {
    // TODO
}

bool tryGetModelName(const char** modelName, const al::ActorInitInfo& initInfo) {
    // TODO
}

}  // namespace alPlacementFunction
