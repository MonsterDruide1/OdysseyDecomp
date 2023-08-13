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
private:
    GameSystemInfo* mGameSystemInfo;
    GameDataHolderBase* mGameDataHolder;
    ScreenCaptureExecutor* mScreenCaptureExecutor;
    alSceneFunction::SceneFactory* mSceneFactory;
    InitializeThread* mInitializeThread;
    AudioDirector* mAudioDirector;

public:
    SceneCreator(const GameSystemInfo*, GameDataHolderBase*, ScreenCaptureExecutor*,
                 alSceneFunction::SceneFactory*, AudioDirector*);
    void createScene(const char*, const char*, s32, const char*, bool, s32);
    void setSceneAndThreadInit(Scene*, const char*, s32, const char*, s32, sead::Heap*);
    void setSceneAndInit(Scene*, const char*, s32, const char*);
    bool tryEndInitThread();
    bool isExistInitThread();
};

}  // namespace al
