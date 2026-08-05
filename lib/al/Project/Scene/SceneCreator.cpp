#include "Project/Scene/SceneCreator.h"

#include <heap/seadHeapMgr.h>

#include "Library/Memory/HeapUtil.h"
#include "Library/Scene/Scene.h"
#include "Library/Scene/SceneFactory.h"
#include "Library/Thread/AsyncFunctorThread.h"
#include "Library/Thread/FunctorV1M.h"
#include "Project/Memory/SceneHeapSetter.h"
#include "Project/Scene/SceneInitInfo.h"

namespace al {
using InitFunctor = FunctorV1M<Scene*, void (Scene::*)(const SceneInitInfo&), const SceneInitInfo&>;

SceneCreator::SceneCreator(const GameSystemInfo* gameSystemInfo, GameDataHolderBase* gameDataHolder,
                           ScreenCaptureExecutor* screenCaptureExecutor,
                           alSceneFunction::SceneFactory* sceneFactory,
                           AudioDirector* audioDirector)
    : mGameSystemInfo(gameSystemInfo), mGameDataHolder(gameDataHolder),
      mScreenCaptureExecutor(screenCaptureExecutor), mSceneFactory(sceneFactory),
      mAudioDirector(audioDirector) {}

Scene* SceneCreator::createScene(const char* sceneFactoryEntry, const char* stageName,
                                 s32 scenarioNo, const char* sceneName,
                                 bool initializeOnSeparateThread, s32 threadPriority) {
    createSceneHeap(stageName, false);
    SceneHeapSetter heapSetter;
    alSceneFunction::SceneCreatorFunction func = nullptr;
    mSceneFactory->getEntryIndex(&func, sceneFactoryEntry);
    Scene* scene = func();
    SceneInitInfo* initInfo =
        new SceneInitInfo(mGameSystemInfo, mGameDataHolder, mScreenCaptureExecutor, stageName,
                          scenarioNo, sceneName, mAudioDirector);
    if (initializeOnSeparateThread)
        mInitializeThread =
            createAndStartInitializeThread(heapSetter.getSceneHeap(), threadPriority,
                                           InitFunctor(scene, &Scene::initializeAsync, *initInfo));
    else
        scene->init(*initInfo);
    return scene;
}

void SceneCreator::setSceneAndThreadInit(Scene* scene, const char* stageName, s32 scenarioNo,
                                         const char* sceneName, s32 threadPriority,
                                         sead::Heap* heap) {
    if (heap) {
        sead::ScopedCurrentHeapSetter heapSetter(heap);
        SceneInitInfo* initInfo =
            new SceneInitInfo(mGameSystemInfo, mGameDataHolder, mScreenCaptureExecutor, stageName,
                              scenarioNo, sceneName, mAudioDirector);
        mInitializeThread = createAndStartInitializeThread(
            heap, threadPriority, InitFunctor(scene, &Scene::initializeAsync, *initInfo));
    } else {
        SceneHeapSetter heapSetter;
        SceneInitInfo* initInfo =
            new SceneInitInfo(mGameSystemInfo, mGameDataHolder, mScreenCaptureExecutor, stageName,
                              scenarioNo, sceneName, mAudioDirector);
        mInitializeThread =
            createAndStartInitializeThread(heapSetter.getSceneHeap(), threadPriority,
                                           InitFunctor(scene, &Scene::initializeAsync, *initInfo));
    }
}

void SceneCreator::setSceneAndInit(Scene* scene, const char* stageName, s32 scenarioNo,
                                   const char* sceneName) {
    SceneHeapSetter heapSetter;
    SceneInitInfo* initInfo =
        new SceneInitInfo(mGameSystemInfo, mGameDataHolder, mScreenCaptureExecutor, stageName,
                          scenarioNo, sceneName, mAudioDirector);
    scene->init(*initInfo);
}

bool SceneCreator::tryEndInitThread() {
    if (!isExistInitThread())
        return true;

    if (tryWaitDoneAndDestroyInitializeThread(mInitializeThread)) {
        mInitializeThread = nullptr;
        return true;
    }

    return false;
}

bool SceneCreator::isExistInitThread() const {
    return mInitializeThread;
}
}  // namespace al
