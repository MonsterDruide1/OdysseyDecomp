#pragma once

#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Sequence/IUseSceneCreator.h"
#include "Library/System/GameSystemInfo.h"

namespace al {
struct GameSystemInfo;
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
    virtual bool isDisposable() { return false; }
    virtual Scene* getCurrentScene() const;
    virtual SceneCreator* getSceneCreator() const override;
    virtual void setSceneCreator(SceneCreator* sceneCreator) override;

    AudioKeeper* getAudioKeeper() const override;
    void initAudio(const GameSystemInfo&, const char*, s32, s32, s32, const char*);
    void initAudioKeeper(const char*);
    void initDrawSystemInfo(const SequenceInitInfo&);
    AudioSystemInfo* getAudioSystemInfo();

    DrawSystemInfo* getDrawInfo() const { return mDrawSystemInfo; }

private:
    sead::FixedSafeString<0x40> mName;
    Scene* mNextScene;
    Scene* mCurrentScene;
    SceneCreator* mSceneCreator;
    AudioDirector* mAudioDirector;
    AudioKeeper* mAudioKeeper;
    DrawSystemInfo* mDrawSystemInfo;
    bool mIsAlive;
};
}  // namespace al
