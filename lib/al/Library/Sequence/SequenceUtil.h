#pragma once

#include <heap/seadHeap.h>

namespace alSceneFunction {
class SceneFactory;
}

namespace al {
class IUseSceneCreator;
struct SequenceInitInfo;
class GameDataHolderBase;
class AudioDirector;
class ScreenCaptureExecutor;
class Scene;
class Sequence;
struct AudioSystemInfo;
struct AudioDirectorInitInfo;

void initSceneCreator(IUseSceneCreator* user, const SequenceInitInfo& initInfo,
                      GameDataHolderBase* gameDataHolder, AudioDirector* audioDirector,
                      ScreenCaptureExecutor* screenCaptureExecutor,
                      alSceneFunction::SceneFactory* sceneFactory);
void createSceneAndInit(IUseSceneCreator* user, const char* sceneFactoryEntry,
                        const char* stageName, s32 scenarioNo, const char* sceneName);
void createSceneAndUseInitThread(IUseSceneCreator* user, const char* sceneFactoryEntry,
                                 s32 threadPriority, const char* stageName, s32 scenarioNo,
                                 const char* sceneName);
void setSceneAndInit(IUseSceneCreator* user, Scene* scene, const char* stageName, s32 scenarioNo,
                     const char* sceneName);
void setSceneAndUseInitThread(IUseSceneCreator* user, Scene* scene, s32 threadPriority,
                              const char* stageName, s32 scenarioNo, const char* sceneName,
                              sead::Heap* heap);
bool tryEndSceneInitThread(IUseSceneCreator* user);
bool isExistSceneInitThread(const IUseSceneCreator* user);
void initAudioDirector(Sequence* sequence, AudioSystemInfo* audioSystemInfo,
                       AudioDirectorInitInfo& initInfo);
void setSequenceAudioKeeperToSceneSeDirector(Sequence* sequence, Scene* scene);
void setSequenceNameForActorPickTool(Sequence* sequence, Scene* scene);
}  // namespace al
