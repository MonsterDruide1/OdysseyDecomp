#include "Library/Obj/PartsFunction.h"

#include <math/seadMatrix.h>

#include "Library/Area/AreaObjGroup.h"
#include "Library/Base/StringUtil.h"
#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Obj/BreakModel.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/HostSyncTowerCamera.h"
#include "Library/Obj/KeyMoveCameraObj.h"
#include "Library/Obj/PartsModel.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

inline const sead::Matrix34f* getJoinMtx(const LiveActor* parent, const char* joinMtxName) {
    return joinMtxName ? getJointMtxPtr(parent, joinMtxName) : parent->getBaseMtx();
}

CollisionObj* createCollisionObj(const LiveActor* parent, const ActorInitInfo& info,
                                 const char* collisionFileName, HitSensor* hitSensor,
                                 const char* joinMtxName, const char* suffix) {
    return new CollisionObj(info, getModelResource(parent), collisionFileName, hitSensor,
                            getJoinMtx(parent, joinMtxName), suffix);
}

CollisionObj* createCollisionObjMtx(const LiveActor* parent, const ActorInitInfo& info,
                                    const char* collisionFileName, HitSensor* hitSensor,
                                    const sead::Matrix34f* joinMtx, const char* suffix) {
    return new CollisionObj(info, getModelResource(parent), collisionFileName, hitSensor, joinMtx,
                            suffix);
}

PartsModel* createPartsModel(LiveActor* parent, const ActorInitInfo& info, const char* name,
                             const char* arcName, const sead::Matrix34f* joinMtx) {
    PartsModel* partsModel = new PartsModel(name);
    if (!joinMtx)
        joinMtx = parent->getBaseMtx();
    partsModel->initPartsMtx(parent, info, arcName, joinMtx, false);
    return partsModel;
}

PartsModel* createPartsModelFile(LiveActor* parent, const ActorInitInfo& info, const char* name,
                                 const char* arcName, const char* suffix) {
    PartsModel* partsModel = new PartsModel(name);
    partsModel->initPartsFixFile(parent, info, arcName, nullptr, suffix);
    StringTmp<128>("[PartsModel] %s", partsModel->getName()).cstr();
    return partsModel;
}

PartsModel* createPartsModelFileSuffix(LiveActor* parent, const ActorInitInfo& info,
                                       const char* name, const char* arcName, const char* arcSuffix,
                                       const char* suffix) {
    PartsModel* partsModel = new PartsModel(name);
    partsModel->initPartsFixFile(parent, info, arcName, arcSuffix, suffix);
    StringTmp<128>("[PartsModel] %s", partsModel->getName()).cstr();
    return partsModel;
}

PartsModel* createSimplePartsModel(LiveActor* parent, const ActorInitInfo& info, const char* name,
                                   const char* arcName, const char* suffix) {
    PartsModel* partsModel = new PartsModel(name);
    partsModel->initPartsFixFile(parent, info, arcName, nullptr, suffix);
    StringTmp<128>("[PartsModel] %s", partsModel->getName()).cstr();
    return partsModel;
}

PartsModel* createSimplePartsModelSuffix(LiveActor* parent, const ActorInitInfo& info,
                                         const char* name, const char* arcName,
                                         const char* arcSuffix, const char* suffix) {
    PartsModel* partsModel = new PartsModel(name);
    partsModel->initPartsFixFile(parent, info, arcName, arcSuffix, suffix);
    StringTmp<128>("[PartsModel] %s", partsModel->getName()).cstr();
    return partsModel;
}

PartsModel* createPartsModelSuffix(LiveActor* parent, const ActorInitInfo& info, const char* name,
                                   const char* arcName, const char* suffix,
                                   const sead::Matrix34f* joinMtx) {
    PartsModel* partsModel = new PartsModel(name);
    if (!joinMtx)
        joinMtx = parent->getBaseMtx();
    partsModel->initPartsSuffix(parent, info, arcName, suffix, joinMtx, false);
    return partsModel;
}

PartsModel* createPartsModelJoint(LiveActor* parent, const ActorInitInfo& info, const char* name,
                                  const char* arcName, const char* joinMtxName) {
    PartsModel* partsModel = new PartsModel(name);
    partsModel->initPartsMtx(parent, info, arcName, getJointMtxPtr(parent, joinMtxName), false);
    return partsModel;
}

PartsModel* createPartsModelSuffixJoint(LiveActor* parent, const ActorInitInfo& info,
                                        const char* name, const char* arcName,
                                        const char* arcSuffix, const char* joinMtxName) {
    PartsModel* partsModel = new PartsModel(name);
    partsModel->initPartsSuffix(parent, info, arcName, arcSuffix,
                                getJointMtxPtr(parent, joinMtxName), false);
    return partsModel;
}

void appearBreakModelRandomRotateY(LiveActor* actor) {
    actor->appear();
    addRotateAndRepeatY(actor, getRandomDegree());
}

bool updateSyncHostVisible(bool* isChildHidden, LiveActor* actor, const LiveActor* host, bool unk) {
    if (isDead(host) || isClipped(host) || isHideModel(host) || unk) {
        if (!*isChildHidden) {
            if (isExistModel(actor))
                alActorSystemFunction::removeFromExecutorDraw(actor);

            if (isExistShadow(actor))
                hideShadow(actor);

            *isChildHidden = true;
        }
        return false;
    }

    if (*isChildHidden) {
        if (isExistModel(actor))
            alActorSystemFunction::addToExecutorDraw(actor);
        if (isExistShadow(actor))
            showShadow(actor);
        *isChildHidden = false;
    }
    return true;
}

bool isTraceModelRandomRotate(const LiveActor* actor) {
    if (!isExistModelResourceYaml(actor, "InitTraceModel", nullptr))
        return false;

    return tryGetByamlKeyBoolOrFalse(getModelResourceYaml(actor, "InitTraceModel", nullptr),
                                     "IsRandomRotate");
}

BreakModel* createBreakModel(const LiveActor* parent, const ActorInitInfo& info,
                             const char* objName, const char* modelName, const char* fileSuffixName,
                             const sead::Matrix34f* rootMtx, const char* breakActionName) {
    BreakModel* breakModel =
        new BreakModel(parent, objName, modelName, fileSuffixName, rootMtx, breakActionName);
    initCreateActorWithPlacementInfo(breakModel, info);
    return breakModel;
}

HostSyncTowerCamera* createHostSyncTowerCamera(LiveActor* actor, const ActorInitInfo& info) {
    AreaObjGroup* areaObjGroup = createLinkAreaGroup(
        actor, info, "HostSyncTowerCameraArea", "アクター追従塔カメラ起動エリア", "子供エリア");
    return new HostSyncTowerCamera(actor, areaObjGroup, info);
}

void activateHostSyncTowerCamera(HostSyncTowerCamera* camera) {
    camera->activate();
}

void deactivateHostSyncTowerCamera(HostSyncTowerCamera* camera) {
    camera->deactivate();
}

void updateHostSyncTowerCamera(HostSyncTowerCamera* camera) {
    camera->update();
}

KeyMoveCameraObj* tryCreateLinksKeyMoveCameraObj(const ActorInitInfo& info, const char* childName) {
    if (!isExistLinkChild(info, childName, 0))
        return nullptr;

    KeyMoveCameraObj* cameraObj = new KeyMoveCameraObj("キー移動カメラオブジェ");
    initLinksActor(cameraObj, info, childName, 0);
    cameraObj->validateStartAtNearestPosition();
    cameraObj->makeActorDead();
    return cameraObj;
}

KeyMoveCameraObj* tryCreateLinksKeyMoveCameraObjWithInterpole(const ActorInitInfo& info,
                                                              const char* childName) {
    if (!isExistLinkChild(info, childName, 0))
        return nullptr;

    KeyMoveCameraObj* cameraObj = new KeyMoveCameraObj("キー移動カメラオブジェ");
    initLinksActor(cameraObj, info, childName, 0);
    cameraObj->validateStartAtNearestPosition();
    cameraObj->makeActorDead();
    cameraObj->setFirstCameraStartInterpoleStepDefault();
    return cameraObj;
}

}  // namespace al
