#pragma once

#include <prim/seadSafeString.h>

#include "al/Library/Audio/IUseAudioKeeper.h"
#include "al/Library/Nerve/NerveExecutor.h"
#include "al/Library/Sequence/IUseSceneCreator.h"
#include "al/Library/System/GameSystemInfo.h"

namespace al {
struct GameSystemInfo;
class SequenceInitInfo;
class AudioSystemInfo;
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
    virtual bool isDisposable() { return false; }
    virtual Scene* getCurrentScene() const;
    virtual SceneCreator* getSceneCreator() const override;
    virtual void setSceneCreator(SceneCreator* sceneCreator) override;

    AudioKeeper* getAudioKeeper() const override;
    void initAudio(const GameSystemInfo&, const char*, int, int, int, const char*);
    void initAudioKeeper(const char*);
    void initDrawSystemInfo(const SequenceInitInfo&);
    AudioSystemInfo* getAudioSystemInfo();

    GameDrawInfo* getDrawInfo() const { return mGameDrawInfo; }

private:
    sead::FixedSafeString<0x40> mName;
    Scene* mNextScene;
    Scene* mCurrentScene;
    SceneCreator* mSceneCreator;
    AudioDirector* mAudioDirector;
    AudioKeeper* mAudioKeeper;
    GameDrawInfo* mGameDrawInfo;
    bool mIsAlive;
};
}  // namespace al
