#pragma once

#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Sequence/IUseSceneCreator.h"

namespace al {
struct GameSystemInfo;
struct DrawSystemInfo;
struct AudioSystemInfo;
class AudioDirector;
class Scene;

struct SequenceInitInfo {
    SequenceInitInfo(const al::GameSystemInfo* info);

    const al::GameSystemInfo* systemInfo;
};

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

    const sead::FixedSafeString<0x40>& getName() const { return mName; }

    DrawSystemInfo* getDrawInfo() const { return mDrawSystemInfo; }

    bool isAlive() const { return mIsAlive; }

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
}  // namespace al
