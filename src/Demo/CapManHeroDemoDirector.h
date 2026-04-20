#pragma once

#include <math/seadQuat.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al
class StageTalkDemoNpcCap;

class CapManHeroDemoDirector : public al::HioNode, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_CapManHeroDemoDirector;

    CapManHeroDemoDirector();

    const char* getSceneObjName() const override;
    ~CapManHeroDemoDirector() override;

    void init(const al::ActorInitInfo&);
    bool isEndDemo() const;
    bool isExistTalkDemoStageStart() const;
    bool isExistTalkDemoMoonRockFind() const;
    bool isExistTalkDemoAfterMoonRockBreakDemo() const;
    void preEventFromSceneFirstMoonGet(const char*);
    void startTalkDemoFirstMoonGet();
    void startTalkDemoCore(StageTalkDemoNpcCap*);
    void startTalkDemoStageStart();
    void startTalkDemoMoonRockFind();
    void startTalkDemoAfterMoonRockBreakDemo();

    al::LiveActor* getCapManHero() const { return mCapManHero; }

    void setTalkDemoFirstMoonGet(StageTalkDemoNpcCap* talkDemoActor) {
        mTalkDemoFirstMoonGet = talkDemoActor;
    }

    void setTalkDemoStageStart(StageTalkDemoNpcCap* talkDemoActor) {
        mTalkDemoStageStart = talkDemoActor;
    }

    void setTalkDemoMoonRock(StageTalkDemoNpcCap* talkDemoActor) {
        mTalkDemoMoonRock = talkDemoActor;
    }

    void setDemoStartInfo(const sead::Quatf& demoStartInfo) { mDemoStartInfo = demoStartInfo; }

    const sead::Quatf& getDemoStartInfo() const { return mDemoStartInfo; }

private:
    al::LiveActor* mCapManHero = nullptr;
    StageTalkDemoNpcCap* mCurrentTalkDemo = nullptr;
    StageTalkDemoNpcCap* mTalkDemoFirstMoonGet = nullptr;
    StageTalkDemoNpcCap* mTalkDemoStageStart = nullptr;
    StageTalkDemoNpcCap* mTalkDemoMoonRock = nullptr;
    sead::Quatf mDemoStartInfo;
};

static_assert(sizeof(CapManHeroDemoDirector) == 0x40);

namespace CapManHeroDemoFunction {
void capManHeroControl(al::LiveActor*);
al::LiveActor* createDemoCapManHero(const char*, const al::ActorInitInfo&, const char*);
void startCapManHeroCommonSettingAfterShowModel(al::LiveActor*);
}  // namespace CapManHeroDemoFunction
