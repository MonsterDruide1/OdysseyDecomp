#include "Library/Sequence/SequenceUtil.h"

#include "Library/Audio/AudioDirector.h"
#include "Library/Audio/AudioDirectorInitInfo.h"
#include "Library/Scene/Scene.h"
#include "Library/Se/Function/SeFunction.h"
#include "Library/Se/SeDirector.h"
#include "Library/Sequence/IUseSceneCreator.h"
#include "Library/Sequence/Sequence.h"
#include "Library/Sequence/SequenceInitInfo.h"
#include "Project/Scene/SceneCreator.h"

namespace al {

void initSceneCreator(IUseSceneCreator* user, const SequenceInitInfo& initInfo,
                      GameDataHolderBase* gameDataHolder, AudioDirector* audioDirector,
                      ScreenCaptureExecutor* screenCaptureExecutor,
                      alSceneFunction::SceneFactory* sceneFactory) {
    SceneCreator* sceneCreator =
        new SceneCreator(initInfo.gameSystemInfo, gameDataHolder, screenCaptureExecutor,
                         sceneFactory, audioDirector);
    user->setSceneCreator(sceneCreator);
}

void createSceneAndInit(IUseSceneCreator* user, const char* sceneFactoryEntry,
                        const char* stageName, s32 scenarioNo, const char* sceneName) {
    user->getSceneCreator()->createScene(sceneFactoryEntry, stageName, scenarioNo, sceneName, false,
                                         -1);
}

void createSceneAndUseInitThread(IUseSceneCreator* user, const char* sceneFactoryEntry,
                                 s32 threadPriority, const char* stageName, s32 scenarioNo,
                                 const char* sceneName) {
    user->getSceneCreator()->createScene(sceneFactoryEntry, stageName, scenarioNo, sceneName, true,
                                         threadPriority);
}

void setSceneAndInit(IUseSceneCreator* user, Scene* scene, const char* stageName, s32 scenarioNo,
                     const char* sceneName) {
    user->getSceneCreator()->setSceneAndInit(scene, stageName, scenarioNo, sceneName);
}

void setSceneAndUseInitThread(IUseSceneCreator* user, Scene* scene, s32 threadPriority,
                              const char* stageName, s32 scenarioNo, const char* sceneName,
                              sead::Heap* heap) {
    user->getSceneCreator()->setSceneAndThreadInit(scene, stageName, scenarioNo, sceneName,
                                                   threadPriority, heap);
}

bool tryEndSceneInitThread(IUseSceneCreator* user) {
    return user->getSceneCreator()->tryEndInitThread();
}

bool isExistSceneInitThread(const IUseSceneCreator* user) {
    return user->getSceneCreator()->isExistInitThread();
}

void initAudioDirector(Sequence* sequence, AudioSystemInfo* audioSystemInfo,
                       AudioDirectorInitInfo& initInfo) {
    if (!sequence)
        return;
    initInfo.audioSystemInfo = audioSystemInfo;
    initInfo.demoDirector = nullptr;
    if (initInfo.curStage)
        initInfo.curStage = nullptr;
    if (initInfo.scenarioNo)
        initInfo.scenarioNo = 0;
    if (initInfo.seDirectorInitInfo.maxRequests <= 0)
        initInfo.seDirectorInitInfo.maxRequests = 30;
    if (initInfo.seDirectorInitInfo.playerCount <= 0)
        initInfo.seDirectorInitInfo.playerCount = 20;
    initInfo.bgmDirectorInitInfo.name = "Sequence";
    initInfo.duckingName = "DuckingForSequence";
    AudioDirector* audioDirector = new AudioDirector();
    audioDirector->init(initInfo);
    sequence->setAudioDirector(audioDirector);
}

void setSequenceAudioKeeperToSceneSeDirector(Sequence* sequence, Scene* scene) {
    if (!scene->getAudioDirector())
        return;
    SeDirector* seDirector = scene->getAudioDirector()->getSeDirector();
    if (!seDirector)
        return;

    SeKeeper* seKeeper = alSeFunction::tryGetSeKeeper(sequence);
    if (seKeeper)
        seDirector->setUpperLevelKeeper(seKeeper);
}

void setSequenceNameForActorPickTool(Sequence* sequence, Scene* scene) {}
}  // namespace al
