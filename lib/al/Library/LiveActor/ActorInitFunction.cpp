#include "Library/LiveActor/ActorInitFunction.h"

#include <nn/g3d/ModelObj.h>

#include "Library/Action/ActorActionKeeper.h"
#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Camera/SceneCameraInfo.h"
#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/ICollisionPartsKeeper.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/Collision/PartsMtxConnector.h"
#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Execute/ExecuteDirector.h"
#include "Library/Execute/ExecuteUtil.h"
#include "Library/Item/ItemUtil.h"
#include "Library/Light/ModelMaterialCategory.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorParamHolder.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/HitReactionKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/LiveActor/LiveActorInfo.h"
#include "Library/LiveActor/LiveActorKeeper.h"
#include "Library/Model/ModelCtrl.h"
#include "Library/Model/ModelGroup.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Model/ModelShapeUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Play/Graphics/PrepassTriangleCulling.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Screen/ScreenPointKeeper.h"
#include "Library/Shader/ActorOcclusionKeeper.h"
#include "Library/Shadow/DepthShadowMapCtrl.h"
#include "Library/Shadow/DepthShadowMapDirector.h"
#include "Library/Shadow/ShadowDirector.h"
#include "Library/Shadow/ShadowKeeper.h"
#include "Library/Shadow/ShadowMaskCastOvalCylinder.h"
#include "Library/Shadow/ShadowMaskCtrl.h"
#include "Library/Shadow/ShadowMaskCube.h"
#include "Library/Shadow/ShadowMaskCylinder.h"
#include "Library/Shadow/ShadowMaskDirector.h"
#include "Library/Shadow/ShadowMaskSphere.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"
#include "Project/Light/ActorPrepassLightKeeper.h"

namespace al {

void initActorSceneInfo(LiveActor* actor, const ActorInitInfo& info) {
    auto* sceneInfo = new ActorSceneInfo();
    *sceneInfo = info.actorSceneInfo;
    actor->initSceneInfo(sceneInfo);
    info.allActorsGroup->registerActor(actor);
}

void initExecutorUpdate(LiveActor* actor, const ActorInitInfo& info, const char* listName) {
    registerExecutorActorUpdate(actor, info.executeDirector, listName);
}

void initExecutorDraw(LiveActor* actor, const ActorInitInfo& info, const char* listName) {
    registerExecutorActorDraw(actor, info.executeDirector, listName);
}

void initExecutorPlayer(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤー");
    initExecutorDraw(actor, info, "プレイヤー");
};

void initExecutorPlayerPreMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤー[PreMovement]");
}

void initExecutorPlayerMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤー[Movement]");
}

void initExecutorPlayerModel(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤーモデル");
    initExecutorDraw(actor, info, "プレイヤーモデル");
}

void initExecutorPlayerDecoration(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤー装飾");
    initExecutorDraw(actor, info, "プレイヤー装飾");
}

void initExecutorEnemy(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "敵");
    initExecutorDraw(actor, info, "敵");
}

void initExecutorEnemyMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "敵[Movement]");
}

void initExecutorEnemyDecoration(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "敵装飾");
    initExecutorDraw(actor, info, "敵装飾");
}

void initExecutorEnemyDecorationMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "敵装飾[Movement]");
}

void initExecutorMapObj(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "地形オブジェ");
    initExecutorDraw(actor, info, "地形オブジェ");
}

void initExecutorMapObjMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "地形オブジェ[Movement]");
}

void initExecutorMapObjDecoration(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "地形オブジェ装飾");
    initExecutorDraw(actor, info, "地形オブジェ装飾");
}

void initExecutorNpcDecoration(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "ＮＰＣ装飾");
    initExecutorDraw(actor, info, "ＮＰＣ");
}

void initExecutorShadowVolume(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "影ボリューム");
    initExecutorDraw(actor, info, "影ボリューム");
}

void initExecutorShadowVolumeFillStencil(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "影ボリュームのフィル");
    initExecutorDraw(actor, info, "影ボリュームのフィル");
}

void initExecutorCollisionMapObjDecorationMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "コリジョン地形装飾[Movement]");
}

void initExecutorWatchObj(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "監視オブジェ");
}

void initExecutorDebugMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "デバッグ[ActorMovement]");
}

inline s32 getCalcViewCore(LiveActor* actor) {
    volatile s32 viewCore = actor->getModelKeeper()->getModelCtrl()->getCalcViewCore();
    return viewCore;
}

void initExecutorModelUpdate(LiveActor* actor, const ActorInitInfo& info) {
    s32 calcViewCore = getCalcViewCore(actor);

    const char* table;
    if (calcViewCore == 1)
        table = "ビュー更新(コア1)";
    else if (calcViewCore == 2)
        table = "ビュー更新(コア2)";
    else if (actor->getModelKeeper()->getModelCtrl()->getModelObj()->get_8c() > 1)
        table = "ビュー更新(コア1)";
    else
        table = "ビュー更新(コア2)";

    initExecutorDraw(actor, info, "モデル描画バッファ更新");
    if (isUsingPrepassTriangleCulling() && isIncludePrepassCullingShape(actor))
        initExecutorDraw(actor, info, "カリング");
    initExecutorUpdate(actor, info, table);
}

void initExecutorDrcAssistMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "DRCアシスト[Movement]");
}

void initActorPoseTRSV(LiveActor* actor) {
    actor->initPoseKeeper(new ActorPoseKeeperTRSV());
}

void initActorPoseTRMSV(LiveActor* actor) {
    actor->initPoseKeeper(new ActorPoseKeeperTRMSV());
}

void initActorPoseTRGMSV(LiveActor* actor) {
    actor->initPoseKeeper(new ActorPoseKeeperTRGMSV());
}

void initActorPoseTFSV(LiveActor* actor) {
    actor->initPoseKeeper(new ActorPoseKeeperTFSV());
}

void initActorPoseTFUSV(LiveActor* actor) {
    actor->initPoseKeeper(new ActorPoseKeeperTFUSV());
}

void initActorPoseTFGSV(LiveActor* actor) {
    actor->initPoseKeeper(new ActorPoseKeeperTFGSV());
}

void initActorPoseTQSV(LiveActor* actor) {
    actor->initPoseKeeper(new ActorPoseKeeperTQSV());
}

void initActorPoseTQGSV(LiveActor* actor) {
    actor->initPoseKeeper(new ActorPoseKeeperTQGSV());
}

void initActorPoseTQGMSV(LiveActor* actor) {
    actor->initPoseKeeper(new ActorPoseKeeperTQGMSV());
}

void initActorSRT(LiveActor* actor, const ActorInitInfo& info) {
    sead::Vector3f trans = {0.0f, 0.0f, 0.0f};
    tryGetTrans(&trans, info);
    setTrans(actor, trans);

    sead::Vector3f rotate = {0.0f, 0.0f, 0.0f};
    tryGetRotate(&rotate, info);
    updatePoseRotate(actor, rotate);

    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    tryGetScale(&scale, info);
    setScale(actor, scale);
}

void initActorModelKeeper(LiveActor* actor, const ActorInitInfo& info,
                          const ActorResource* resource, s32 blendAnimMax) {
    auto* modelKeeper = new ModelKeeper();
    modelKeeper->initResource(resource);

    SceneCameraInfo* sceneCameraInfo = info.actorSceneInfo.cameraDirector->getSceneCameraInfo();

    if (sceneCameraInfo->getViewNumMax() >= 2) {
        modelKeeper->getModelCtrl()->setCameraInfo(
            &sceneCameraInfo->getViewAt(0)->getLookAtCam().getMatrix(),
            &sceneCameraInfo->getViewAt(1)->getLookAtCam().getMatrix(),
            &sceneCameraInfo->getViewAt(0)->getProjMtx(),
            &sceneCameraInfo->getViewAt(1)->getProjMtx());
    } else {
        modelKeeper->getModelCtrl()->setCameraInfo(
            &sceneCameraInfo->getViewAt(0)->getLookAtCam().getMatrix(),
            &sceneCameraInfo->getViewAt(0)->getLookAtCam().getMatrix(),
            &sceneCameraInfo->getViewAt(0)->getProjMtx(),
            &sceneCameraInfo->getViewAt(0)->getProjMtx());
    }

    modelKeeper->initModel(
        blendAnimMax, info.actorSceneInfo.graphicsSystemInfo->getGpuMemAllocator(),
        info.actorSceneInfo.graphicsSystemInfo->getModelShaderHolder(),
        info.actorSceneInfo.graphicsSystemInfo->getModelOcclusionCullingDirector(),
        info.actorSceneInfo.graphicsSystemInfo->getShadowDirector(),
        info.actorSceneInfo.graphicsSystemInfo->getPrepassTriangleCulling());

    {
        const char* cubeMapName = nullptr;
        tryGetStringArg(&cubeMapName, info, "CubeMapUnitName");
        if (cubeMapName)
            forceApplyCubeMap(modelKeeper, info.actorSceneInfo.graphicsSystemInfo, cubeMapName);
    }

    actor->initModelKeeper(modelKeeper);
    info.actorSceneInfo.modelGroup->registerModel(modelKeeper);

    sead::Matrix34f baseMtx;
    actor->getPoseKeeper()->calcBaseMtx(&baseMtx);
    setBaseMtxAndCalcAnim(actor, baseMtx, actor->getPoseKeeper()->getScale());
}

void initActorModelKeeper(LiveActor* actor, const ActorInitInfo& info, const char* actorResource,
                          s32 blendAnimMax, const char* animResource) {
    initActorModelKeeper(actor, info,
                         findOrCreateActorResourceWithAnimResource(
                             info.actorResourceHolder, actorResource, animResource, nullptr, false),
                         blendAnimMax);
}

void initActorModelKeeperByHost(LiveActor* actor, const LiveActor* host) {
    actor->initModelKeeper(host->getModelKeeper());
}

void initActorModelForceCubeMap(LiveActor* actor, const ActorInitInfo& info) {
    const char* cubeMapName = nullptr;
    tryGetStringArg(&cubeMapName, info, "CubeMapUnitName");
    if (cubeMapName && actor->getModelKeeper())
        forceApplyCubeMap(actor, cubeMapName);
}

void initActorActionKeeper(LiveActor* actor, const ActorInitInfo& info,
                           const char* modelArchiveName, const char* suffix) {
    ActorResource* resource = nullptr;
    if (actor->getModelKeeper()) {
        resource = findOrCreateActorResource(info.actorResourceHolder,
                                             getModelResource(actor)->getPath(), suffix);
    }

    initActorActionKeeper(actor, resource, modelArchiveName, suffix);
}

void initActorActionKeeper(LiveActor* actor, const ActorResource* resource,
                           const char* modelArchiveName, const char* suffix) {
    auto* actionKeeper = ActorActionKeeper::tryCreate(actor, resource, modelArchiveName, suffix);
    actor->initActionKeeper(actionKeeper);
    if (actionKeeper)
        actionKeeper->init();
}

void initActorEffectKeeper(LiveActor* actor, const ActorInitInfo& info, const char* name) {
    auto* effectKeeper = new EffectKeeper(info.effectSystemInfo, name, getTransPtr(actor),
                                          tryGetScalePtr(actor), actor->getBaseMtx());
    actor->initEffectKeeper(effectKeeper);
}

void initActorSeKeeper(LiveActor* actor, const ActorInitInfo& info, const char* seName,
                       const sead::Vector3f* transPtr, const sead::Matrix34f* baseMtx) {
    AudioDirector* audioDirector = info.audioDirector;
    AudioKeeper* audioKeeper;
    if (actor->getAudioKeeper())
        audioKeeper = actor->getAudioKeeper();
    else {
        audioKeeper = alAudioKeeperFunction::createAudioKeeper(audioDirector);
        actor->initAudioKeeper(audioKeeper);
    }
    audioKeeper->initSeKeeper(audioDirector, seName, transPtr, baseMtx, actor->getModelKeeper(),
                              info.actorSceneInfo.cameraDirector);
}

void initActorSeKeeper(LiveActor* actor, const ActorInitInfo& info, const char* seName) {
    initActorSeKeeper(actor, info, seName, getTransPtr(actor), actor->getBaseMtx());
}

void initActorSeKeeperWithout3D(LiveActor* actor, const ActorInitInfo& info, const char* seName) {
    initActorSeKeeper(actor, info, seName, nullptr, nullptr);
}

void initActorBgmKeeper(LiveActor* actor, const ActorInitInfo& info, const char* bgmName) {
    AudioDirector* audioDirector = info.audioDirector;
    AudioKeeper* audioKeeper;
    if (actor->getAudioKeeper())
        audioKeeper = actor->getAudioKeeper();
    else {
        audioKeeper = alAudioKeeperFunction::createAudioKeeper(audioDirector);
        actor->initAudioKeeper(audioKeeper);
    }
    audioKeeper->initBgmKeeper(info.audioDirector, bgmName);
}

bool isInitializedBgmKeeper(LiveActor* actor) {
    AudioKeeper* audioKeeper = actor->getAudioKeeper();
    return audioKeeper && audioKeeper->getBgmKeeper();
}

void initHitReactionKeeper(LiveActor* actor, const char* suffix) {
    initHitReactionKeeper(actor, getModelResource(actor), suffix);
}

void initHitReactionKeeper(LiveActor* actor, const Resource* resource, const char* suffix) {
    HitReactionKeeper* hitReactionKeeper = HitReactionKeeper::tryCreate(
        actor->getName(), getTransPtr(actor), actor, actor, actor,
        actor->getSceneInfo()->padRumbleDirector, actor->getSceneInfo()->sceneStopCtrl,
        actor->getSceneInfo()->graphicsSystemInfo->getRadialBlurDirector(),
        actor->getSceneInfo()->playerHolder, resource, suffix);
    if (hitReactionKeeper)
        actor->setHitReactionKeeper(hitReactionKeeper);
}

void initActorParamHolder(LiveActor* actor, const char* suffix) {
    initActorParamHolder(actor, getModelResource(actor), suffix);
}

void initActorParamHolder(LiveActor* actor, const Resource* resource, const char* suffix) {
    ActorParamHolder* paramHolder = ActorParamHolder::tryCreate(actor, resource, suffix);
    if (paramHolder)
        actor->setActorParamHolder(paramHolder);
}

void initDepthShadowMapCtrl(LiveActor* actor, const Resource* resource, const ActorInitInfo& info,
                            const char* suffix) {
    ByamlIter fileIter;
    sead::FixedSafeString<256> fileName;
    if (!tryGetActorInitFileIterAndName(&fileIter, &fileName, resource, "InitDepthShadowMap",
                                        suffix))
        return;

    DepthShadowMapCtrl* depthShadowMapCtrl = new DepthShadowMapCtrl(resource);
    depthShadowMapCtrl->init(actor, fileIter);
    actor->getShadowKeeper()->setDepthShadowMapCtrl(depthShadowMapCtrl);
}

void initDepthShadowMapCtrlWithoutIter(LiveActor* actor, s32 size, bool isAppendSubActor) {
    DepthShadowMapCtrl* depthShadowMapCtrl = new DepthShadowMapCtrl(nullptr);
    depthShadowMapCtrl->initWithoutIter(actor, size);
    actor->getShadowKeeper()->setDepthShadowMapCtrl(depthShadowMapCtrl);
    actor->getShadowKeeper()->getDepthShadowMapCtrl()->setAppendSubActor(isAppendSubActor);
}

// TODO: assign proper parameter names
void addDepthShadowMapInfo(const LiveActor* actor, const char* a1, s32 a2, s32 a3, s32 a4, f32 a5,
                           bool a6, const sead::Vector3f& a7, bool a8, const sead::Vector3f& a9,
                           const sead::Vector3f& a10, bool a11, const char* a12, s32 a13, bool a14,
                           f32 a15, f32 a16, f32 a17, bool a18, bool a19, f32 a20, s32 a21,
                           bool a22) {
    actor->getShadowKeeper()->getDepthShadowMapCtrl()->appendDepthShadowMapInfo(
        a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,
        a21, a22, false, 1.0f);
}

void declareUseDepthShadowMap(const LiveActor* actor, s32 num) {
    actor->getSceneInfo()
        ->graphicsSystemInfo->getShadowDirector()
        ->getDepthShadowMapDirector()
        ->declareUseDepthShadowMap(num);
}

// TODO: assign proper parameter names
void createDepthShadowMap(const LiveActor* actor, const char* a1, s32 a2, s32 a3, s32 a4) {
    actor->getSceneInfo()
        ->graphicsSystemInfo->getShadowDirector()
        ->getDepthShadowMapDirector()
        ->createDepthShadowMap(actor->getShadowKeeper()->getDepthShadowMapCtrl(),
                               actor->getModelKeeper(), a1, a2, a3, a4);
}

void initShadowMaskCtrl(LiveActor* actor, const ActorInitInfo& info, const ByamlIter& iter,
                        const char* unused) {
    bool isIgnoreShadowMaskYaml = false;
    tryGetArg(&isIgnoreShadowMaskYaml, info, "IsIgnoreShadowMaskYaml");

    ShadowMaskCtrl* shadowMaskCtrl = new ShadowMaskCtrl(isIgnoreShadowMaskYaml);
    shadowMaskCtrl->init(actor, info, iter);

    ShadowMaskDirector* shadowMaskDirector =
        info.actorSceneInfo.graphicsSystemInfo->getShadowDirector()->getShadowMaskDirector();
    s32 numShadowMasks = shadowMaskCtrl->getShadowMaskNum();
    for (s32 i = 0; i < numShadowMasks; i++)
        shadowMaskDirector->registerShadowMask(shadowMaskCtrl->getShadowMask(i));

    actor->getShadowKeeper()->setShadowMaskCtrl(shadowMaskCtrl);
}

void initShadowMaskCtrlWithoutInitFile(LiveActor* actor, const ActorInitInfo& info, s32 numMasks) {
    ShadowMaskCtrl* shadowMaskCtrl = new ShadowMaskCtrl(false);
    shadowMaskCtrl->init(actor, numMasks);

    ShadowMaskDirector* shadowMaskDirector =
        info.actorSceneInfo.graphicsSystemInfo->getShadowDirector()->getShadowMaskDirector();
    s32 numShadowMasks = shadowMaskCtrl->getShadowMaskNum();
    for (s32 i = 0; i < numShadowMasks; i++)
        shadowMaskDirector->registerShadowMask(shadowMaskCtrl->getShadowMask(i));

    actor->getShadowKeeper()->setShadowMaskCtrl(shadowMaskCtrl);
}

void createShadowMaskSphere(LiveActor* actor, const char* name, const char* jointName,
                            const char* drawCategory) {
    ShadowMaskSphere* shadowMaskSphere = new ShadowMaskSphere(name);
    shadowMaskSphere->createMtxConnector();
    shadowMaskSphere->setDrawCategory(drawCategory);
    ShadowMaskCtrl* shadowMaskCtrl = actor->getShadowKeeper()->getShadowMaskCtrl();
    shadowMaskSphere->setHost(actor);
    shadowMaskSphere->declare(shadowMaskSphere->getDrawCategory());

    MtxConnector* mtxConnector = shadowMaskSphere->getMtxConnector();
    if (jointName && !isEqualString(jointName, "") && isExistJoint(actor, jointName))
        attachMtxConnectorToJoint(mtxConnector, actor, jointName);
    else
        attachMtxConnectorToActor(mtxConnector, actor);

    shadowMaskCtrl->appendShadowMask(shadowMaskSphere);
}

// TODO: assign proper parameter names
void createShadowMaskCube(LiveActor* actor, const char* name, const char* jointName,
                          const char* drawCategory, const sead::Color4f& color,
                          const sead::Vector3f& offset, f32 a2, f32 a3, f32 dropLength,
                          const sead::Vector3f& a5, f32 a6) {
    ShadowMaskCube* shadowMaskCube = new ShadowMaskCube(name);
    shadowMaskCube->createMtxConnector();
    shadowMaskCube->init(a2, a3, dropLength, a5, a6, offset, color);
    shadowMaskCube->setDrawCategory(drawCategory);
    ShadowMaskCtrl* shadowMaskCtrl = actor->getShadowKeeper()->getShadowMaskCtrl();
    shadowMaskCube->setHost(actor);
    shadowMaskCube->declare(shadowMaskCube->getDrawCategory());

    MtxConnector* mtxConnector = shadowMaskCube->getMtxConnector();
    if (jointName && !isEqualString(jointName, "") && isExistJoint(actor, jointName))
        attachMtxConnectorToJoint(mtxConnector, actor, jointName);
    else
        attachMtxConnectorToActor(mtxConnector, actor);

    shadowMaskCtrl->appendShadowMask(shadowMaskCube);
}

// TODO: assign proper parameter names
void createShadowMaskCylinder(LiveActor* actor, const char* name, const char* jointName,
                              const char* drawCategory, const sead::Color4f& color,
                              const sead::Vector3f& offset, f32 a2, f32 dropLength, f32 a3, f32 a5,
                              f32 a6) {
    ShadowMaskCylinder* shadowMaskCylinder = new ShadowMaskCylinder(name);
    shadowMaskCylinder->createMtxConnector();
    shadowMaskCylinder->init(a2, dropLength, a3, a5, a6, offset, color);
    shadowMaskCylinder->setDrawCategory(drawCategory);
    ShadowMaskCtrl* shadowMaskCtrl = actor->getShadowKeeper()->getShadowMaskCtrl();
    shadowMaskCylinder->setHost(actor);
    shadowMaskCylinder->declare(shadowMaskCylinder->getDrawCategory());

    MtxConnector* mtxConnector = shadowMaskCylinder->getMtxConnector();
    if (jointName && !isEqualString(jointName, "") && isExistJoint(actor, jointName))
        attachMtxConnectorToJoint(mtxConnector, actor, jointName);
    else
        attachMtxConnectorToActor(mtxConnector, actor);

    shadowMaskCtrl->appendShadowMask(shadowMaskCylinder);
}

void createShadowMaskCastOvalCylinder(LiveActor* actor, const char* name, const char* jointName,
                                      const char* drawCategory, const sead::Color4f& color,
                                      const sead::Vector3f& offset, const sead::Vector3f& scale,
                                      f32 dropLength, f32 expXZ, f32 expY, f32 distYBase) {
    ShadowMaskCastOvalCylinder* shadowMaskCastOvalCylinder = new ShadowMaskCastOvalCylinder(name);
    shadowMaskCastOvalCylinder->createMtxConnector();
    shadowMaskCastOvalCylinder->init(scale, dropLength, expXZ, expY, distYBase, offset, color);
    shadowMaskCastOvalCylinder->setDrawCategory(drawCategory);
    ShadowMaskCtrl* shadowMaskCtrl = actor->getShadowKeeper()->getShadowMaskCtrl();
    shadowMaskCastOvalCylinder->setHost(actor);
    shadowMaskCastOvalCylinder->declare(shadowMaskCastOvalCylinder->getDrawCategory());

    MtxConnector* mtxConnector = shadowMaskCastOvalCylinder->getMtxConnector();
    if (jointName && !isEqualString(jointName, "") && isExistJoint(actor, jointName))
        attachMtxConnectorToJoint(mtxConnector, actor, jointName);
    else
        attachMtxConnectorToActor(mtxConnector, actor);

    shadowMaskCtrl->appendShadowMask(shadowMaskCastOvalCylinder);
}

void initActorCollision(LiveActor* actor, const sead::SafeString& filePath,
                        HitSensor* connectedSensor, const sead::Matrix34f* jointMtx) {
    initActorCollisionWithResource(actor, getModelResource(actor), filePath, connectedSensor,
                                   jointMtx, nullptr);
}

void initActorCollisionWithResource(LiveActor* actor, const Resource* resource,
                                    const sead::SafeString& filePath, HitSensor* connectedSensor,
                                    const sead::Matrix34f* jointMtx, const char* suffix) {
    sead::FixedSafeString<256> attributePath;
    attributePath = filePath;
    attributePath.append("Attribute");

    const void* kcl = resource->tryGetKcl(filePath);
    if (!kcl)
        return;

    const u8* byml = resource->tryGetByml(attributePath);

    const char* specialPurpose = nullptr;
    const char* optionalPurpose = nullptr;
    s32 priority = -1;

    ByamlIter collision;
    if (tryGetActorInitFileIter(&collision, resource, "InitCollision", suffix)) {
        collision.tryGetStringByKey(&specialPurpose, "SpecialPurpose");
        collision.tryGetStringByKey(&optionalPurpose, "OptionalPurpose");
        collision.tryGetIntByKey(&priority, "Priority");
    }

    initActorCollisionWithFilePtr(actor, const_cast<void*>(kcl), byml, connectedSensor, jointMtx,
                                  specialPurpose, optionalPurpose, priority);
}

void initActorCollisionWithArchiveName(LiveActor* actor, const sead::SafeString& resourceName,
                                       const sead::SafeString& filePath, HitSensor* connectedSensor,
                                       const sead::Matrix34f* jointMtx) {
    initActorCollisionWithResource(actor, findOrCreateResource(resourceName, nullptr), filePath,
                                   connectedSensor, jointMtx, nullptr);
}

void initActorCollisionWithFilePtr(LiveActor* actor, void* kcl, const void* byml,
                                   HitSensor* connectedSensor, const sead::Matrix34f* jointMtx,
                                   const char* specialPurpose, const char* optionalPurpose,
                                   s32 priority) {
    CollisionParts* parts = new CollisionParts(const_cast<void*>(kcl), byml);
    parts->set_16e(true);
    parts->setSpecialPurpose(specialPurpose);
    parts->setOptionalPurpose(optionalPurpose);
    parts->setPriority(priority);
    sead::Matrix34f initMtx;
    makeMtxSRT(&initMtx, actor);
    parts->setConnectedSensor(connectedSensor);
    parts->initParts(initMtx);
    parts->setJointMtx(jointMtx);

    actor->getCollisionDirector()->getActivePartsKeeper()->addCollisionParts(parts);
    parts->invalidateBySystem();
    actor->setCollisionParts(parts);
}

void initStageSwitch(LiveActor* actor, const ActorInitInfo& info) {
    initStageSwitch(actor, info.stageSwitchDirector, *info.placementInfo);
}

void initActorItemKeeper(LiveActor* actor, const ActorInitInfo& info, const ByamlIter& iter) {
    s32 addItemNum = 0;

    ByamlIter initInfo;
    if (iter.tryGetIterByKey(&initInfo, "InitInfo")) {
        s32 addItemNum_ = 0;
        if (initInfo.tryGetIntByKey(&addItemNum_, "AddItemNum"))
            addItemNum = addItemNum_;
    }

    const ByamlIter* itemList;
    ByamlIter itemList_;
    if (iter.tryGetIterByKey(&itemList_, "ItemList"))
        itemList = &itemList_;
    else
        itemList = &iter;
    ByamlIter items = *itemList;

    s32 itemsNum = items.getSize();
    s32 totalItems = itemsNum + addItemNum;

    if (totalItems <= 0)
        return;

    actor->initItemKeeper(totalItems);
    for (s32 i = 0; i < totalItems; i++) {
        ByamlIter itemIter;
        if (!items.tryGetIterByIndex(&itemIter, i))
            continue;

        const char* item = nullptr;
        if (!itemIter.tryGetStringByKey(&item, "Item"))
            continue;

        const char* timing = nullptr;
        itemIter.tryGetStringByKey(&timing, "Timing");
        const char* factor = nullptr;
        itemIter.tryGetStringByKey(&factor, "Factor");
        bool isNoDeclarePlacementActor =
            tryGetByamlKeyBoolOrFalse(itemIter, "IsNoDeclarePlacementActor");
        s32 numMax = -1;
        itemIter.tryGetIntByKey(&numMax, "NumMax");
        addItem(actor, info, item, timing, factor, numMax, isNoDeclarePlacementActor);
    }
}

bool initActorPrePassLightKeeper(LiveActor* actor, const Resource* resource,
                                 const ActorInitInfo& info, const char* suffix) {
    if (!ActorPrePassLightKeeper::isExistFile(resource, suffix))
        return false;

    ActorPrePassLightKeeper* keeper = new ActorPrePassLightKeeper(actor);
    keeper->init(resource, info, suffix);
    actor->initActorPrePassLightKeeper(keeper);
    return true;
}

void initActorOcclusionKeeper(LiveActor* actor, const Resource* resource, const ActorInitInfo& info,
                              const char* fileSuffix) {
    GraphicsSystemInfo* graphicsSystemInfo = info.actorSceneInfo.graphicsSystemInfo;
    if (!ActorOcclusionKeeper::isExistFile(resource, fileSuffix))
        return;

    ActorOcclusionKeeper* keeper =
        new ActorOcclusionKeeper(graphicsSystemInfo, resource, actor, fileSuffix);
    actor->initActorOcclusionKeeper(keeper);
}

void initSubActorKeeper(LiveActor* actor, const ActorInitInfo& info, const char* suffix,
                        s32 maxSubActors) {
    SubActorKeeper* keeper = SubActorKeeper::tryCreate(actor, suffix, maxSubActors);
    if (!keeper)
        return;

    actor->initSubActorKeeper(keeper);
    keeper->init(info, suffix, maxSubActors);
}

void initSubActorKeeperNoFile(LiveActor* actor, const ActorInitInfo& info, s32 maxSubActors) {
    SubActorKeeper* keeper = SubActorKeeper::create(actor);
    actor->initSubActorKeeper(keeper);
    keeper->init(info, nullptr, maxSubActors);
}

void registerSubActor(LiveActor* actor, LiveActor* subActor) {
    actor->getSubActorKeeper()->registerSubActor(subActor, 0);
}

void registerSubActorSyncClipping(LiveActor* actor, LiveActor* subActor) {
    actor->getSubActorKeeper()->registerSubActor(subActor, 2);
}

void registerSubActorSyncClippingAndHide(LiveActor* actor, LiveActor* subActor) {
    actor->getSubActorKeeper()->registerSubActor(subActor, 6);
}

void registerSubActorSyncAll(LiveActor* actor, LiveActor* subActor) {
    actor->getSubActorKeeper()->registerSubActor(subActor, 15);
}

void setSubActorOffSyncClipping(LiveActor* actor) {
    SubActorKeeper* keeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < keeper->getCurActorCount(); i++)
        keeper->getActorInfo(i)->syncType &= ~2;
}

void initScreenPointKeeper(LiveActor* actor, const Resource* resource, const ActorInitInfo& info,
                           const char* fileName) {
    if (!ScreenPointKeeper::isExistFile(resource, fileName))
        return;

    ScreenPointKeeper* keeper = new ScreenPointKeeper();
    keeper->initByYaml(actor, resource, info, fileName);
    actor->initScreenPointKeeper(keeper);
}

void initScreenPointKeeperNoYaml(LiveActor* actor, s32 size) {
    if (actor->getScreenPointKeeper())
        return;

    ScreenPointKeeper* keeper = new ScreenPointKeeper();
    keeper->initArray(size);
    actor->initScreenPointKeeper(keeper);
}

// TODO: rename parameter
void initActorMaterialCategory(LiveActor* actor, const ActorInitInfo& info, const char* a1) {
    GraphicsSystemInfo* graphicsSystemInfo = info.actorSceneInfo.graphicsSystemInfo;
    ModelMaterialCategory::tryCreate(actor->getModelKeeper()->getModelCtrl(), a1,
                                     graphicsSystemInfo->getMaterialCategoryKeeper());
}

}  // namespace al
