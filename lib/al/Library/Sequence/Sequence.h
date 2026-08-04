#pragma once

#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Sequence/IUseSceneCreator.h"

namespace sead {
class Heap;
}  // namespace sead

namespace alSceneFunction {
class SceneFactory;
}  // namespace alSceneFunction

namespace al {
struct GameSystemInfo;
struct DrawSystemInfo;
struct SequenceInitInfo;
struct AudioSystemInfo;
struct AudioDirectorInitInfo;
class GameDataHolderBase;
class AudioDirector;
class Scene;
class ScreenCaptureExecutor;

class Sequence : public NerveExecutor, public IUseAudioKeeper, public IUseSceneCreator {
public:
    Sequence(const char* name);
    ~Sequence() override;

    virtual void init(const SequenceInitInfo& initInfo);

    virtual void update();
    virtual void kill();
    virtual void drawMain() const;
    virtual void drawSub() const;

    AudioKeeper* getAudioKeeper() const override { return mAudioKeeper; }

    virtual bool isDisposable() const;

    virtual Scene* getCurrentScene() const { return nullptr; }

    SceneCreator* getSceneCreator() const override { return mSceneCreator; }

    void setSceneCreator(SceneCreator* sceneCreator) override { mSceneCreator = sceneCreator; }

    void initAudio(const GameSystemInfo&, const char*, s32, s32, s32, const char*);
    void initAudioKeeper(const char*);
    void initDrawSystemInfo(const SequenceInitInfo&);
    AudioSystemInfo* getAudioSystemInfo();

    DrawSystemInfo* getDrawInfo() const { return mDrawSystemInfo; }

    AudioDirector* getAudioDirector() const { return mAudioDirector; }

    void setNextScene(Scene* scene) { mNextScene = scene; }

private:
    sead::FixedSafeString<0x40> mName;
    Scene* mCurrentScene = nullptr;
    Scene* mNextScene = nullptr;
    SceneCreator* mSceneCreator = nullptr;
    AudioDirector* mAudioDirector = nullptr;
    AudioKeeper* mAudioKeeper = nullptr;
    DrawSystemInfo* mDrawSystemInfo = nullptr;
    bool mIsAlive = true;
};

void initSceneCreator(IUseSceneCreator* sceneCreator, const SequenceInitInfo& initInfo,
                      GameDataHolderBase* gameDataHolder, AudioDirector* audioDirector,
                      ScreenCaptureExecutor* screenCaptureExecutor,
                      alSceneFunction::SceneFactory* sceneFactory);
Scene* createSceneAndInit(IUseSceneCreator* sceneCreator, const char* stageName,
                          const char* sceneName, s32 scenarioNo, const char* sequenceParam);
void createSceneAndUseInitThread(IUseSceneCreator* sceneCreator, const char* stageName,
                                 s32 priority, const char* sceneName, s32 scenarioNo,
                                 const char* sequenceParam);
void setSceneAndInit(IUseSceneCreator* sceneCreator, Scene* scene, const char* stageName,
                     s32 scenarioNo, const char* sequenceParam);
void setSceneAndUseInitThread(IUseSceneCreator* sceneCreator, Scene* scene, s32 priority,
                              const char* stageName, s32 scenarioNo, const char* sequenceParam,
                              sead::Heap* heap);
bool tryEndSceneInitThread(IUseSceneCreator* sceneCreator);
bool isExistSceneInitThread(const IUseSceneCreator* sceneCreator);
void initAudioDirector(Sequence* sequence, AudioSystemInfo* audioSystemInfo,
                       AudioDirectorInitInfo& initInfo);
void setSequenceAudioKeeperToSceneSeDirector(Sequence* sequence, Scene* scene);
void setSequenceNameForActorPickTool(Sequence* sequence, Scene* scene);
}  // namespace al
