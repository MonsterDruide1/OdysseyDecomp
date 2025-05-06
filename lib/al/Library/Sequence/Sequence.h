#pragma once

#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Sequence/IUseSceneCreator.h"

namespace al {
struct GameSystemInfo;
struct DrawSystemInfo;
struct SequenceInitInfo;
struct AudioSystemInfo;
class AudioDirector;
class Scene;

class Sequence : public NerveExecutor, public IUseAudioKeeper, public IUseSceneCreator {
public:
    Sequence(const char* name);
    virtual ~Sequence() override;

    virtual void init(const SequenceInitInfo& initInfo);

    virtual void update();
    virtual void kill();
    virtual void drawMain() const;
    virtual void drawSub() const;

    virtual bool isDisposable() const { return true; }

    virtual Scene* getCurrentScene() const { return nullptr; }

    virtual SceneCreator* getSceneCreator() const override { return mSceneCreator; }

    virtual void setSceneCreator(SceneCreator* sceneCreator) override {
        mSceneCreator = sceneCreator;
    }

    AudioKeeper* getAudioKeeper() const override { return mAudioKeeper; }

    void initAudio(const GameSystemInfo&, const char*, s32, s32, s32, const char*);
    void initAudioKeeper(const char*);
    void initDrawSystemInfo(const SequenceInitInfo&);
    AudioSystemInfo* getAudioSystemInfo();

    DrawSystemInfo* getDrawInfo() const { return mDrawSystemInfo; }

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
