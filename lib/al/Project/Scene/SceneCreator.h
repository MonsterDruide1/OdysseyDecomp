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
class Scene;

class SceneCreator {
public:
    SceneCreator(const GameSystemInfo* gameSystemInfo, GameDataHolderBase* gameDataHolder,
                 ScreenCaptureExecutor* screenCaptureExecutor,
                 alSceneFunction::SceneFactory* sceneFactory, AudioDirector* audioDirector);
    Scene* createScene(const char*, const char*, s32, const char*, bool, s32);
    void setSceneAndThreadInit(Scene*, const char*, s32, const char*, s32, sead::Heap*);
    void setSceneAndInit(Scene*, const char*, s32, const char*);
    bool tryEndInitThread();
    bool isExistInitThread() const;

private:
    const GameSystemInfo* mGameSystemInfo;
    GameDataHolderBase* mGameDataHolder;
    ScreenCaptureExecutor* mScreenCaptureExecutor;
    alSceneFunction::SceneFactory* mSceneFactory;
    InitializeThread* mInitializeThread = nullptr;
    AudioDirector* mAudioDirector;
};

}  // namespace al
