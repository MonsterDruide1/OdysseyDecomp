#include "Library/LiveActor/ActorInitUtil.h"

#include <math/seadBoundBox.h>
#include <nn/g3d/ModelObj.h>

#include "Library/Action/ActorActionKeeper.h"
#include "Library/Base/HashCodeUtil.h"
#include "Library/Base/Macros.h"
#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraDirector.h"
#include "Library/Draw/GraphicsQualityController.h"
#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Effect/PartsEffectGroup.h"
#include "Library/Factory/Factory.h"
#include "Library/HitSensor/HitSensorKeeper.h"
#include "Library/HitSensor/SensorFunction.h"
#include "Library/Light/ModelMaterialCategory.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFactory.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Model/ModelLodCtrl.h"
#include "Library/Model/ModelOcclusionQuery.h"
#include "Library/Nerve/NerveAction.h"
#include "Library/Nerve/NerveActionCtrl.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Obj/ActorDitherAnimator.h"
#include "Library/Obj/FarDistanceDitherAnimator.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Resource/ActorResource.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"
#include "Project/HitSensor/HitSensor.h"
#include "Project/Item/ActorScoreKeeper.h"

namespace al {

using PoseKeeperCreatorFunction = void (*)(LiveActor*);

const NameToCreator<PoseKeeperCreatorFunction> gActorPoseTable[] = {
    {"TRSV", initActorPoseTRSV}, {"TRMSV", initActorPoseTRMSV}, {"TRGMSV", initActorPoseTRGMSV},
    {"TFSV", initActorPoseTFSV}, {"TFUSV", initActorPoseTFUSV}, {"TFGSV", initActorPoseTFGSV},
    {"TQSV", initActorPoseTQSV}, {"TQGSV", initActorPoseTQGSV}, {"TQGMSV", initActorPoseTQGMSV},
};

void initActor(LiveActor* actor, const ActorInitInfo& initInfo) {
    initActorSuffix(actor, initInfo, nullptr);
}

ALWAYS_INLINE bool initActorPoseKeeper(const char* pose, LiveActor* actor) {
    if (!pose)
        return false;
    s32 poseId = -1;
    for (s32 i = 0; i < 9; i++) {
        if (isEqualString(gActorPoseTable[i].name, pose)) {
            poseId = i;
            break;
        }
    }
    if (poseId != -1) {
        gActorPoseTable[poseId].creationFunction(actor);
        return true;
    }
    return false;
}

ALWAYS_INLINE void initActorPose(LiveActor* actor, const ActorInitInfo& initInfo,
                                 Resource* modelRes, const char* suffix) {
    ByamlIter initPose;
    if (!tryGetActorInitFileIter(&initPose, modelRes, "InitPose", suffix))
        return;

    const char* pose = nullptr;
    if (!initPose.tryGetStringByKey(&pose, "Pose"))
        return;

    if (!initActorPoseKeeper(pose, actor))
        return;

    if (isEqualString(pose, "TFUSV") && tryGetByamlKeyBoolOrFalse(initPose, "IsFrontUp"))
        ((ActorPoseKeeperTFUSV*)actor->getPoseKeeper())->setFrontUp(true);
}

ALWAYS_INLINE void initActorScale(LiveActor* actor, const ActorInitInfo& initInfo,
                                  Resource* modelRes, const char* suffix) {
    ByamlIter initScale;
    if (!tryGetActorInitFileIter(&initScale, modelRes, "InitScale", suffix))
        return;

    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    if (!tryGetByamlScale(&scale, initScale, "Scale"))
        return;

    const sead::Vector3f& prevScale = getScale(actor);
    scale.x = scale.x * prevScale.x;
    scale.y = scale.y * prevScale.y;
    scale.z = scale.z * prevScale.z;
    setScale(actor, scale);
}

ALWAYS_INLINE ModelLodCtrl* initActorModelLodCtrl(LiveActor* actor, Resource* modelRes,
                                                  const char* suffix) {
    ByamlIter initLod;
    if (!tryGetActorInitFileIter(&initLod, modelRes, "InitLod", suffix))
        return nullptr;

    sead::BoundBox3f modelBoundingBox;
    s32 lodModelCount = 5;
    if (isExistModel(actor)) {
        lodModelCount = getLodModelCount(actor);
        calcModelBoundingBox(&modelBoundingBox, actor);
    }

    ModelLodCtrl* modelLodCtrl = new ModelLodCtrl{
        actor,
        getTransPtr(actor),
        actor->getBaseMtx(),
        getScalePtr(actor),
        modelBoundingBox,
        lodModelCount,
    };
    modelLodCtrl->init(initLod);

    return modelLodCtrl;
}

ALWAYS_INLINE void initActorModel(LiveActor* actor, const ActorInitInfo& initInfo,
                                  Resource* modelRes, ActorResource* actorResource,
                                  const char* suffix) {
    ByamlIter initModel;
    if (!tryGetActorInitFileIter(&initModel, modelRes, "InitModel", suffix))
        return;

    s32 blendAnimMax = 1;
    initModel.tryGetIntByKey(&blendAnimMax, "BlendAnimMax");
    const char* displayRootJointName = nullptr;
    initModel.tryGetStringByKey(&displayRootJointName, "DisplayRootJointName");

    initActorModelKeeper(actor, initInfo, actorResource, blendAnimMax);
    if (displayRootJointName)
        actor->getModelKeeper()->setDisplayRootJointMtxPtr(
            getJointMtxPtr(actor, displayRootJointName));

    bool isCreateUniqShader = false;
    initModel.tryGetBoolByKey(&isCreateUniqShader, "IsCreateUniqShader");
    if (isCreateUniqShader)
        createUniqueShader(actor);

    s32 partialAnimSlotNum = 0;
    initModel.tryGetIntByKey(&partialAnimSlotNum, "PartialAnimSlotNum");
    s32 partialAnimGroupNum = partialAnimSlotNum;
    initModel.tryGetIntByKey(&partialAnimGroupNum, "PartialAnimGroupNum");
    s32 partialAnimPartsListBufferSize = 16;
    initModel.tryGetIntByKey(&partialAnimPartsListBufferSize, "PartialAnimPartsListBufferSize");
    if (partialAnimGroupNum >= 1)
        initPartialSklAnim(actor, partialAnimSlotNum, partialAnimGroupNum,
                           partialAnimPartsListBufferSize);

    bool isFixedModel = false;
    initModel.tryGetBoolByKey(&isFixedModel, "IsFixedModel");
    if (isFixedModel)
        setFixedModelFlag(actor);

    bool isIgnoreUpdateOnDrawClipping = false;
    initModel.tryGetBoolByKey(&isIgnoreUpdateOnDrawClipping, "IsIgnoreUpdateOnDrawClipping");
    if (isIgnoreUpdateOnDrawClipping)
        setIgnoreUpdateDrawClipping(actor, true);

    ModelKeeper* modelKeeper = actor->getModelKeeper();

    ModelLodCtrl* modelLodCtrl = initActorModelLodCtrl(actor, modelRes, suffix);
    modelKeeper->setModelLodCtrl(modelLodCtrl);
    if (modelLodCtrl)
        initInfo.actorSceneInfo.graphicsSystemInfo->getModelLodAllCtrl()->registerLodCtrl(
            modelLodCtrl);

    MaterialCategoryKeeper* materialCategoryKeeper =
        initInfo.actorSceneInfo.graphicsSystemInfo->getMaterialCategoryKeeper();
    ModelMaterialCategory::tryCreate(modelKeeper->getModelCtrl(), modelRes, suffix,
                                     materialCategoryKeeper);

    GraphicsQualityInfo* graphicsQualityInfo =
        initInfo.actorSceneInfo.graphicsSystemInfo->getGraphicsQualityController()
            ->getGraphicsQualityInfo();
    modelKeeper->getModelCtrl()->setGraphicsQualityInfo(graphicsQualityInfo);

    ModelOcclusionQuery* modelOcclusionQuery =
        ModelOcclusionQuery::tryCreate(actor, modelRes, suffix);
    modelKeeper->getModelCtrl()->setModelOcclusionQuery(modelOcclusionQuery);

    bool isCalcViewCore1 = false;
    initModel.tryGetBoolByKey(&isCalcViewCore1, "IsCalcViewCore1");
    if (isCalcViewCore1)
        modelKeeper->getModelCtrl()->setCalcViewCore(1);

    bool isCalcViewCore2 = false;
    initModel.tryGetBoolByKey(&isCalcViewCore2, "IsCalcViewCore2");
    if (isCalcViewCore2)
        modelKeeper->getModelCtrl()->setCalcViewCore(2);
}

ALWAYS_INLINE void initActorExecutor(LiveActor* actor, const ActorInitInfo& initInfo,
                                     Resource* modelRes, const char* suffix) {
    ByamlIter initExecutor;
    if (!tryGetActorInitFileIter(&initExecutor, modelRes, "InitExecutor", suffix)) {
        ModelKeeper* modelKeeper = actor->getModelKeeper();
        if (modelKeeper && modelKeeper->getModelCtrl()->getModelObj()->GetNumShapes())
            initExecutorModelUpdate(actor, initInfo);
        return;
    }

    const char* updaterCategoryName = nullptr;
    const char* drawerCategoryName = nullptr;

    ByamlIter iter;
    if (initExecutor.tryGetIterByKey(&iter, "Updater") && iter.isTypeArray()) {
        ByamlIter entry;
        s32 index = 0;
        while (iter.tryGetIterByIndex(&entry, index)) {
            entry.tryGetStringByKey(&updaterCategoryName, "CategoryName");
            initExecutorUpdate(actor, initInfo, updaterCategoryName);
            index++;
        }
    }

    if (initExecutor.tryGetIterByKey(&iter, "Drawer")) {
        ModelKeeper* modelKeeper = actor->getModelKeeper();
        if (modelKeeper && modelKeeper->getModelCtrl()->getModelObj()->GetNumShapes())
            initExecutorModelUpdate(actor, initInfo);

        if (iter.isTypeArray()) {
            ByamlIter entry;
            s32 index = 0;
            while (iter.tryGetIterByIndex(&entry, index)) {
                entry.tryGetStringByKey(&drawerCategoryName, "CategoryName");
                initExecutorDraw(actor, initInfo, drawerCategoryName);
                index++;
            }
        }
    }
}

ALWAYS_INLINE void initActorSensor(LiveActor* actor, const ActorInitInfo& initInfo,
                                   Resource* modelRes, const char* suffix) {
    ByamlIter initSensor;
    if (!tryGetActorInitFileIter(&initSensor, modelRes, "InitSensor", suffix))
        return;

    s32 numSensors = initSensor.getSize();
    if (numSensors <= 0)
        return;

    actor->initHitSensor(numSensors);
    for (s32 i = 0; i < numSensors; i++) {
        ByamlIter sensor;
        if (!initSensor.tryGetIterByIndex(&sensor, i))
            continue;

        const char* sensorName = nullptr;
        if (!sensor.tryGetStringByKey(&sensorName, "Name"))
            continue;

        const char* sensorType = nullptr;
        if (!sensor.tryGetStringByKey(&sensorType, "Type"))
            continue;

        f32 radius = 0.0f;
        sensor.tryGetFloatByKey(&radius, "Radius");

        s32 maxCount = 8;
        sensor.tryGetIntByKey(&maxCount, "MaxCount");

        sead::Vector3f offset = sead::Vector3f::zero;
        tryGetByamlV3f(&offset, sensor);

        HitSensorType type = alSensorFunction::findSensorTypeByName(sensorType);
        if (type == HitSensorType::CollisionParts)
            maxCount = 0;

        addHitSensor(actor, initInfo, sensorName, (u32)type, radius, maxCount, offset);

        const char* joint = nullptr;
        sensor.tryGetStringByKey(&joint, "Joint");
        if (joint)
            setHitSensorJointMtx(actor, sensorName, joint);
    }
}

ALWAYS_INLINE void initActorCollision(LiveActor* actor, const ActorInitInfo& initInfo,
                                      Resource* modelRes, const char* suffix) {
    ByamlIter initCollision;
    if (!tryGetActorInitFileIter(&initCollision, modelRes, "InitCollision", suffix))
        return;

    const char* name = nullptr;
    initCollision.tryGetStringByKey(&name, "Name");
    StringTmp<256> unused;

    if (!name)
        name = getBaseName(modelRes->getArchiveName());

    const char* sensorName = nullptr;
    HitSensor* sensor = nullptr;
    if (initCollision.tryGetStringByKey(&sensorName, "Sensor"))
        sensor = getHitSensor(actor, sensorName);

    const char* joint = nullptr;
    initCollision.tryGetStringByKey(&joint, "Joint");
    sead::Matrix34f* jointMtx = nullptr;
    if (joint)
        jointMtx = getJointMtxPtr(actor, joint);

    initActorCollisionWithResource(actor, modelRes, name, sensor, jointMtx, suffix);
}

ALWAYS_INLINE void initActorCollider(LiveActor* actor, const ActorInitInfo& initInfo,
                                     Resource* modelRes, const char* suffix) {
    ByamlIter initCollider;
    if (!tryGetActorInitFileIter(&initCollider, modelRes, "InitCollider", suffix))
        return;

    f32 radius = 0.0f;
    initCollider.tryGetFloatByKey(&radius, "Radius");

    u32 planeNum = 0;
    initCollider.tryGetUIntByKey(&planeNum, "PlaneNum");

    sead::Vector3f offset = sead::Vector3f::zero;
    tryGetByamlV3f(&offset, initCollider);

    actor->initCollider(radius, offset.y, planeNum);
}

ALWAYS_INLINE void initActorEffect(LiveActor* actor, const ActorInitInfo& initInfo,
                                   Resource* modelRes, const char* suffix) {
    ByamlIter initEffect;
    if (!tryGetActorInitFileIter(&initEffect, modelRes, "InitEffect", suffix))
        return;

    const char* name = nullptr;
    if (!initEffect.tryGetStringByKey(&name, "Name"))
        return;

    initActorEffectKeeper(actor, initInfo, name);
}

ALWAYS_INLINE void initActorSound(LiveActor* actor, const ActorInitInfo& initInfo,
                                  Resource* modelRes, const char* suffix) {
    ByamlIter initSound;
    const char* seName = nullptr;
    const char* bgmName = nullptr;
    if (tryGetActorInitFileIter(&initSound, modelRes, "InitSound", suffix)) {
        initSound.tryGetStringByKey(&seName, "Name");
    } else if (tryGetActorInitFileIter(&initSound, modelRes, "InitAudio", suffix)) {
        initSound.tryGetStringByKey(&seName, "SeUserName");
        initSound.tryGetStringByKey(&bgmName, "BgmUserName");
    } else
        return;

    if (seName)
        initActorSeKeeper(actor, initInfo, seName);
    if (initSound.isExistKey("BgmUserName"))
        initActorBgmKeeper(actor, initInfo, bgmName);
}

ALWAYS_INLINE void initActorRail(LiveActor* actor, const ActorInitInfo& initInfo) {
    if (isExistRail(initInfo, "Rail"))
        actor->initRailKeeper(initInfo, "Rail");
}

ALWAYS_INLINE void initActorGroupClipping(LiveActor* actor, const ActorInitInfo& initInfo,
                                          const ByamlIter& initClipping) {
    if (initClipping.isExistKey("NoGroupClipping"))
        return;

    ByamlIter groupClipping;
    if (!initClipping.tryGetIterByKey(&groupClipping, "GroupClipping"))
        return;

    initGroupClipping(actor, initInfo);
}

ALWAYS_INLINE void initActorClipping(LiveActor* actor, const ActorInitInfo& initInfo,
                                     Resource* modelRes, const char* suffix) {
    ByamlIter initClipping;
    if (!tryGetActorInitFileIter(&initClipping, modelRes, "InitClipping", suffix))
        return;
    initActorClipping(actor, initInfo);

    bool invalidate = false;
    initClipping.tryGetBoolByKey(&invalidate, "Invalidate");
    if (invalidate)
        invalidateClipping(actor);

    f32 radius = 0.0f;
    if (initClipping.tryGetFloatByKey(&radius, "Radius")) {
        setClippingInfo(actor, radius, nullptr);
    } else if (actor->getModelKeeper()) {
        const sead::Vector3f& scale = getScale(actor);
        f32 maxXY = sead::Mathf::max(sead::Mathf::abs(scale.x), sead::Mathf::abs(scale.y));
        f32 maxXYZ = sead::Mathf::max(maxXY, sead::Mathf::abs(scale.z));

        setClippingInfo(actor, calcModelBoundingSphereRadius(actor) * maxXYZ, nullptr);
    }

    sead::BoundBox3f obb;
    if (tryGetByamlBox3f(&obb, initClipping, "Obb"))
        setClippingObb(actor, obb);

    f32 nearClipDistance = 0.0f;
    if (initClipping.tryGetFloatByKey(&nearClipDistance, "NearClipDistance"))
        setClippingNearDistance(actor, nearClipDistance);

    initActorGroupClipping(actor, initInfo, initClipping);
}

ALWAYS_INLINE void initActorShadowMask(LiveActor* actor, const ActorInitInfo& initInfo,
                                       Resource* modelRes, const char* suffix) {
    bool usingDepthShadow = false;
    tryGetArg(&usingDepthShadow, initInfo, "UsingDepthShadow");
    initDepthShadowMapCtrl(actor, modelRes, initInfo, suffix);

    ByamlIter initShadowMask;
    if (!tryGetActorInitFileIter(&initShadowMask, modelRes, "InitShadowMask", suffix))
        return;

    initShadowMaskCtrl(actor, initInfo, initShadowMask, "InitShadowMask");
    if (usingDepthShadow)
        invalidateShadowMaskIntensityAll(actor);
}

ALWAYS_INLINE void initActorFlag(LiveActor* actor, const ActorInitInfo& initInfo,
                                 Resource* modelRes, const char* suffix) {
    ByamlIter initFlag;
    if (!tryGetActorInitFileIter(&initFlag, modelRes, "InitFlag", suffix))
        return;

    ByamlIter tmp;
    if (initFlag.tryGetIterByKey(&tmp, "MaterialCode"))
        validateMaterialCode(actor);
    if (initFlag.tryGetIterByKey(&tmp, "UpdatePuddleMaterial"))
        validatePuddleMaterial(actor);
}

ALWAYS_INLINE void initActorItem(LiveActor* actor, const ActorInitInfo& initInfo,
                                 Resource* modelRes, const char* suffix) {
    ByamlIter initItem;
    if (!tryGetActorInitFileIter(&initItem, modelRes, "InitItem", suffix))
        return;

    initActorItemKeeper(actor, initInfo, initItem);
}

ALWAYS_INLINE void initActorScore(LiveActor* actor, const ActorInitInfo& initInfo,
                                  Resource* modelRes, const char* suffix) {
    ByamlIter initScore;
    if (!tryGetActorInitFileIter(&initScore, modelRes, "InitScore", suffix))
        return;

    actor->initScoreKeeper();
    actor->getActorScoreKeeper()->init(initScore);
}

ALWAYS_INLINE void initActorAction(LiveActor* actor, const ActorInitInfo& initInfo,
                                   Resource* modelRes, ActorResource* actorResource,
                                   const char* suffix) {
    const char* archiveName = actorResource->getModelRes()->getArchiveName();
    initActorActionKeeper(actor, actorResource, archiveName, suffix);

    if (!actor->getModelKeeper())
        return;

    if (!tryStartAction(actor, archiveName)) {
        ActorActionKeeper* actionKeeper = actor->getActorActionKeeper();
        if (actionKeeper)
            actionKeeper->startAction(archiveName);
    }

    ModelKeeper* modelKeeper = actor->getModelKeeper();
    if (!modelKeeper)
        return;

    DitherAnimator* ditherAnimator = ActorDitherAnimator::tryCreate(actor, modelRes, suffix);
    if (ditherAnimator) {
        modelKeeper->setDitherAnimator(ditherAnimator);
        return;
    }

    ditherAnimator = FarDistanceDitherAnimator::tryCreate(actor, modelRes, suffix);
    if (ditherAnimator)
        modelKeeper->setDitherAnimator(ditherAnimator);
}

void initActorImpl(LiveActor* actor, const ActorInitInfo& initInfo,
                   const sead::SafeString& folderName, const sead::SafeString& fileName,
                   const char* suffix) {
    StringTmp<256> path = {"%s/%s", folderName.cstr(), fileName.cstr()};
    ActorResource* actorResource =
        findOrCreateActorResource(initInfo.actorResourceHolder, path.cstr(), suffix);
    Resource* modelRes = actorResource->getModelRes();
    if (!actor->getSceneInfo())
        initActorSceneInfo(actor, initInfo);

    initActorPose(actor, initInfo, modelRes, suffix);
    initActorSRT(actor, initInfo);
    initActorScale(actor, initInfo, modelRes, suffix);
    initActorModel(actor, initInfo, actorResource->getModelRes(), actorResource, suffix);
    initActorPrePassLightKeeper(actor, modelRes, initInfo, suffix);
    initActorExecutor(actor, initInfo, modelRes, suffix);
    initActorSensor(actor, initInfo, modelRes, suffix);
    initActorCollision(actor, initInfo, modelRes, suffix);
    initActorCollider(actor, initInfo, modelRes, suffix);
    initActorEffect(actor, initInfo, modelRes, suffix);
    initActorSound(actor, initInfo, modelRes, suffix);
    initActorRail(actor, initInfo);
    initStageSwitch(actor, initInfo);
    initActorClipping(actor, initInfo, modelRes, suffix);
    initActorOcclusionKeeper(actor, modelRes, initInfo, suffix);
    initActorShadowMask(actor, initInfo, modelRes, suffix);
    initActorFlag(actor, initInfo, modelRes, suffix);
    initActorItem(actor, initInfo, modelRes, suffix);
    initActorScore(actor, initInfo, modelRes, suffix);
    initScreenPointKeeper(actor, modelRes, initInfo, suffix);
    initHitReactionKeeper(actor, modelRes, suffix);
    initActorParamHolder(actor, modelRes, suffix);
    initActorAction(actor, initInfo, modelRes, actorResource, suffix);

    if (actor->getNerveKeeper() && actor->getNerveKeeper()->getActionCtrl())
        resetNerveActionForInit(actor);

    if (!actor->getSubActorKeeper())
        initSubActorKeeper(actor, initInfo, suffix, 0);
}

void initActorSuffix(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix) {
    const char* objName = nullptr;
    tryGetObjectName(&objName, initInfo);
    initActorImpl(actor, initInfo, "ObjectData", objName, suffix);
}

void initActorChangeModel(LiveActor* actor, const ActorInitInfo& initInfo) {
    initActorChangeModelSuffix(actor, initInfo, nullptr);
}

inline const char* getModelName(const ActorInitInfo& initInfo) {
    const char* name = nullptr;
    if (alPlacementFunction::tryGetModelName(&name, initInfo))
        return name;
    else if (tryGetObjectName(&name, initInfo))
        return name;
    return nullptr;
}

void initActorChangeModelSuffix(LiveActor* actor, const ActorInitInfo& initInfo,
                                const char* suffix) {
    const char* file = getModelName(initInfo);
    initActorImpl(actor, initInfo, "ObjectData", file, suffix);
}

void initActorWithArchiveName(LiveActor* actor, const ActorInitInfo& initInfo,
                              const sead::SafeString& archiveName, const char* suffix) {
    initChildActorWithArchiveNameWithPlacementInfo(actor, initInfo, archiveName, suffix);
}

void initChildActorWithArchiveNameWithPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                                    const sead::SafeString& archiveName,
                                                    const char* suffix) {
    initActorImpl(actor, initInfo, "ObjectData", archiveName.cstr(), suffix);
}

void initChildActorWithArchiveNameNoPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                                  const sead::SafeString& archiveName,
                                                  const char* suffix) {
    PlacementInfo placementInfo;
    ActorInitInfo childInitInfo;
    childInitInfo.initViewIdHost(&placementInfo, initInfo);
    initActorImpl(actor, childInitInfo, "ObjectData", archiveName.cstr(), suffix);
}

LiveActor* createChildLinkSimpleActor(const char* actorName, const char* linkName,
                                      const ActorInitInfo& initInfo, bool alive) {
    ActorInitInfo childInitInfo;
    PlacementInfo placementInfo;
    getLinksInfoByIndex(&placementInfo, *initInfo.placementInfo, linkName, 0);
    childInitInfo.initViewIdSelf(&placementInfo, initInfo);

    LiveActor* actor = new LiveActor(actorName);
    initActor(actor, childInitInfo);

    if (alive)
        actor->makeActorAlive();
    else
        actor->makeActorDead();
    return actor;
}

LiveActor* createChildLinkMapPartsActor(const char* actorName, const char* linkName,
                                        const ActorInitInfo& initInfo, s32 linkIndex, bool alive) {
    ActorInitInfo childInitInfo;
    PlacementInfo placementInfo;
    getLinksInfoByIndex(&placementInfo, *initInfo.placementInfo, linkName, linkIndex);
    childInitInfo.initViewIdSelf(&placementInfo, initInfo);

    LiveActor* actor = new LiveActor(actorName);
    initMapPartsActor(actor, childInitInfo, nullptr);

    if (alive)
        actor->makeActorAlive();
    else
        actor->makeActorDead();
    return actor;
}

void initMapPartsActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix) {
    const char* modelName;
    StringTmp<256> fileName;
    StringTmp<256> folderName;

    const PlacementInfo& placementInfo = *initInfo.placementInfo;
    modelName = nullptr;
    if (alPlacementFunction::tryGetModelName(&modelName, placementInfo) &&
        !isEqualString(modelName, "")) {
        fileName.copy(modelName);
        folderName.copy("ObjectData");
    } else {
        tryGetStringArg(&modelName, placementInfo, "UnitConfigName");
        fileName.copy(modelName);
        folderName.copy("ObjectData");
    }

    initActorImpl(actor, initInfo, folderName, fileName, suffix);
}

void initLinksActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix,
                    s32 linkIndex) {
    ActorInitInfo childInitInfo;
    PlacementInfo placementInfo;
    getLinksInfoByIndex(&placementInfo, *initInfo.placementInfo, suffix, linkIndex);
    childInitInfo.initViewIdSelf(&placementInfo, initInfo);
    actor->init(childInitInfo);
}

ActorInitInfo* createLinksPlayerActorInfo(LiveActor* actor, const ActorInitInfo& initInfo) {
    s32 startPosNum = calcLinkChildNum(initInfo, "PlayerRestartPos");
    ActorInitInfo* result = new ActorInitInfo();
    if (startPosNum == 1) {
        PlacementInfo* placementInfo = new PlacementInfo();
        getLinksInfoByIndex(placementInfo, *initInfo.placementInfo, "PlayerRestartPos", 0);
        result->initViewIdSelf(placementInfo, initInfo);
    }
    return result;
}

const char* getLinksActorClassName(const ActorInitInfo& initInfo, const char* linkName,
                                   s32 linkIndex) {
    ActorInitInfo childInitInfo;
    PlacementInfo placementInfo;

    const char* className = nullptr;
    getLinksInfoByIndex(&placementInfo, *initInfo.placementInfo, linkName, linkIndex);
    childInitInfo.initViewIdSelf(&placementInfo, initInfo);
    getClassName(&className, childInitInfo);
    return className;
}

const char* getLinksActorDisplayName(const ActorInitInfo& initInfo, const char* linkName,
                                     s32 linkIndex) {
    PlacementInfo placementInfo;
    getLinksInfoByIndex(&placementInfo, *initInfo.placementInfo, linkName, linkIndex);
    const char* displayName = nullptr;
    getDisplayName(&displayName, placementInfo);
    return displayName;
}

const char* getLinksActorObjectName(const ActorInitInfo& initInfo, const char* linkName,
                                    s32 linkIndex) {
    PlacementInfo placementInfo;
    getLinksInfoByIndex(&placementInfo, *initInfo.placementInfo, linkName, linkIndex);
    const char* objectName = nullptr;
    getObjectName(&objectName, placementInfo);
    return objectName;
}

void initCreateActorWithPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo) {
    actor->init(initInfo);
}

void initCreateActorWithPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                      const PlacementInfo& placementInfo) {
    ActorInitInfo childInitInfo;
    childInitInfo.initViewIdSelf(&placementInfo, initInfo);
    actor->init(childInitInfo);
}

void initCreateActorNoPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo) {
    PlacementInfo placementInfo;
    ActorInitInfo childInitInfo;
    childInitInfo.initViewIdHost(&placementInfo, initInfo);
    actor->init(childInitInfo);
}

void initCreateActorNoPlacementInfoNoViewId(LiveActor* actor, const ActorInitInfo& initInfo) {
    PlacementInfo placementInfo;
    ActorInitInfo childInitInfo;
    childInitInfo.initNoViewId(&placementInfo, initInfo);
    actor->init(childInitInfo);
}

ALWAYS_INLINE LiveActor* createActorFromFactory(const ActorInitInfo& childInitInfo,
                                                const PlacementInfo* placementInfo) {
    const ActorFactory* factory = childInitInfo.actorFactory;

    const char* objectName = nullptr;
    getObjectName(&objectName, childInitInfo);
    const char* className = nullptr;
    getClassName(&className, childInitInfo);

    ActorCreatorFunction creationFunction = nullptr;
    factory->getEntryIndex(&creationFunction, className);
    if (!creationFunction)
        return nullptr;

    const char* displayName;
    getDisplayName(&displayName, childInitInfo);
    LiveActor* actor = (*creationFunction)(displayName);
    actor->init(childInitInfo);
    return actor;
}

LiveActor* createPlacementActorFromFactory(const ActorInitInfo& initInfo,
                                           const PlacementInfo* placementInfo) {
    ActorInitInfo childInitInfo;
    childInitInfo.initViewIdSelf(placementInfo, initInfo);
    return createActorFromFactory(childInitInfo, placementInfo);
}

LiveActor* createLinksActorFromFactory(const ActorInitInfo& initInfo, const char* linkName,
                                       s32 linkNum) {
    ActorInitInfo childInitInfo;
    PlacementInfo placementInfo;
    getLinksInfoByIndex(&placementInfo, *initInfo.placementInfo, linkName, linkNum);
    childInitInfo.initViewIdSelf(&placementInfo, initInfo);
    return createActorFromFactory(childInitInfo, &placementInfo);
}

LiveActorGroup* createLinksActorGroupFromFactory(const ActorInitInfo& initInfo,
                                                 const char* linkName, const char* groupName) {
    return tryCreateLinksActorGroupFromFactory(initInfo, linkName, groupName);
}

LiveActorGroup* tryCreateLinksActorGroupFromFactory(const ActorInitInfo& initInfo,
                                                    const char* linkName, const char* groupName) {
    ActorInitInfo childInitInfo;
    PlacementInfo placementInfo;
    s32 linkChildNum = calcLinkChildNum(initInfo, linkName);
    if (linkChildNum < 1)
        return nullptr;

    LiveActorGroup* group = new LiveActorGroup(groupName, linkChildNum);
    for (s32 i = 0; i < linkChildNum; i++) {
        getLinksInfoByIndex(&placementInfo, *initInfo.placementInfo, linkName, i);
        childInitInfo.initViewIdSelf(&placementInfo, initInfo);
        group->registerActor(createActorFromFactory(childInitInfo, &placementInfo));
    }
    return group;
}

LiveActor* tryCreateLinksActorFromFactorySingle(const ActorInitInfo& initInfo,
                                                const char* linkName) {
    if (calcLinkChildNum(initInfo, linkName) < 1)
        return nullptr;
    return createLinksActorFromFactory(initInfo, linkName, 0);
}

void createAndRegisterLinksActorFromFactory(LiveActorGroup* group, const ActorInitInfo& initInfo,
                                            const char* linkName) {
    ActorInitInfo childInitInfo;
    PlacementInfo placementInfo;
    s32 linkChildNum = calcLinkChildNum(initInfo, linkName);
    if (linkChildNum < 1)
        return;

    for (s32 i = 0; i < linkChildNum; i++) {
        getLinksInfoByIndex(&placementInfo, *initInfo.placementInfo, linkName, i);
        childInitInfo.initViewIdSelf(&placementInfo, initInfo);
        group->registerActor(createActorFromFactory(childInitInfo, &placementInfo));
    }
}

void makeMapPartsModelName(sead::BufferedSafeString* modelName, sead::BufferedSafeString* path,
                           const PlacementInfo& placementInfo) {
    const char* archiveName = nullptr;
    if (alPlacementFunction::tryGetModelName(&archiveName, placementInfo) &&
        !isEqualString(archiveName, "")) {
        modelName->copy(archiveName);
        path->format("ObjectData/%s", archiveName);
    } else {
        tryGetStringArg(&archiveName, placementInfo, "UnitConfigName");
        modelName->copy(archiveName);
        path->format("ObjectData/%s", archiveName);
    }
}

void makeMapPartsModelName(sead::BufferedSafeString* modelName, sead::BufferedSafeString* path,
                           const ActorInitInfo& initInfo) {
    makeMapPartsModelName(modelName, path, *initInfo.placementInfo);
}

const char* tryGetMapPartsSuffix(const ActorInitInfo& initInfo, const char* suffix) {
    StringTmp<64> name = {"InitActor%s", suffix};
    if (!tryGetMapPartsResourceYaml(initInfo, name.cstr()))
        return nullptr;
    return suffix;
}

void initMapPartsActorWithArchiveName(LiveActor* actor, const ActorInitInfo& initInfo,
                                      const char* fileName, const char* suffix) {
    return initActorImpl(actor, initInfo, "MapPartsData", fileName, suffix);
}

void initNerve(LiveActor* actor, const Nerve* nerve, s32 maxStates) {
    actor->initNerveKeeper(new NerveKeeper(actor, nerve, maxStates));
}

void initNerveAction(LiveActor* actor, const char* actionName,
                     alNerveFunction::NerveActionCollector* collector, s32 maxStates) {
    auto* nerveActionCtrl = new NerveActionCtrl(collector);
    initNerve(actor, nerveActionCtrl->findNerve(actionName), maxStates);
    actor->getNerveKeeper()->initNerveAction(nerveActionCtrl);
    startNerveAction(actor, actionName);
}

bool trySyncStageSwitchAppear(LiveActor* actor) {
    using LiveActorFunctor = FunctorV0M<LiveActor*, void (LiveActor::*)()>;

    if (listenStageSwitchOnOffAppear(actor, LiveActorFunctor{actor, &LiveActor::appear},
                                     LiveActorFunctor{actor, &LiveActor::kill})) {
        actor->makeActorDead();
        return true;
    } else {
        actor->makeActorAlive();
        return false;
    }
}

bool trySyncStageSwitchKill(LiveActor* actor) {
    using LiveActorFunctor = FunctorV0M<LiveActor*, void (LiveActor::*)()>;

    bool result = listenStageSwitchOnOffKill(actor, LiveActorFunctor(actor, &LiveActor::kill),
                                             LiveActorFunctor(actor, &LiveActor::appear));
    actor->makeActorAlive();

    return result;
}

bool trySyncStageSwitchAppearAndKill(LiveActor* actor) {
    if (trySyncStageSwitchAppear(actor))
        return true;
    return trySyncStageSwitchKill(actor);
}

bool tryListenStageSwitchAppear(LiveActor* actor) {
    using LiveActorFunctor = FunctorV0M<LiveActor*, void (LiveActor::*)()>;

    if (listenStageSwitchOnAppear(actor, LiveActorFunctor{actor, &LiveActor::appear})) {
        actor->makeActorDead();
        return true;
    } else {
        actor->makeActorAlive();
        return false;
    }
}

bool tryListenStageSwitchKill(LiveActor* actor) {
    using LiveActorFunctor = FunctorV0M<LiveActor*, void (LiveActor::*)()>;

    return listenStageSwitchOnKill(actor, LiveActorFunctor(actor, &LiveActor::kill));
}

void syncSensorScaleY(LiveActor* actor) {
    f32 scaleY = getScale(actor).y;
    for (s32 i = 0; i < actor->getHitSensorKeeper()->getSensorNum(); i++)
        actor->getHitSensorKeeper()->getSensor(i)->scaleY(scaleY);
}

void syncSensorAndColliderScaleY(LiveActor* actor) {
    syncSensorScaleY(actor);
    f32 scaleY = getScale(actor).y;
    setColliderRadius(actor, scaleY * getColliderRadius(actor));
    setColliderOffsetY(actor, scaleY * getColliderOffsetY(actor));
}

void setMaterialCode(LiveActor* actor, const char* materialCode) {
    if (actor->getEffectKeeper())
        tryUpdateEffectMaterialCode(actor, materialCode);
    if (isExistSeKeeper(actor))
        tryUpdateSeMaterialCode(actor, materialCode);
}

void initMaterialCode(LiveActor* actor, const ActorInitInfo& initInfo) {
    const char* materialCode = "";
    if (tryGetStringArg(&materialCode, initInfo, "MaterialCode"))
        setMaterialCode(actor, materialCode);
}

bool tryAddDisplayRotate(LiveActor* actor, const ActorInitInfo& initInfo) {
    sead::Vector3f rotate = {0.0f, 0.0f, 0.0f};
    if (!tryGetDisplayRotate(&rotate, initInfo))
        return false;

    sead::Matrix34f rotateMtx;
    rotateMtx.makeR({sead::Mathf::deg2rad(rotate.x), sead::Mathf::deg2rad(rotate.y),
                     sead::Mathf::deg2rad(rotate.z)});

    sead::Matrix34f origMtx = sead::Matrix34f::ident;
    makeMtxRT(&origMtx, actor);

    origMtx = origMtx * rotateMtx;
    updatePoseMtx(actor, &origMtx);
    return true;
}

bool tryAddDisplayOffset(LiveActor* actor, const ActorInitInfo& initInfo) {
    sead::Vector3f offset = {0.0f, 0.0f, 0.0f};
    if (!tryGetDisplayOffset(&offset, initInfo))
        return false;

    *getTransPtr(actor) += offset;
    return true;
}

bool tryAddDisplayScale(LiveActor* actor, const ActorInitInfo& initInfo) {
    sead::Vector3f scale = {0.0f, 0.0f, 0.0f};
    if (!tryGetDisplayScale(&scale, initInfo))
        return false;

    setScaleX(actor, getScale(actor).x * scale.x);
    setScaleY(actor, getScale(actor).y * scale.y);
    setScaleZ(actor, getScale(actor).z * scale.z);
    return true;
}

const PlacementInfo& getPlacementInfo(const ActorInitInfo& initInfo) {
    return *initInfo.placementInfo;
}

const LayoutInitInfo& getLayoutInitInfo(const ActorInitInfo& initInfo) {
    return *initInfo.layoutInitInfo;
}

AudioDirector* getAudioDirector(const ActorInitInfo& initInfo) {
    return initInfo.audioDirector;
}

CollisionDirector* getCollisionDirectorFromInfo(const ActorInitInfo& initInfo) {
    return initInfo.actorSceneInfo.collisionDirector;
}

const SceneCameraInfo* getSceneCameraInfoFromInfo(const ActorInitInfo& initInfo) {
    return initInfo.actorSceneInfo.cameraDirector->getSceneCameraInfo();
}

GraphicsSystemInfo* getGraphicsSystemInfo(const ActorInitInfo& initInfo) {
    return initInfo.actorSceneInfo.graphicsSystemInfo;
}

agl::DrawContext* getDrawContext(const ActorInitInfo& initInfo) {
    return initInfo.actorSceneInfo.graphicsSystemInfo->getDrawContext();
}

void getActorRecourseDataF32(f32* val, LiveActor* actor, const char* fileName,
                             const char* entryName) {
    isExistModelResourceYaml(actor, fileName, nullptr);
    ByamlIter iter = {getModelResourceYaml(actor, fileName, nullptr)};
    tryGetByamlF32(val, iter, entryName);
}

void getActorRecourseDataString(const char** val, LiveActor* actor, const char* fileName,
                                const char* entryName) {
    isExistModelResourceYaml(actor, fileName, nullptr);
    ByamlIter iter = {getModelResourceYaml(actor, fileName, nullptr)};
    *val = getByamlKeyString(iter, entryName);
}

void getActorRecourseDataV3f(sead::Vector3f* val, LiveActor* actor, const char* fileName,
                             const char* entryName) {
    isExistModelResourceYaml(actor, fileName, nullptr);
    ByamlIter iter = {getModelResourceYaml(actor, fileName, nullptr)};
    if (entryName)
        tryGetByamlV3f(val, iter, entryName);
    else
        tryGetByamlV3f(val, iter);
}

void getActorRecourseDataBox3f(sead::BoundBox3f* box, LiveActor* actor, const char* fileName,
                               const char* entryName) {
    isExistModelResourceYaml(actor, fileName, nullptr);
    ByamlIter iter = {getModelResourceYaml(actor, fileName, nullptr)};
    if (entryName)
        tryGetByamlBox3f(box, iter, entryName);
    else
        tryGetByamlBox3f(box, iter);
}

PartsEffectGroup* createPartsEffectGroup(LiveActor* actor, const ActorInitInfo& initInfo,
                                         s32 maxEffects) {
    const char* name;
    if (isExistModelResourceYaml(actor, "InitPartsEffect", nullptr))
        getActorRecourseDataString(&name, actor, "InitPartsEffect", "Name");
    else
        getActorRecourseDataString(&name, actor, "InitEffect", "Name");

    PartsEffectGroup* group = new PartsEffectGroup();
    group->init(maxEffects, initInfo.effectSystemInfo, name, getTransPtr(actor),
                tryGetScalePtr(actor), actor->getBaseMtx(), actor);
    return group;
}

}  // namespace al
