#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
class LiveActor;
}  // namespace al

class StageTalkDemoNpcCap;

class CapManHeroDemoDirector : public al::HioNode, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_CapManHeroDemoDirector;

    CapManHeroDemoDirector();

    const char* getSceneObjName() const override { return "キャップ会話ディレクター"; }

    void init(const al::ActorInitInfo& info);
    bool isEndDemo() const;
    bool isExistTalkDemoStageStart() const;
    bool isExistTalkDemoMoonRockFind() const;
    bool isExistTalkDemoAfterMoonRockBreakDemo() const;
    void preEventFromSceneFirstMoonGet(const char* eventName);
    void startTalkDemoFirstMoonGet();
    void startTalkDemoCore(StageTalkDemoNpcCap* talkDemo);
    void startTalkDemoStageStart();
    void startTalkDemoMoonRockFind();
    void startTalkDemoAfterMoonRockBreakDemo();

    al::LiveActor* getCapManHero() const { return mCapManHero; }

    void setTalkDemoFirstMoonGet(StageTalkDemoNpcCap* talkDemo) {
        mTalkDemoFirstMoonGet = talkDemo;
    }

    void setTalkDemoStageStart(StageTalkDemoNpcCap* talkDemo) { mTalkDemoStageStart = talkDemo; }

    void setTalkDemoMoonRock(StageTalkDemoNpcCap* talkDemo) { mTalkDemoMoonRock = talkDemo; }

    void setDemoStartInfo(const sead::Quatf& quat) { mDemoStartQuat = quat; }

    void getDemoStartInfo(sead::Quatf* quat) const { *quat = mDemoStartQuat; }

private:
    al::LiveActor* mCapManHero = nullptr;
    StageTalkDemoNpcCap* mCurrentDemo = nullptr;
    StageTalkDemoNpcCap* mTalkDemoFirstMoonGet = nullptr;
    StageTalkDemoNpcCap* mTalkDemoStageStart = nullptr;
    StageTalkDemoNpcCap* mTalkDemoMoonRock = nullptr;
    sead::Quatf mDemoStartQuat;
};

static_assert(sizeof(CapManHeroDemoDirector) == 0x40);

namespace CapManHeroDemoFunction {
void capManHeroControl(al::LiveActor* actor);
al::LiveActor* createDemoCapManHero(const char* actorName, const al::ActorInitInfo& info,
                                    const char* suffix);
void startCapManHeroCommonSettingAfterShowModel(al::LiveActor* actor);
void setDemoStartInfo(const al::IUseSceneObjHolder* holder, const sead::Quatf& quat);
void getDemoStartInfo(const al::IUseSceneObjHolder* holder, sead::Quatf* quat);
}  // namespace CapManHeroDemoFunction
