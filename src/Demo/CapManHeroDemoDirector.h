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

struct CapManHeroDemoStartInfo {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

static_assert(sizeof(CapManHeroDemoStartInfo) == 0x10);
static_assert(sizeof(CapManHeroDemoStartInfo) == sizeof(sead::Quatf));

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

    void setDemoStartInfo(const sead::Quatf& demoStartInfo) {
        mDemoStartInfo.x = demoStartInfo.x;
        mDemoStartInfo.y = demoStartInfo.y;
        mDemoStartInfo.z = demoStartInfo.z;
        mDemoStartInfo.w = demoStartInfo.w;
    }

    const CapManHeroDemoStartInfo& getDemoStartInfo() const { return mDemoStartInfo; }

private:
    al::LiveActor* mCapManHero = nullptr;
    StageTalkDemoNpcCap* mCurrentTalkDemo = nullptr;
    StageTalkDemoNpcCap* mTalkDemoFirstMoonGet = nullptr;
    StageTalkDemoNpcCap* mTalkDemoStageStart = nullptr;
    StageTalkDemoNpcCap* mTalkDemoMoonRock = nullptr;
    CapManHeroDemoStartInfo mDemoStartInfo;
};

static_assert(sizeof(CapManHeroDemoDirector) == 0x40);

namespace CapManHeroDemoFunction {
void capManHeroControl(al::LiveActor*);
al::LiveActor* createDemoCapManHero(const char*, const al::ActorInitInfo&, const char*);
void startCapManHeroCommonSettingAfterShowModel(al::LiveActor*);
}  // namespace CapManHeroDemoFunction
