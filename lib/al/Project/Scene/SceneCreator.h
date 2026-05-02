#pragma once

#include <heap/seadHeap.h>

namespace alSceneFunction {
class SceneFactory;
}

namespace al {
struct GameSystemInfo;
class GameDataHolderBase;
class ScreenCaptureExecutor;
class InitializeThread;
class AudioDirector;
class IUseSceneCreator;
class Sequence;
class Scene;

class SceneCreator {
public:
    SceneCreator(const GameSystemInfo*, GameDataHolderBase*, ScreenCaptureExecutor*,
                 alSceneFunction::SceneFactory*, AudioDirector*);
    void createScene(const char*, const char*, s32, const char*, bool, s32);
    void setSceneAndThreadInit(Scene*, const char*, s32, const char*, s32, sead::Heap*);
    void setSceneAndInit(Scene*, const char*, s32, const char*);
    bool tryEndInitThread();
    bool isExistInitThread();

private:
    GameSystemInfo* mGameSystemInfo;
    GameDataHolderBase* mGameDataHolder;
    ScreenCaptureExecutor* mScreenCaptureExecutor;
    alSceneFunction::SceneFactory* mSceneFactory;
    InitializeThread* mInitializeThread;
    AudioDirector* mAudioDirector;
};

void setSceneAndUseInitThread(IUseSceneCreator* sceneCreatorUser, Scene* scene, s32 priority,
                              const char* stageName, s32 scenarioNo, const char* sequenceName,
                              sead::Heap* heap);
bool tryEndSceneInitThread(IUseSceneCreator* sceneCreatorUser);
void setSequenceAudioKeeperToSceneSeDirector(Sequence* sequence, Scene* scene);
void setSequenceNameForActorPickTool(Sequence* sequence, Scene* scene);

}  // namespace al
