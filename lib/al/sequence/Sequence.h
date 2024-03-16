#pragma once

#include <prim/seadSafeString.h>
#include "al/HakoniwaSequence/HakoniwaSequence.h"
#include "al/audio/AudioDirector.h"
#include "al/iuse/IUseAudioKeeper.h"
#include "al/iuse/IUseSceneCreator.h"
#include "al/nerve/NerveExecutor.h"

namespace al {
class Scene;
class SceneCreator;
class AudioDirector;
class DrawSystemInfo;
class SequenceInitInfo;
class GameSystemInfo;

class Sequence : NerveExecutor, IUseAudioKeeper, IUseSceneCreator {
public:
    Sequence(const char* name);
    ~Sequence() override;

    AudioKeeper* getAudioKeeper() const override;
    SceneCreator* getSceneCreator() const override;
    void setSceneCreator(SceneCreator* creator) override;

    virtual void init(const al::SequenceInitInfo& initInfo);
    virtual void update();
    virtual void kill();
    virtual void drawMain() const;
    virtual void drawSub() const;
    virtual bool isDisposable() const;
    virtual Scene* getCurrentScene() const;

    void initAudio(const GameSystemInfo& info, const char*, int, int, int, const char*);
    void initAudioKeeper(const char*);
    void initDrawSystemInfo(const SequenceInitInfo& info);
    const AudioSystemInfo& getAudioSystemInfo() const;

private:
    sead::FixedSafeString<64> mName;
    Scene* mCurrentScene = nullptr;
    Scene* mNextScene = nullptr;
    SceneCreator* mSceneCreator = nullptr;
    AudioDirector* mAudioDirector = nullptr;
    AudioKeeper* mAudioKeeper = nullptr;
    DrawSystemInfo* mDrawSystemInfo = nullptr;
    bool isAlive = true;
};

}  // namespace al
