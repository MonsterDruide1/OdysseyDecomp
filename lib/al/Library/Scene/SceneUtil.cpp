#include "Library/Scene/SceneUtil.h"

#include <common/aglRenderBuffer.h>
#include <math/seadVector.h>

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/SwitchAreaDirector.h"
#include "Library/Audio/AudioDirector.h"
#include "Library/Audio/AudioDirectorInitInfo.h"
#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraFlagCtrl.h"
#include "Library/Camera/CameraPoseUpdater.h"
#include "Library/Camera/CameraPoserSceneInfo.h"
#include "Library/Camera/CameraRequestParamHolder.h"
#include "Library/Camera/CameraResourceHolder.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Camera/CameraViewCtrlPause.h"
#include "Library/Camera/CameraViewCtrlScene.h"
#include "Library/Camera/SceneCameraInfo.h"
#include "Library/Controller/PadRumbleDirector.h"
#include "Library/Draw/GraphicsQualityController.h"
#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Draw/ViewRenderer.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Effect/EffectSystem.h"
#include "Library/Execute/ExecuteUtil.h"
#include "Library/File/FileUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Layout/LayoutSystem.h"
#include "Library/Layout/LayoutUtil.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/LiveActor/LiveActorKit.h"
#include "Library/Model/ModelDrawBufferUpdater.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Play/Camera/SimpleCameraPoserFactory.h"
#include "Library/PostProcessing/PostProcessingFilter.h"
#include "Library/Resource/ResourceHolder.h"
#include "Library/Scene/DemoDirector.h"
#include "Library/Scene/Scene.h"
#include "Library/Scene/SceneStopCtrl.h"
#include "Library/Screen/ScreenCoverCtrl.h"
#include "Library/Se/Function/SeDbFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageResourceKeeper.h"
#include "Library/Stage/StageResourceList.h"
#include "Library/System/GameSystemInfo.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/Clipping/ClippingDirector.h"
#include "Project/Gravity/GravityHolder.h"
#include "Project/LiveActor/ActorExecuteFunction.h"

namespace al {

s32 getStageInfoMapNum(const Scene* scene) {
    if (!scene->getStageResourceKeeper())
        return 0;

    StageResourceList* resourceList = scene->getStageResourceKeeper()->getMapStageInfo();
    if (!resourceList)
        return 0;

    return resourceList->getStageResourceNum();
}

s32 getStageInfoDesignNum(const Scene* scene) {
    if (!scene->getStageResourceKeeper())
        return 0;

    StageResourceList* resourceList = scene->getStageResourceKeeper()->getDesignStageInfo();
    if (!resourceList)
        return 0;

    return resourceList->getStageResourceNum();
}

s32 getStageInfoSoundNum(const Scene* scene) {
    if (!scene->getStageResourceKeeper())
        return 0;

    StageResourceList* resourceList = scene->getStageResourceKeeper()->getSoundStageInfo();
    if (!resourceList)
        return 0;

    return resourceList->getStageResourceNum();
}

StageInfo* getStageInfoMap(const Scene* scene, s32 index) {
    return scene->getStageResourceKeeper()->getMapStageInfo()->getStageInfo(index);
}

StageInfo* getStageInfoDesign(const Scene* scene, s32 index) {
    return scene->getStageResourceKeeper()->getDesignStageInfo()->getStageInfo(index);
}

StageInfo* getStageInfoSound(const Scene* scene, s32 index) {
    return scene->getStageResourceKeeper()->getSoundStageInfo()->getStageInfo(index);
}

Resource* getStageResourceMap(const Scene* scene, s32 index) {
    return getStageInfoMap(scene, index)->getResource();
}

Resource* tryGetStageResourceDesign(const Scene* scene, s32 index) {
    if (getStageInfoDesignNum(scene) == 0)
        return nullptr;
    return scene->getStageResourceKeeper()
        ->getDesignStageInfo()
        ->getStageInfo(index)
        ->getResource();
}

Resource* tryGetStageResourceSound(const Scene* scene, s32 index) {
    if (getStageInfoSoundNum(scene) == 0)
        return nullptr;
    return scene->getStageResourceKeeper()->getSoundStageInfo()->getStageInfo(index)->getResource();
}

agl::RenderBuffer* getSceneFrameBufferMain(const Scene* scene) {
    DrawSystemInfo* drawSystemInfo = scene->getDrawSystemInfo();
    return drawSystemInfo->isDocked ? drawSystemInfo->dockedRenderBuffer :
                                      drawSystemInfo->handheldRenderBuffer;
}

agl::DrawContext* getSceneDrawContext(const Scene* scene) {
    return scene->getDrawSystemInfo()->drawContext;
}

agl::RenderBuffer* getSceneFrameBufferConsole(const Scene* scene) {
    return scene->getDrawSystemInfo()->dockedRenderBuffer;
}

agl::RenderBuffer* getSceneFrameBufferHandheld(const Scene* scene) {
    return scene->getDrawSystemInfo()->handheldRenderBuffer;
}

f32 getSceneFrameBufferMainAspect(const Scene* scene) {
    const sead::Vector2f& virtualSize = getSceneFrameBufferMain(scene)->getVirtualSize();

    return virtualSize.x / virtualSize.y;
}

bool isChangedGraphicsQualityMode(const Scene* scene) {
    return scene->getLiveActorKit()
        ->getGraphicsSystemInfo()
        ->getGraphicsQualityController()
        ->isChangedGraphicsQualityMode();
}

AreaObjDirector* getSceneAreaObjDirector(const Scene* scene) {
    return scene->getLiveActorKit()->getAreaObjDirector();
}

ExecuteDirector* getSceneExecuteDirector(const Scene* scene) {
    return scene->getLiveActorKit()->getExecuteDirector();
}

PlayerHolder* getScenePlayerHolder(const Scene* scene) {
    return scene->getLiveActorKit()->getPlayerHolder();
}

ItemDirectorBase* getSceneItemDirector(const Scene* scene) {
    return scene->getLiveActorKit()->getItemDirector();
}

void initActorInitInfo(ActorInitInfo* actorInitInfo, const Scene* scene,
                       const PlacementInfo* placementInfo, const LayoutInitInfo* layoutInfo,
                       const ActorFactory* actorFactory, SceneMsgCtrl* sceneMsgCtrl,
                       GameDataHolderBase* gameDataHolderBase) {
    LiveActorKit* actorKit = scene->getLiveActorKit();

    actorInitInfo->initNew(
        placementInfo, layoutInfo, actorKit->getLiveActorGroupAllActors(), actorFactory,
        actorKit->getActorResourceHolder(), actorKit->getAreaObjDirector(),
        scene->getAudioDirector(), actorKit->getCameraDirector(), actorKit->getClippingDirector(),
        actorKit->getCollisionDirector(), actorKit->getDemoDirector(),
        actorKit->getEffectSystem() ? actorKit->getEffectSystem()->getEffectSystemInfo() : nullptr,
        actorKit->getExecuteDirector(), gameDataHolderBase, actorKit->getGravityHolder(),
        actorKit->getHitSensorDirector(), actorKit->getItemDirector(),
        actorKit->getNatureDirector(), actorKit->getGamePadSystem(),
        actorKit->getPadRumbleDirector(), actorKit->getPlayerHolder(), scene->getSceneObjHolder(),
        sceneMsgCtrl, scene->getSceneStopCtrl(), scene->getScreenCoverCtrl(),
        actorKit->getScreenPointDirector(), actorKit->getShadowDirector(),
        actorKit->getStageSwitchDirector(), actorKit->getModelGroup(),
        actorKit->getGraphicsSystemInfo(), actorKit->getModelDrawBufferCounter(),
        actorKit->getDynamicDrawActorGroup());
}

void initLayoutInitInfo(LayoutInitInfo* layoutInfo, const Scene* scene,
                        const SceneInitInfo& sceneInfo) {
    LiveActorKit* actorKit = scene->getLiveActorKit();
    LayoutKit* layoutKit = scene->getLayoutKit();

    if (actorKit) {
        layoutInfo->init(
            actorKit->getExecuteDirector(), actorKit->getEffectSystem()->getEffectSystemInfo(),
            scene->getSceneObjHolder(), scene->getAudioDirector(), actorKit->getCameraDirector(),
            sceneInfo.gameSysInfo->layoutSystem, sceneInfo.gameSysInfo->messageSystem,
            sceneInfo.gameSysInfo->gamePadSystem, actorKit->getPadRumbleDirector());
    } else {
        layoutInfo->init(layoutKit->getExecuteDirector(),
                         layoutKit->getEffectSystem()->getEffectSystemInfo(),
                         scene->getSceneObjHolder(), scene->getAudioDirector(), nullptr,
                         sceneInfo.gameSysInfo->layoutSystem, sceneInfo.gameSysInfo->messageSystem,
                         sceneInfo.gameSysInfo->gamePadSystem, nullptr);
    }

    layoutInfo->setDrawContext(scene->getDrawSystemInfo()->drawContext);
    if (layoutKit)
        layoutInfo->setDrawInfo(layoutKit->getDrawInfo());
    if (actorKit) {
        layoutInfo->setOcclusionCullingJudge(
            actorKit->getGraphicsSystemInfo()->getOcclusionCullingJudge());
    }
}

// https://decomp.me/scratch/CBhyQ
// NON_MATCHING: Bad order of operations
void initPlacementAreaObj(Scene* scene, const ActorInitInfo& actorInfo) {
    AreaInitInfo areaInitInfo[256];
    s32 entries = 0;

    for (s32 i = 0; i < getStageInfoMapNum(scene); i++) {
        StageInfo* stageInfo = getStageInfoMap(scene, i);
        PlacementInfo placementInfo;
        if (tryGetPlacementInfo(&placementInfo, stageInfo, "AreaList")) {
            initAreaInitInfo(&areaInitInfo[entries], placementInfo, actorInfo);
            entries++;
        }
    }
    for (s32 i = 0; i < getStageInfoDesignNum(scene); i++) {
        StageInfo* stageInfo = getStageInfoDesign(scene, i);
        PlacementInfo placementInfo;
        if (tryGetPlacementInfo(&placementInfo, stageInfo, "AreaList")) {
            initAreaInitInfo(&areaInitInfo[entries], placementInfo, actorInfo);
            entries++;
        }
    }
    for (s32 i = 0; i < getStageInfoSoundNum(scene); i++) {
        StageInfo* stageInfo = getStageInfoSound(scene, i);
        PlacementInfo placementInfo;
        if (tryGetPlacementInfo(&placementInfo, stageInfo, "AreaList")) {
            initAreaInitInfo(&areaInitInfo[entries], placementInfo, actorInfo);
            entries++;
        }
    }

    actorInfo.actorSceneInfo.areaObjDirector->placement(areaInitInfo, entries);
}

void initPlacementGravityObj(Scene* scene) {
    PlacementInfo placementInfo;
    for (s32 i = 0; i < getStageInfoMapNum(scene); i++) {
        StageInfo* stageInfo = getStageInfoMap(scene, i);

        s32 count;
        if (tryGetPlacementInfoAndCount(&placementInfo, &count, stageInfo, "GravityList")) {
            for (s32 j = 0; j < count; j++) {
                PlacementInfo placementInfo2;
                getPlacementInfoByIndex(&placementInfo2, placementInfo, j);
                if (isClassName(placementInfo2, "GravityPoint"))

                    scene->getLiveActorKit()->getGravityHolder()->createGravity(placementInfo2);
            }
        }
    }
}

bool tryGetPlacementInfoAndCount(PlacementInfo* outPlacementInfo, s32* outCount,
                                 const StageInfo* stageInfo, const char* key) {
    if (tryGetPlacementInfo(outPlacementInfo, stageInfo, key)) {
        *outCount = getCountPlacementInfo(*outPlacementInfo);
        return true;
    }

    *outCount = 0;
    return false;
}

void initPlacementObjectMap(Scene* scene, const ActorInitInfo& actorInfo, const char* name) {
    s32 resourceNum = getStageInfoMapNum(scene);
    for (s32 i = 0; i < resourceNum; i++)
        initPlacementByStageInfo(getStageInfoMap(scene, i), name, actorInfo);
}

void initPlacementByStageInfo(const StageInfo* stageInfo, const char* key,
                              const ActorInitInfo& actorInfo) {
    PlacementInfo placementInfo;
    if (tryGetPlacementInfo(&placementInfo, stageInfo, key)) {
        s32 count = getCountPlacementInfo(placementInfo);

        for (s32 i = 0; i < count; i++) {
            PlacementInfo placementInfo2;
            getPlacementInfoByIndex(&placementInfo2, placementInfo, i);
            createPlacementActorFromFactory(actorInfo, &placementInfo2);
        }
    }
}

void initPlacementObjectDesign(Scene* scene, const ActorInitInfo& actorInfo, const char* name) {
    s32 resourceNum = getStageInfoDesignNum(scene);
    for (s32 i = 0; i < resourceNum; i++)
        initPlacementByStageInfo(getStageInfoDesign(scene, i), name, actorInfo);
}

void initPlacementObjectSound(Scene* scene, const ActorInitInfo& actorInfo, const char* name) {
    s32 resourceNum = getStageInfoSoundNum(scene);
    for (s32 i = 0; i < resourceNum; i++)
        initPlacementByStageInfo(getStageInfoSound(scene, i), name, actorInfo);
}

LiveActor* tryInitPlacementSingleObject(Scene* scene, const ActorInitInfo& actorInfo,
                                        s32 resourceType, const char* key) {
    if (!scene->getStageResourceKeeper())
        return nullptr;

    StageResourceList* resourceList =
        scene->getStageResourceKeeper()->getStageResourceList(resourceType);
    if (!resourceList)
        return nullptr;

    s32 resourceNum = resourceList->getStageResourceNum();

    LiveActor* actor = nullptr;
    for (s32 i = 0; i < resourceNum; i++) {
        PlacementInfo placementInfo;
        StageInfo* stageInfo =
            scene->getStageResourceKeeper()->getStageResourceList(resourceType)->getStageInfo(i);

        if (tryGetPlacementInfo(&placementInfo, stageInfo, key)) {
            s32 count = getCountPlacementInfo(placementInfo);
            for (s32 j = 0; j < count; j++) {
                PlacementInfo newPlacementInfo;
                getPlacementInfoByIndex(&newPlacementInfo, placementInfo, j);
                LiveActor* newActor = createPlacementActorFromFactory(actorInfo, &newPlacementInfo);
                if (newActor)
                    actor = newActor;
            }
        }
    }
    return actor;
}

LiveActor* tryInitPlacementSingleObject(Scene* scene, const ActorInitInfo& actorInfo,
                                        s32 resourceType, const char* key, const char* name) {
    if (!scene->getStageResourceKeeper())
        return nullptr;

    StageResourceList* resourceList =
        scene->getStageResourceKeeper()->getStageResourceList(resourceType);
    if (!resourceList)
        return nullptr;

    s32 resourceNum = resourceList->getStageResourceNum();

    LiveActor* actor = nullptr;
    for (s32 i = 0; i < resourceNum; i++) {
        PlacementInfo placementInfo;
        StageInfo* stageInfo =
            scene->getStageResourceKeeper()->getStageResourceList(resourceType)->getStageInfo(i);

        if (tryGetPlacementInfo(&placementInfo, stageInfo, key)) {
            s32 count = getCountPlacementInfo(placementInfo);
            for (s32 j = 0; j < count; j++) {
                PlacementInfo newPlacementInfo;
                getPlacementInfoByIndex(&newPlacementInfo, placementInfo, j);
                const char* objName = nullptr;
                getObjectName(&objName, newPlacementInfo);
                if (isEqualString(objName, name)) {
                    LiveActor* newActor =
                        createPlacementActorFromFactory(actorInfo, &newPlacementInfo);
                    if (newActor)
                        actor = newActor;
                }
            }
        }
    }
    return actor;
}

bool tryInitPlacementActorGroup(LiveActorGroup* liveActorGroup, Scene* scene,
                                const ActorInitInfo& actorInfo, s32 resourceType, const char* key,
                                const char* name) {
    if (!scene->getStageResourceKeeper())
        return false;

    StageResourceList* resourceList =
        scene->getStageResourceKeeper()->getStageResourceList(resourceType);
    if (!resourceList)
        return false;

    s32 resourceNum = resourceList->getStageResourceNum();
    bool isRegistered = false;
    for (s32 i = 0; i < resourceNum; i++) {
        PlacementInfo placementInfo;
        StageInfo* stageInfo =
            scene->getStageResourceKeeper()->getStageResourceList(resourceType)->getStageInfo(i);

        if (tryGetPlacementInfo(&placementInfo, stageInfo, key)) {
            s32 count = getCountPlacementInfo(placementInfo);
            for (s32 j = 0; j < count; j++) {
                PlacementInfo newPlacementInfo;
                getPlacementInfoByIndex(&newPlacementInfo, placementInfo, j);
                const char* objName = nullptr;
                getObjectName(&objName, newPlacementInfo);
                if (isEqualString(objName, name)) {
                    LiveActor* actor =
                        createPlacementActorFromFactory(actorInfo, &newPlacementInfo);
                    if (actor) {
                        liveActorGroup->registerActor(actor);
                        isRegistered = true;
                    }
                }
            }
        }
    }
    return isRegistered;
}

void initPlacementByStageInfoSingle(const StageInfo* stageInfo, const char* key,
                                    const ActorInitInfo& actorInfo) {
    initPlacementByStageInfo(stageInfo, key, actorInfo);
}

bool tryGetPlacementInfo(PlacementInfo* outPlacementInfo, const StageInfo* stageInfo,
                         const char* key) {
    ByamlIter iter;
    if (stageInfo->getPlacementIter().tryGetIterByKey(&iter, key)) {
        outPlacementInfo->set(iter, stageInfo->getZoneIter());
        return true;
    }

    return false;
}

void getPlacementInfo(PlacementInfo* outPlacementInfo, const StageInfo* stageInfo,
                      const char* key) {
    ByamlIter iter;
    stageInfo->getPlacementIter().tryGetIterByKey(&iter, key);

    outPlacementInfo->set(iter, stageInfo->getZoneIter());
}

void getPlacementInfoAndCount(PlacementInfo* outPlacementInfo, s32* outCount,
                              const StageInfo* stageInfo, const char* key) {
    getPlacementInfo(outPlacementInfo, stageInfo, key);
    *outCount = getCountPlacementInfo(*outPlacementInfo);
}

void initAreaObjDirector(Scene* scene, const AreaObjFactory* factory) {
    scene->getLiveActorKit()->getAreaObjDirector()->init(factory);
}

void initDemoDirector(Scene* scene, DemoDirector* demoDirector) {
    scene->getLiveActorKit()->setDemoDirector(demoDirector);
}

void initHitSensorDirector(Scene* scene) {
    scene->getLiveActorKit()->initHitSensorDirector();
}

void initGravityHolder(Scene* scene) {
    scene->getLiveActorKit()->initGravityHolder();
}

void initItemDirector(Scene* scene, ItemDirectorBase* itemDirector) {
    scene->getLiveActorKit()->setItemDirector(itemDirector);
}

void initNatureDirector(Scene* scene) {
    scene->getLiveActorKit()->initNatureDirector();
}

void initSwitchAreaDirector(Scene* scene, s32 val1, s32 val2) {
    scene->getLiveActorKit()->initSwitchAreaDirector(val1, val2);
}

void registerSwitchOnAreaGroup(Scene* scene, SwitchOnAreaGroup* switchOnAreaGroup) {
    scene->getLiveActorKit()->getSwitchAreaDirector()->registerSwitchOnAreaGroup(switchOnAreaGroup);
}

void registerSwitchKeepOnAreaGroup(Scene* scene, SwitchKeepOnAreaGroup* switchKeepOnAreaGroup) {
    scene->getLiveActorKit()->getSwitchAreaDirector()->registerSwitchKeepOnAreaGroup(
        switchKeepOnAreaGroup);
}

void initGraphicsSystemInfo(Scene* scene, const char* name, s32 index) {
    if (1 < index) {
        StringTmp<32> scenario{"Scenario%d"};
        GraphicsSystemInfo* systemInfo = scene->getLiveActorKit()->getGraphicsSystemInfo();

        Resource* resource;
        if (getStageInfoDesignNum(scene) != 0)
            resource = getStageInfoDesign(scene, 0)->getResource();
        else
            resource = nullptr;
        systemInfo->initStageResource(resource, name, scenario.cstr());
        return;
    }

    GraphicsSystemInfo* systemInfo = scene->getLiveActorKit()->getGraphicsSystemInfo();

    Resource* resource;
    if (getStageInfoDesignNum(scene) != 0)
        resource = getStageInfoDesign(scene, 0)->getResource();
    else
        resource = nullptr;
    systemInfo->initStageResource(resource, name, nullptr);
}

void initCameraDirectorImpl(Scene* scene, const CameraPoserFactory* cameraPoserFactory) {
    LiveActorKit* actorKit = scene->getLiveActorKit();

    CameraPoserSceneInfo* sceneInfo = new CameraPoserSceneInfo();
    sceneInfo->init(actorKit->getAreaObjDirector(), actorKit->getCollisionDirector(),
                    scene->getAudioDirector());

    if (!cameraPoserFactory)
        cameraPoserFactory = new SimpleCameraPoserFactory("カメラファクトリー");

    scene->getLiveActorKit()->getCameraDirector()->init(sceneInfo, cameraPoserFactory);
    registerExecutorUser(scene->getLiveActorKit()->getCameraDirector(),
                         actorKit->getExecuteDirector(), "カメラ");
}

void initCameraDirector(Scene* scene, const char* name, s32 index,
                        const CameraPoserFactory* cameraPoserFactory) {
    initCameraDirectorImpl(scene, cameraPoserFactory);
    CameraDirector* cameraDirector = scene->getLiveActorKit()->getCameraDirector();

    CameraResourceHolder* cameraResourceHolder =
        new CameraResourceHolder(name, getStageInfoMapNum(scene));

    for (s32 i = 0; i < getStageInfoMapNum(scene); i++) {
        cameraResourceHolder->tryInitCameraResource(getStageInfoMap(scene, i)->getResource(),
                                                    i > 0 ? 1 : index);
    }

    cameraDirector->initResourceHolder(cameraResourceHolder);
    cameraDirector->initAreaCameraSwitcherSingle();
}

void initCameraDirectorWithoutStageResource(Scene* scene,
                                            const CameraPoserFactory* cameraPoserFactory) {
    initCameraDirectorImpl(scene, cameraPoserFactory);
}

void initCameraDirectorFix(Scene* scene, const sead::Vector3f& pos, const sead::Vector3f& lookAtPos,
                           const CameraPoserFactory* cameraPoserFactory) {
    initCameraDirectorImpl(scene, cameraPoserFactory);
    startCamera(scene, initFixCamera(scene, "Scene", pos, lookAtPos));
}

void initSceneCameraFovyDegree(Scene* scene, f32 fov) {
    scene->getLiveActorKit()->getCameraDirector()->initSceneFovyDegree(fov);
}

void initSnapShotCameraAudioKeeper(Scene* scene, IUseAudioKeeper* audioKeeper) {
    scene->getLiveActorKit()->getCameraDirector()->initSnapShotCameraAudioKeeper(audioKeeper);
}

void setCameraAspect(Scene* scene, f32 aspectA, f32 aspectB) {
    scene->getLiveActorKit()->getCameraDirector()->getPoseUpdater(0)->setAspect(aspectA);
    if (aspectB != -1.0f)
        scene->getLiveActorKit()->getCameraDirector()->getPoseUpdater(1)->setAspect(aspectB);
}

void resetSceneInitEntranceCamera(Scene* scene) {
    scene->getLiveActorKit()
        ->getCameraDirector()
        ->getSceneCameraCtrl()
        ->getSceneViewAt(0)
        ->resetViewName();
}

void stopCameraByDeathPlayer(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->stopByDeathPlayer();
}

void restartCameraByDeathPlayer(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->restartByDeathPlayer();
}

void startInvalidCameraStopJudgeByDemo(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->startInvalidStopJudgeByDemo();
}

void endInvalidCameraStopJudgeByDemo(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->endInvalidStopJudgeByDemo();
}

void startCameraSnapShotMode(Scene* scene, bool useInterpole) {
    scene->getLiveActorKit()->getCameraDirector()->startSnapShotMode(useInterpole);
}

void endCameraSnapShotMode(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->endSnapShotMode();
}

bool isCameraReverseInputH(const Scene* scene) {
    return scene->getLiveActorKit()->getCameraDirector()->getFlagCtrl()->isCameraReverseInputH;
}

void onCameraReverseInputH(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->getFlagCtrl()->isCameraReverseInputH = true;
}

void offCameraReverseInputH(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->getFlagCtrl()->isCameraReverseInputH = false;
}

bool isCameraReverseInputV(const Scene* scene) {
    return scene->getLiveActorKit()->getCameraDirector()->getFlagCtrl()->isCameraReverseInputV;
}

void onCameraReverseInputV(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->getFlagCtrl()->isCameraReverseInputV = true;
}

void offCameraReverseInputV(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->getFlagCtrl()->isCameraReverseInputV = false;
}

s32 getCameraStickSensitivityLevel(const Scene* scene) {
    return scene->getLiveActorKit()
        ->getCameraDirector()
        ->getSceneCameraCtrl()
        ->getRequestParamHolder()
        ->getStickSensitivityLevel();
}

void setCameraStickSensitivityLevel(Scene* scene, s32 sensitivityLevel) {
    scene->getLiveActorKit()
        ->getCameraDirector()
        ->getSceneCameraCtrl()
        ->getRequestParamHolder()
        ->setStickSensitivityLevel(sensitivityLevel);
}

bool isValidCameraGyro(const Scene* scene) {
    return !scene->getLiveActorKit()->getCameraDirector()->getFlagCtrl()->isInvalidCameraGyro;
}

void validateCameraGyro(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->getFlagCtrl()->isInvalidCameraGyro = false;
}

void invalidateCameraGyro(Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->getFlagCtrl()->isInvalidCameraGyro = true;
}

s32 getCameraGyroSensitivityLevel(const Scene* scene) {
    return scene->getLiveActorKit()
        ->getCameraDirector()
        ->getSceneCameraCtrl()
        ->getRequestParamHolder()
        ->getGyroSensitivityLevel();
}

void setCameraGyroSensitivityLevel(Scene* scene, s32 sensitivityLevel) {
    scene->getLiveActorKit()
        ->getCameraDirector()
        ->getSceneCameraCtrl()
        ->getRequestParamHolder()
        ->setGyroSensitivityLevel(sensitivityLevel);
}

PauseCameraCtrl* initAndCreatePauseCameraCtrl(Scene* scene, f32 value) {
    return scene->getLiveActorKit()->getCameraDirector()->initAndCreatePauseCameraCtrl(value);
}

void startCameraPause(PauseCameraCtrl* pauseCameraCtrl) {
    pauseCameraCtrl->startCameraPause();
}

void endCameraPause(PauseCameraCtrl* pauseCameraCtrl) {
    pauseCameraCtrl->endCameraPause();
}

AudioDirector* initAudioDirectorImpl(Scene* scene, const SceneInitInfo& sceneInfo,
                                     AudioDirectorInitInfo& audioDirectorInfo) {
    audioDirectorInfo.audioSystemInfo =
        sceneInfo.gameSysInfo->audioSystem ?
            sceneInfo.gameSysInfo->audioSystem->getAudioSystemInfo() :
            nullptr;

    if (!audioDirectorInfo.curStage)
        audioDirectorInfo.curStage = sceneInfo.initStageName;
    if (audioDirectorInfo.scenarioNo == 0)
        audioDirectorInfo.scenarioNo = sceneInfo.scenarioNo;
    if (audioDirectorInfo.seDirectorInitInfo.maxRequests < 1)
        audioDirectorInfo.seDirectorInitInfo.maxRequests = 200;
    if (audioDirectorInfo.seDirectorInitInfo.playerCount < 1)
        audioDirectorInfo.seDirectorInitInfo.playerCount = 40;

    audioDirectorInfo.bgmDirectorInitInfo.field_0 = true;
    audioDirectorInfo.bgmDirectorInitInfo.field_8 = "Scene";
    audioDirectorInfo.duckingName = "DuckingForScene";

    AudioDirector* audioDirector = new AudioDirector();
    audioDirector->init(audioDirectorInfo);
    scene->setAudioDirector(audioDirector);
    audioDirector->setDependentModule(sceneInfo.audioDirector);
    return audioDirector;
}

void initAudioDirector2D(Scene* scene, const SceneInitInfo& sceneInfo,
                         AudioDirectorInitInfo& audioDirectorInfo) {
    initAudioDirectorImpl(scene, sceneInfo, audioDirectorInfo);
}

void initAudioDirector3D(Scene* scene, const SceneInitInfo& sceneInfo,
                         AudioDirectorInitInfo& audioDirectorInfo) {
    initAudioDirector3D(scene, sceneInfo, audioDirectorInfo, &getLookAtCamera(scene, 0),
                        &getProjection(scene, 0), scene->getLiveActorKit()->getAreaObjDirector());
}

void initAudioDirector3D(Scene* scene, const SceneInitInfo& sceneInfo,
                         AudioDirectorInitInfo& audioDirectorInfo,
                         const sead::LookAtCamera* lookAtCamera, const Projection* projection,
                         AreaObjDirector* areaObjDirector) {
    if (!audioDirectorInfo.seDirectorInitInfo.cameraPos)
        audioDirectorInfo.seDirectorInitInfo.cameraPos = &lookAtCamera->getPos();
    if (!audioDirectorInfo.seDirectorInitInfo.cameraMatrix)
        audioDirectorInfo.seDirectorInitInfo.cameraMatrix = &lookAtCamera->getMatrix();
    if (!audioDirectorInfo.seDirectorInitInfo.cameraProjection)
        audioDirectorInfo.seDirectorInitInfo.cameraProjection = projection;
    if (!audioDirectorInfo.seDirectorInitInfo.cameraAt)
        audioDirectorInfo.seDirectorInitInfo.cameraAt = &lookAtCamera->getAt();
    if (!audioDirectorInfo.areaObjDirector)
        audioDirectorInfo.areaObjDirector = areaObjDirector;
    if (!audioDirectorInfo.demoDirector)
        audioDirectorInfo.demoDirector = scene->getLiveActorKit()->getDemoDirector();

    initAudioDirectorImpl(scene, sceneInfo, audioDirectorInfo)->init3D(audioDirectorInfo);
}

void initSceneAudioKeeper(Scene* scene, const SceneInitInfo& sceneInfo, const char* name) {
    AudioSystemInfo* audioSystemInfo = nullptr;
    if (sceneInfo.gameSysInfo->audioSystem)
        audioSystemInfo = sceneInfo.gameSysInfo->audioSystem->getAudioSystemInfo();

    const char* seUserName = alSeDbFunction::tryFindSceneSeUserName(
        audioSystemInfo, sceneInfo.initStageName, sceneInfo.scenarioNo);

    if (seUserName)
        name = seUserName;

    scene->setAudioKeeper(
        alAudioKeeperFunction::createAudioKeeper(scene->getAudioDirector(), name, nullptr));
}

void setIsSafeFinalizingInParallelThread(Scene* scene, bool isSafe) {
    if (scene->getAudioDirector())
        scene->getAudioDirector()->setIsSafeFinalizingInParallelThread(isSafe);
}

void updateKit(Scene* scene) {
    executeUpdate(scene->getLiveActorKit(), nullptr);
}

void updateKitTable(Scene* scene, const char* name) {
    executeUpdateTable(scene->getLiveActorKit(), name);
}

void updateKitList(Scene* scene, const char* listName, const char* name) {
    executeUpdateList(scene->getLiveActorKit(), listName, name);
}

void updateKitList(Scene* scene, const char* name) {
    updateKitList(scene, "更新", name);
}

void updateLayoutKit(Scene* scene) {
    executeUpdate(scene->getLayoutKit());
}

void updateEffect(Scene* scene) {
    alExecuteFunction::updateEffect(scene->getLiveActorKit()->getExecuteDirector());
}

void updateEffectSystem(Scene* scene) {
    alExecuteFunction::updateEffectSystem(scene->getLiveActorKit()->getExecuteDirector());
}

void updateEffectPlayer(Scene* scene) {
    alExecuteFunction::updateEffectPlayer(scene->getLiveActorKit()->getExecuteDirector());
}

void updateEffectDemo(Scene* scene) {
    alExecuteFunction::updateEffectDemo(scene->getLiveActorKit()->getExecuteDirector());
}

void updateEffectDemoWithPause(Scene* scene) {
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（前処理）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（プレイヤー）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（Ｚソート）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（カメラデモ）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（カメラ前エフェクト）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（ベース２Ｄ）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（２Ｄ）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（後処理）");
}

void updateEffectLayout(Scene* scene) {
    alExecuteFunction::updateEffectLayout(scene->getLiveActorKit()->getExecuteDirector());
}

void updateGraphicsPrev(Scene* scene) {
    LiveActorKit* actorKit = scene->getLiveActorKit();
    if (actorKit && actorKit->getGraphicsSystemInfo() && actorKit->getCameraDirector())
        actorKit->preDrawGraphics();
}

void updateKitListPrev(Scene* scene) {
    scene->getLiveActorKit()->clearGraphicsRequest();
    scene->getLiveActorKit()->getPadRumbleDirector()->update();
}

void updateKitListPost(Scene* scene) {
    executeUpdateList(scene->getLiveActorKit(), "更新", "帽子装着位置更新");
    alExecuteFunction::updateEffect(scene->getLiveActorKit()->getExecuteDirector());
}

void updateKitListPostDemoWithPauseNormalEffect(Scene* scene) {
    executeUpdateList(scene->getLiveActorKit(), "更新", "帽子装着位置更新");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（前処理）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（プレイヤー）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（Ｚソート）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（カメラデモ）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（カメラ前エフェクト）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（ベース２Ｄ）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（２Ｄ）");
    executeUpdateList(scene->getLiveActorKit(), "更新", "エフェクト（後処理）");
}

void updateKitListPostOnNerveEnd(Scene* scene) {
    scene->getLiveActorKit()->updateGraphics();
    updateGraphicsPrev(scene);
}

void drawKit(const Scene* scene, const char* name) {
    executeDraw(scene->getLiveActorKit(), name);
}

void drawKitList(const Scene* scene, const char* listName, const char* name) {
    executeDrawList(scene->getLiveActorKit(), listName, name);
}

void drawLayoutKit(const Scene* scene, const char* name) {
    executeDraw(scene->getLayoutKit(), name);
}

void drawEffectDeferred(const Scene* scene, s32 index) {
    SceneCameraInfo* cameraInfo =
        scene->getLiveActorKit()->getCameraDirector()->getSceneCameraInfo();
    alEffectSystemFunction::drawEffectDeferred(
        scene->getLiveActorKit()->getEffectSystem(), getProjectionMtx(cameraInfo, index),
        getViewMtx(cameraInfo, index), getNear(cameraInfo, index), getFar(cameraInfo, index),
        getFovy(cameraInfo, index));
}

void startForwardPlayerScreenFader(const Scene* scene, s32 a, s32 b, f32 c) {
    scene->getLiveActorKit()
        ->getGraphicsSystemInfo()
        ->getViewRenderer()
        ->startForwardPlayerScreenFader(a, b, c);
}

void endForwardPlayerScreenFader(const Scene* scene, s32 a) {
    scene->getLiveActorKit()
        ->getGraphicsSystemInfo()
        ->getViewRenderer()
        ->endForwardPlayerScreenFader(a);
}

bool isStopScene(const Scene* scene) {
    return scene->getSceneStopCtrl()->isStop();
}

void startCheckViewCtrlByCameraPos(Scene* scene) {
    scene->getLiveActorKit()->getClippingDirector()->startCheckViewCtrlByCameraPos();
}

void startCheckViewCtrlByLookAtPos(Scene* scene) {
    scene->getLiveActorKit()->getClippingDirector()->startCheckViewCtrlByLookAtPos();
}

void startCheckViewCtrlByPlayerPos(Scene* scene) {
    scene->getLiveActorKit()->getClippingDirector()->startCheckViewCtrlByPlayerPos();
}

bool isExistScreenCoverCtrl(const Scene* scene) {
    return scene->getScreenCoverCtrl() != nullptr;
}

bool isRequestCaptureScreenCover(const Scene* scene) {
    return scene->getScreenCoverCtrl()->isRequestCapture();
}

void resetRequestCaptureScreenCover(const Scene* scene) {
    scene->getScreenCoverCtrl()->resetRequestCapture();
}

bool isOffDrawScreenCover(const Scene* scene) {
    return scene->getScreenCoverCtrl()->getCoverFrames() == 0;
}

void resetCaptureScreenCover(const Scene* scene) {
    scene->getScreenCoverCtrl()->resetCapture();
}

void validatePostProcessingFilter(const Scene* scene) {
    scene->getLiveActorKit()->getGraphicsSystemInfo()->getPostProcessingFilter()->validate();
}

void invalidatePostProcessingFilter(const Scene* scene) {
    scene->getLiveActorKit()->getGraphicsSystemInfo()->getPostProcessingFilter()->invalidate();
}

void incrementPostProcessingFilterPreset(const Scene* scene) {
    scene->getLiveActorKit()->getGraphicsSystemInfo()->getPostProcessingFilter()->incrementPreset();
}

void decrementPostProcessingFilterPreset(const Scene* scene) {
    scene->getLiveActorKit()->getGraphicsSystemInfo()->getPostProcessingFilter()->decrementPreset();
}

s32 getPostProcessingFilterPresetId(const Scene* scene) {
    return scene->getLiveActorKit()
        ->getGraphicsSystemInfo()
        ->getPostProcessingFilter()
        ->getPresetId();
}

bool isActiveDemo(const Scene* scene) {
    return scene->getLiveActorKit()->getDemoDirector()->isActiveDemo();
}

const char* getActiveDemoName(const Scene* scene) {
    return scene->getLiveActorKit()->getDemoDirector()->getActiveDemoName();
}

LiveActor** getDemoActorList(const Scene* scene) {
    return scene->getLiveActorKit()->getDemoDirector()->getDemoActorList();
}

s32 getDemoActorNum(const Scene* scene) {
    return scene->getLiveActorKit()->getDemoDirector()->getDemoActorNum();
}

void updateDemoActor(const Scene* scene) {
    scene->getLiveActorKit()->getDemoDirector()->updateDemoActor(nullptr);
}

void updateDemoActorForPauseEffect(const Scene* scene) {
    EffectSystem* effectSystem = scene->getLiveActorKit()->getEffectSystem();
    scene->getLiveActorKit()->getDemoDirector()->updateDemoActor(effectSystem);
}

void stopAllSe(const Scene* scene, u32 index) {
    if (scene && scene->getAudioDirector()) {
        alSeFunction::stopAllSe(scene->getAudioDirector(), index);
        return;
    }
}

void initPadRumble(const Scene* scene, const SceneInitInfo& sceneInfo) {
    WaveVibrationHolder* waveVibrationHolder = sceneInfo.gameSysInfo->waveVibrationHolder;
    scene->getLiveActorKit()->getPadRumbleDirector()->setWaveVibrationHolder(waveVibrationHolder);

    alAudioSystemFunction::setPadRumbleDirectorForSe(
        scene->getAudioDirector(), scene->getLiveActorKit()->getPadRumbleDirector());
}

void stopPadRumble(const Scene* scene) {
    scene->getLiveActorKit()->getPadRumbleDirector()->stopAllRumble();
}

void pausePadRumble(const Scene* scene) {
    scene->getLiveActorKit()->getPadRumbleDirector()->pause();
}

void endPausePadRumble(const Scene* scene) {
    scene->getLiveActorKit()->getPadRumbleDirector()->endPause();
}

void validatePadRumble(Scene* scene) {
    scene->getLiveActorKit()->getPadRumbleDirector()->validatePadRumble();
}

void invalidatePadRumble(Scene* scene) {
    scene->getLiveActorKit()->getPadRumbleDirector()->invalidatePadRumble();
}

void setPadRumblePowerLevel(Scene* scene, s32 powerLevel) {
    scene->getLiveActorKit()->getPadRumbleDirector()->setPowerLevel(powerLevel);
}

const Resource* getPreLoadFileListArc() {
    return findResource("SystemData/PreLoad");
}

bool tryRequestPreLoadFile(const Scene* scene, const SceneInitInfo& sceneInfo, s32 index,
                           sead::Heap* heap) {
    StringTmp<128> stageName{"%s%d", sceneInfo.initStageName, index};
    return tryRequestPreLoadFile(getPreLoadFileListArc(), stageName, heap, nullptr);
}

}  // namespace al

namespace alSceneFunction {
void initAreaCameraSwitcherMultiForPrototype(const al::Scene* scene) {
    scene->getLiveActorKit()->getCameraDirector()->initAreaCameraSwitcherMultiForPrototype(
        scene->getLiveActorKit()->getAreaObjDirector());
}
}  // namespace alSceneFunction
