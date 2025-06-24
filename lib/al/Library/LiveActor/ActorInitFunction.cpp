#include "Library/LiveActor/ActorInitFunction.h"

#include "Library/Action/ActorActionKeeper.h"
#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Camera/SceneCameraInfo.h"
#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Execute/ExecuteDirector.h"
#include "Library/Execute/ExecuteUtil.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Model/ModelCtrl.h"
#include "Library/Model/ModelGroup.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Model/ModelShapeUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Play/Graphics/PrepassTriangleCulling.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceHolder.h"
#include "nn/g3d/ModelObj.h"

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

// TODO: mismatch
void initExecutorModelUpdate(LiveActor* actor, const ActorInitInfo& info) {
    s32 calcViewCore = actor->getModelKeeper()->getModelCtrl()->getCalcViewCore();
    // mismatch here ^

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
                                             getModelResource(actor)->getName(), suffix);
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

/*
void initHitReactionKeeper(LiveActor*, const char*);
void initHitReactionKeeper(LiveActor*, const Resource*, const char*);
void initActorParamHolder(LiveActor*, const char*);
void initActorParamHolder(LiveActor*, const Resource*, const char*);
void initDepthShadowMapCtrl(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
void initDepthShadowMapCtrlWithoutIter(LiveActor*, s32, bool);
void addDepthShadowMapInfo(const LiveActor*, const char*, s32, s32, s32, f32, bool,
                           const sead::Vector3f&, bool, const sead::Vector3f&,
                           const sead::Vector3f&, bool, const char*, s32, bool, f32, f32, f32, bool,
                           bool, f32, s32, bool);
void declareUseDepthShadowMap(const LiveActor*, s32);
void createDepthShadowMap(const LiveActor*, const char*, s32, s32, s32);
void initShadowMaskCtrl(LiveActor*, const ActorInitInfo&, const ByamlIter&, const char*);
void initShadowMaskCtrlWithoutInitFile(LiveActor*, const ActorInitInfo&, s32);
void createShadowMaskSphere(LiveActor*, const char*, const char*, const char*);
void createShadowMaskCube(LiveActor*, const char*, const char*, const char*, const sead::Color4f&,
                          const sead::Vector3f&, f32, f32, f32, const sead::Vector3f&, f32);
void createShadowMaskCylinder(LiveActor*, const char*, const char*, const char*,
                              const sead::Color4f&, const sead::Vector3f&, f32, f32, f32, f32, f32);
void createShadowMaskCastOvalCylinder(LiveActor*, const char*, const char*, const char*,
                                      const sead::Color4f&, const sead::Vector3f&,
                                      const sead::Vector3f&, f32, f32, f32, f32);
void initActorCollision(LiveActor*, const sead::SafeString&, HitSensor*, const sead::Matrix34f*);
void initActorCollisionWithResource(LiveActor*, const Resource*, const sead::SafeString&,
                                    HitSensor*, const sead::Matrix34f*, const char*);
void initActorCollisionWithArchiveName(LiveActor*, const sead::SafeString&, const sead::SafeString&,
                                       HitSensor*, const sead::Matrix34f*);
void initActorCollisionWithFilePtr(LiveActor*, void*, const void*, HitSensor*,
                                   const sead::Matrix34f*, const char*, const char*, s32);
void initStageSwitch(LiveActor*, const ActorInitInfo&);
void initActorItemKeeper(LiveActor*, const ActorInitInfo&, const ByamlIter&);
bool initActorPrePassLightKeeper(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
void initActorOcclusionKeeper(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
void initSubActorKeeper(LiveActor*, const ActorInitInfo&, const char*, s32);
void initSubActorKeeperNoFile(LiveActor*, const ActorInitInfo&, s32);
void registerSubActor(LiveActor*, LiveActor*);
void registerSubActorSyncClipping(LiveActor*, LiveActor*);
void registerSubActorSyncClippingAndHide(LiveActor*, LiveActor*);
void registerSubActorSyncAll(LiveActor*, LiveActor*);
void setSubActorOffSyncClipping(LiveActor*);
void initScreenPointKeeper(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
void initScreenPointKeeperNoYaml(LiveActor*, s32);
void initActorMaterialCategory(LiveActor*, const ActorInitInfo&, const char*);
*/

}  // namespace al
