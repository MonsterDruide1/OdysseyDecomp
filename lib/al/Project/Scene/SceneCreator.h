#pragma once

namespace sead {
class Heap;
}  // namespace sead

namespace alSceneFunction {
class SceneFactory;
}

namespace al {
class AudioDirector;
class GameDataHolderBase;
class InitializeThread;
class Scene;
class ScreenCaptureExecutor;
struct GameSystemInfo;

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

}  // namespace al
