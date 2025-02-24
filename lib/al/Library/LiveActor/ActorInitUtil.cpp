#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Action/ActorActionKeeper.h"
#include "Library/Base/HashCodeUtil.h"
#include "Library/Base/StringUtil.h"
#include "Library/Draw/GraphicsQualityController.h"
#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Factory/Factory.h"
#include "Library/HitSensor/SensorFunction.h"
#include "Library/Light/ModelMaterialCategory.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Model/ModelLodCtrl.h"
#include "Library/Model/ModelOcclusionQuery.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Obj/ActorDitherAnimator.h"
#include "Library/Obj/FarDistanceDitherAnimator.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceHolder.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"
#include "Project/Item/ActorScoreKeeper.h"
#include "math/seadBoundBox.h"
#include "nn/g3d/ModelObj.h"

namespace al {

using PoseKeeperCreatorFunction = void (*)(al::LiveActor*);

const NameToCreator<PoseKeeperCreatorFunction> gActorPoseTable[] = {
    {"TRSV", initActorPoseTRSV}, {"TRMSV", initActorPoseTRMSV}, {"TRGMSV", initActorPoseTRGMSV},
    {"TFSV", initActorPoseTFSV}, {"TFUSV", initActorPoseTFUSV}, {"TFGSV", initActorPoseTFGSV},
    {"TQSV", initActorPoseTQSV}, {"TQGSV", initActorPoseTQGSV}, {"TQGMSV", initActorPoseTQGMSV},
};

void initActorImpl(LiveActor* actor, const ActorInitInfo& initInfo,
                   const sead::SafeString& folderName, const sead::SafeString& fileName,
                   const char*);

void initActor(LiveActor* actor, const ActorInitInfo& initInfo) {
    const char* objName = nullptr;
    tryGetObjectName(&objName, initInfo);
    initActorImpl(actor, initInfo, "ObjectData", objName, nullptr);
}

__attribute__((always_inline)) bool test(const char* pose, LiveActor* actor) {
    if (pose == nullptr)
        return false;
    s32 poseId = -1;
    for (s32 i = 0; i < 9; i++) {
        if (isEqualString(gActorPoseTable[i].mName, pose)) {
            poseId = i;
            break;
        }
    }
    if (poseId != -1) {
        gActorPoseTable[poseId].mCreationFunction(actor);
        return true;
    }
    return false;
}

void initActorImpl(LiveActor* actor, const ActorInitInfo& initInfo,
                   const sead::SafeString& folderName, const sead::SafeString& fileName,
                   const char* unk) {
    al::StringTmp<256> path = {"%s/%s", folderName.cstr(), fileName.cstr()};
    ActorResource* actorResource =
        al::findOrCreateActorResource(initInfo.getActorResourceHolder(), path.cstr(), unk);
    Resource* modelRes = actorResource->getModelRes();
    if (actor->getSceneInfo() == nullptr)
        initActorSceneInfo(actor, initInfo);

    {
        ByamlIter initPose;
        if (tryGetActorInitFileIter(&initPose, modelRes, "InitPose", unk)) {
            const char* pose = nullptr;
            if (initPose.tryGetStringByKey(&pose, "Pose")) {
                if (test(pose, actor)) {
                    if (isEqualString(pose, "TFUSV") &&
                        tryGetByamlKeyBoolOrFalse(initPose, "IsFrontUp"))
                        ((ActorPoseKeeperTFUSV*)actor->getPoseKeeper())->setFrontUp(true);
                }
            }
        }

        initActorSRT(actor, initInfo);
    }
    {
        ByamlIter initScale;
        if (tryGetActorInitFileIter(&initScale, modelRes, "InitScale", unk)) {
            sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
            if (tryGetByamlScale(&scale, initScale, "Scale")) {
                const sead::Vector3f& prevScale = getScale(actor);
                sead::Vector3f newScale;
                newScale.x = scale.x * prevScale.x;
                newScale.y = scale.y * prevScale.y;
                newScale.z = scale.z * prevScale.z;
                setScale(actor, newScale);
            }
        }
    }
    {
        Resource* modelRes = actorResource->getModelRes();
        ByamlIter initModel;
        if (tryGetActorInitFileIter(&initModel, modelRes, "InitModel", unk)) {
            s32 blendAnimMax = 1;
            initModel.tryGetIntByKey(&blendAnimMax, "BlendAnimMax");
            const char* displayRootJointName = nullptr;
            initModel.tryGetStringByKey(&displayRootJointName, "DisplayRootJointName");

            initActorModelKeeper(actor, initInfo, actorResource, blendAnimMax);
            if (displayRootJointName != nullptr)
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
            initModel.tryGetIntByKey(&partialAnimPartsListBufferSize,
                                     "PartialAnimPartsListBufferSize");
            if (partialAnimSlotNum >= 1)
                initPartialSklAnim(actor, partialAnimSlotNum, partialAnimGroupNum,
                                   partialAnimPartsListBufferSize);

            bool isFixedModel = false;
            initModel.tryGetBoolByKey(&isFixedModel, "IsFixedModel");
            if (isFixedModel)
                setFixedModelFlag(actor);

            bool isIgnoreUpdateOnDrawClipping = false;
            initModel.tryGetBoolByKey(&isIgnoreUpdateOnDrawClipping,
                                      "IsIgnoreUpdateOnDrawClipping");
            if (isIgnoreUpdateOnDrawClipping)
                setIgnoreUpdateDrawClipping(actor, true);

            ModelKeeper* modelKeeper = actor->getModelKeeper();

            ByamlIter initLod;
            ModelLodCtrl* modelLodCtrl = nullptr;
            if (tryGetActorInitFileIter(&initLod, modelRes, "InitLod", unk)) {
                sead::BoundBox3f modelBoundingBox;
                s32 lodModelCount = 5;
                if (isExistModel(actor)) {
                    lodModelCount = getLodModelCount(actor);
                    calcModelBoundingBox(&modelBoundingBox, actor);
                }

                modelLodCtrl = new ModelLodCtrl{actor,
                                                getTransPtr(actor),
                                                actor->getBaseMtx(),
                                                getScalePtr(actor),
                                                modelBoundingBox,
                                                lodModelCount};
                modelLodCtrl->init(initLod);
            }

            modelKeeper->setModelLodCtrl(modelLodCtrl);
            if (modelLodCtrl)
                initInfo.getActorSceneInfo().mGraphicsSystemInfo->mModelLodAllCtrl->registerLodCtrl(
                    modelLodCtrl);

            ModelMaterialCategory::tryCreate(
                modelKeeper->getModelCtrl(), modelRes, unk,
                initInfo.getActorSceneInfo().mGraphicsSystemInfo->mMaterialCategoryKeeper);
            modelKeeper->getModelCtrl()->setGraphicsQualityInfo(
                initInfo.getActorSceneInfo()
                    .mGraphicsSystemInfo->mGraphicsQualityController->getGraphicsQualityInfo());
            ModelOcclusionQuery* modelOcclusionQuery =
                ModelOcclusionQuery::tryCreate(actor, modelRes, unk);
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

        initActorPrePassLightKeeper(actor, modelRes, initInfo, unk);
    }
    {
        ByamlIter initExecutor;
        if (tryGetActorInitFileIter(&initExecutor, modelRes, "InitExecutor", unk)) {
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
        } else {
            ModelKeeper* modelKeeper = actor->getModelKeeper();
            if (modelKeeper && modelKeeper->getModelCtrl()->getModelObj()->GetNumShapes())
                initExecutorModelUpdate(actor, initInfo);
        }
    }
    {
        ByamlIter initSensor;
        if (tryGetActorInitFileIter(&initSensor, modelRes, "InitSensor", unk)) {
            s32 numSensors = initSensor.getSize();
            if (numSensors >= 1)
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

                u32 type = alSensorFunction::findSensorTypeByName(sensorType);
                // CollisionParts
                if (type == 15)
                    maxCount = 0;

                addHitSensor(actor, initInfo, sensorName, type, radius, maxCount, offset);

                const char* joint = nullptr;
                if (sensor.tryGetStringByKey(&joint, "Joint"))
                    setHitSensorJointMtx(actor, sensorName, joint);
            }
        }
    }
    {
        ByamlIter initCollision;
        if (tryGetActorInitFileIter(&initCollision, modelRes, "InitCollision", unk)) {
            const char* name = nullptr;
            initCollision.tryGetStringByKey(&name, "Name");
            sead::FixedSafeString<256> unused;

            if (name == nullptr)
                name = getBaseName(modelRes->getArchiveName());

            const char* sensorName = nullptr;
            HitSensor* sensor = nullptr;
            if (initCollision.tryGetStringByKey(&sensorName, "Sensor"))
                sensor = getHitSensor(actor, sensorName);

            const char* joint = nullptr;
            initCollision.tryGetStringByKey(&joint, "Joint");
            sead::Matrix34f* jointMtx = nullptr;
            if (joint != nullptr)
                jointMtx = getJointMtxPtr(actor, joint);

            initActorCollisionWithResource(actor, modelRes, name, sensor, jointMtx, unk);
        }
    }
    {
        ByamlIter initCollider;
        if (tryGetActorInitFileIter(&initCollider, modelRes, "InitCollider", unk)) {
            f32 radius = 0.0f;
            initCollider.tryGetFloatByKey(&radius, "Radius");

            u32 planeNum = 0;
            initCollider.tryGetUIntByKey(&planeNum, "PlaneNum");

            sead::Vector3f offset = sead::Vector3f::zero;
            tryGetByamlV3f(&offset, initCollider);

            actor->initCollider(radius, offset.y, planeNum);
        }
    }
    {
        ByamlIter initEffect;
        if (tryGetActorInitFileIter(&initEffect, modelRes, "InitEffect", unk)) {
            const char* name = nullptr;
            if (initEffect.tryGetStringByKey(&name, "Name"))
                initActorEffectKeeper(actor, initInfo, name);
        }
    }
    {
        ByamlIter initSound;
        const char* seName = nullptr;
        const char* bgmName = nullptr;
        if (tryGetActorInitFileIter(&initSound, modelRes, "InitSound", unk)) {
            initSound.tryGetStringByKey(&seName, "Name");
        } else if (tryGetActorInitFileIter(&initSound, modelRes, "InitAudio", unk)) {
            initSound.tryGetStringByKey(&seName, "SeUserName");
            initSound.tryGetStringByKey(&bgmName, "BgmUserName");
        }
        if (seName != nullptr)
            initActorSeKeeper(actor, initInfo, seName);
        if (initSound.isExistKey("BgmUserName"))
            initActorBgmKeeper(actor, initInfo, bgmName);
    }
    {
        if (isExistRail(initInfo, "Rail"))
            actor->initRailKeeper(initInfo, "Rail");

        initStageSwitch(actor, initInfo);
    }
    {
        ByamlIter initClipping;
        if (tryGetActorInitFileIter(&initClipping, modelRes, "InitClipping", unk)) {
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

                f32 radius = calcModelBoundingSphereRadius(actor) * maxXYZ;
                setClippingInfo(actor, radius, nullptr);
            }

            sead::BoundBox3f obb;
            if (tryGetByamlBox3f(&obb, initClipping, "Obb"))
                setClippingObb(actor, obb);

            f32 nearClipDistance = 0.0f;
            if (initClipping.tryGetFloatByKey(&nearClipDistance, "NearClipDistance"))
                setClippingNearDistance(actor, nearClipDistance);

            if (!initClipping.isExistKey("NoGroupClipping")) {
                ByamlIter groupClipping;
                if (initClipping.tryGetIterByKey(&groupClipping, "GroupClipping"))
                    initGroupClipping(actor, initInfo);
            }
        }

        initActorOcclusionKeeper(actor, modelRes, initInfo, unk);
    }
    {
        bool usingDepthShadow = false;
        tryGetArg(&usingDepthShadow, initInfo, "UsingDepthShadow");
        initDepthShadowMapCtrl(actor, modelRes, initInfo, unk);

        ByamlIter initShadowMask;
        if (tryGetActorInitFileIter(&initShadowMask, modelRes, "InitShadowMask", unk)) {
            initShadowMaskCtrl(actor, initInfo, initShadowMask, unk);
            if (usingDepthShadow)
                invalidateShadowMaskIntensityAll(actor);
        }
    }
    {
        ByamlIter initFlag;
        if (tryGetActorInitFileIter(&initFlag, modelRes, "InitFlag", unk)) {
            ByamlIter tmp;
            if (initFlag.tryGetIterByKey(&tmp, "MaterialCode"))
                validateMaterialCode(actor);
            if (initFlag.tryGetIterByKey(&tmp, "UpdatePuddleMaterial"))
                validatePuddleMaterial(actor);
        }
    }
    {
        ByamlIter initItem;
        if (tryGetActorInitFileIter(&initItem, modelRes, "InitItem", unk))
            initActorItemKeeper(actor, initInfo, initItem);
    }
    {
        ByamlIter initScore;
        if (tryGetActorInitFileIter(&initScore, modelRes, "InitScore", unk)) {
            actor->initScoreKeeper();
            actor->getActorScoreKeeper()->init(initScore);
        }
        
        initScreenPointKeeper(actor, modelRes, initInfo, unk);
        initHitReactionKeeper(actor, modelRes, unk);
        initActorParamHolder(actor, modelRes, unk);
        const char* archiveName = modelRes->getArchiveName();
        initActorActionKeeper(actor, actorResource, archiveName, unk);

        if (actor->getModelKeeper()) {
            if (!tryStartAction(actor, archiveName)) {
                ActorActionKeeper* actionKeeper = actor->getActorActionKeeper();
                if (actionKeeper)
                    actionKeeper->startAction(archiveName);
            }

            ModelKeeper* modelKeeper = actor->getModelKeeper();
            if (modelKeeper) {
                DitherAnimator* ditherAnimator =
                    ActorDitherAnimator::tryCreate(actor, modelRes, unk);
                if (!ditherAnimator)
                    ditherAnimator = FarDistanceDitherAnimator::tryCreate(actor, modelRes, unk);

                if (ditherAnimator)
                    modelKeeper->setDitherAnimator(ditherAnimator);
            }
        }

        if (actor->getNerveKeeper() && actor->getNerveKeeper()->getActionCtrl())
            resetNerveActionForInit(actor);

        if (!actor->getSubActorKeeper())
            initSubActorKeeper(actor, initInfo, unk, 0);
    }
}

/*
void initActorSuffix(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix);
void initActorChangeModel(LiveActor* actor, const ActorInitInfo& initInfo);
void initActorChangeModelSuffix(LiveActor* actor, const ActorInitInfo& initInfo,
                                const char* suffix);
void initActorWithArchiveName(LiveActor* actor, const ActorInitInfo& initInfo,
                              const sead::SafeString& archiveName, const char* suffix);
void initChildActorWithArchiveNameWithPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                                    const sead::SafeString& archiveName,
                                                    const char* suffix);
void initChildActorWithArchiveNameNoPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                                  const sead::SafeString& archiveName,
                                                  const char* suffix);
LiveActor* createChildLinkSimpleActor(const char* actorName, const char* archiveName,
                                      const ActorInitInfo& initInfo, bool alive);
LiveActor* createChildLinkMapPartsActor(const char* actorName, const char* archiveName,
                                        const ActorInitInfo& initInfo, s32 linkIndex, bool alive);
void initMapPartsActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix);
void initLinksActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix,
                    s32 linkIndex);
ActorInitInfo* createLinksPlayerActorInfo(LiveActor* actor, const ActorInitInfo& initInfo);
const char* getLinksActorClassName(const ActorInitInfo&, const char*, s32);
const char* getLinksActorDisplayName(const ActorInitInfo&, const char*, s32);
const char* getLinksActorObjectName(const ActorInitInfo&, const char*, s32);
void initCreateActorWithPlacementInfo(LiveActor*, const ActorInitInfo&);
void initCreateActorWithPlacementInfo(LiveActor*, const ActorInitInfo&, const PlacementInfo&);
void initCreateActorNoPlacementInfo(LiveActor*, const ActorInitInfo&);
void initCreateActorNoPlacementInfoNoViewId(LiveActor*, const ActorInitInfo&);
LiveActor* createPlacementActorFromFactory(const ActorInitInfo&, const PlacementInfo*);
LiveActor* createLinksActorFromFactory(const ActorInitInfo& info, const char* linkName,
                                       s32 linkNum);
LiveActorGroup* createLinksActorGroupFromFactory(const ActorInitInfo&, const char*, const char*);
LiveActorGroup* tryCreateLinksActorGroupFromFactory(const ActorInitInfo&, const char*, const char*);
LiveActorGroup* tryCreateLinksActorFromFactorySingle(const ActorInitInfo&, const char*);
void createAndRegisterLinksActorFromFactory(LiveActorGroup*, const ActorInitInfo&, const char*);
void makeMapPartsModelName(sead::BufferedSafeString*, sead::BufferedSafeString*,
                           const PlacementInfo&);
void makeMapPartsModelName(sead::BufferedSafeString*, sead::BufferedSafeString*,
                           const ActorInitInfo&);
const char* tryGetMapPartsSuffix(const ActorInitInfo&, const char*);
void initMapPartsActorWithArchiveName(LiveActor*, const ActorInitInfo&, const char*, const char*);
void initNerve(LiveActor* actor, const Nerve* nerve, s32 maxStates);
void initNerveAction(LiveActor* actor, const char* actionName,
                     alNerveFunction::NerveActionCollector* collector, s32 maxStates);
bool trySyncStageSwitchAppear(LiveActor* actor);
bool trySyncStageSwitchKill(LiveActor* actor);
bool trySyncStageSwitchAppearAndKill(LiveActor* actor);
bool tryListenStageSwitchAppear(LiveActor* actor);
bool tryListenStageSwitchKill(LiveActor* actor);
void syncSensorScaleY(LiveActor* actor);
void syncSensorAndColliderScaleY(LiveActor* actor);
void setMaterialCode(LiveActor* actor, const char*);
void initMaterialCode(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayRotate(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayOffset(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayScale(LiveActor* actor, const ActorInitInfo& initInfo);
const PlacementInfo* getPlacementInfo(const ActorInitInfo& initInfo);
const LayoutInitInfo& getLayoutInitInfo(const ActorInitInfo& initInfo);
AudioDirector* getAudioDirector(const ActorInitInfo& initInfo);
CollisionDirector* getCollisionDirectorFromInfo(const ActorInitInfo& initInfo);
const SceneCameraInfo* getSceneCameraInfoFromInfo(const ActorInitInfo& initInfo);
GraphicsSystemInfo* getGraphicsSystemInfo(const ActorInitInfo& initInfo);
sead::DrawContext* getDrawContext(const ActorInitInfo& initInfo);
void getActorRecourseDataF32(f32*, LiveActor*, const char*, const char*);
void getActorRecourseDataString(const char**, LiveActor*, const char*, const char*);
void getActorRecourseDataV3f(sead::Vector3f*, LiveActor* actor, const char*, const char*);
void getActorRecourseDataBox3f(sead::BoundBox3f* box, LiveActor* actor, const char*, const char*);
PartsEffectGroup* createPartsEffectGroup(LiveActor* actor, const ActorInitInfo& initInfo, s32);
*/

}  // namespace al
