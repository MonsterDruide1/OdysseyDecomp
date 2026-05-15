#include "Demo/CapManHeroDemoDirector.h"

#include <prim/seadSafeString.h>

#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActor.h"

#include "Demo/CapManHeroBase.h"
#include "Demo/StageTalkDemoNpcCap.h"
#include "Util/NpcAnimUtil.h"

namespace CapManHeroDemoFunction {

void capManHeroControl(al::LiveActor* actor) {}

al::LiveActor* createDemoCapManHero(const char* actorName, const al::ActorInitInfo& info,
                                    const char* suffix) {
    al::LiveActor* capManHero = new CapManHeroBase(actorName);
    al::initChildActorWithArchiveNameNoPlacementInfo(capManHero, info, "CapManHero", suffix);
    al::initJointControllerKeeper(capManHero, 1);
    rs::initCapWorldNpcTailJointController(capManHero);
    al::invalidateClipping(capManHero);
    capManHero->makeActorDead();
    return capManHero;
}

void startCapManHeroCommonSettingAfterShowModel(al::LiveActor* actor) {}

}  // namespace CapManHeroDemoFunction

CapManHeroDemoDirector::CapManHeroDemoDirector() = default;

void CapManHeroDemoDirector::init(const al::ActorInitInfo& info) {
    al::LiveActor* capManHero = new CapManHeroBase("デモ用キャップ[Director]");
    al::initChildActorWithArchiveNameNoPlacementInfo(capManHero, info, "CapManHero", "Demo");
    al::initJointControllerKeeper(capManHero, 1);
    rs::initCapWorldNpcTailJointController(capManHero);
    al::invalidateClipping(capManHero);
    capManHero->makeActorDead();
    mCapManHero = capManHero;
}

bool CapManHeroDemoDirector::isEndDemo() const {
    return mCurrentDemo->isEndDemo();
}

bool CapManHeroDemoDirector::isExistTalkDemoStageStart() const {
    if (mTalkDemoStageStart)
        return mTalkDemoStageStart->isEnableStageStartDemo();
    return false;
}

bool CapManHeroDemoDirector::isExistTalkDemoMoonRockFind() const {
    if (mTalkDemoMoonRock)
        return mTalkDemoMoonRock->isEnableStartMoonRockFindDemo();
    return false;
}

bool CapManHeroDemoDirector::isExistTalkDemoAfterMoonRockBreakDemo() const {
    if (mTalkDemoMoonRock)
        return mTalkDemoMoonRock->isEnableStartAfterBreakMoonRockDemo();
    return false;
}

void CapManHeroDemoDirector::preEventFromSceneFirstMoonGet(const char* eventName) {
    if (mTalkDemoFirstMoonGet)
        mTalkDemoFirstMoonGet->preEventFromSceneFirstMoonGet(eventName);
}

void CapManHeroDemoDirector::startTalkDemoFirstMoonGet() {
    mCurrentDemo = mTalkDemoFirstMoonGet;
    mCurrentDemo->startDemoFromScene();
}

void CapManHeroDemoDirector::startTalkDemoCore(StageTalkDemoNpcCap* talkDemo) {
    mCurrentDemo = talkDemo;
    talkDemo->startDemoFromScene();
}

void CapManHeroDemoDirector::startTalkDemoStageStart() {
    mCurrentDemo = mTalkDemoStageStart;
    mCurrentDemo->startDemoFromScene();
}

void CapManHeroDemoDirector::startTalkDemoMoonRockFind() {
    mCurrentDemo = mTalkDemoMoonRock;
    mCurrentDemo->startDemoFromScene();
}

void CapManHeroDemoDirector::startTalkDemoAfterMoonRockBreakDemo() {
    mCurrentDemo = mTalkDemoMoonRock;
    mCurrentDemo->startDemoFromScene();
}
