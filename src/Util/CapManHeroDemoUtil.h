#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
class LiveActor;
class Scene;
}  // namespace al
class StageTalkDemoNpcCap;

namespace CapManHeroDemoUtil {
void initCapManHeroDemoDirector(const al::Scene* scene, const al::ActorInitInfo& initInfo);
void initCapManHeroTailJointController(al::LiveActor* actor);
void startCapManHeroCommonSettingAfterShowModel(al::LiveActor* actor);
al::LiveActor* createDemoCapManHero(const char* name, const al::ActorInitInfo& initInfo,
                                    const char* archiveName);
void capManHeroControl(al::LiveActor* actor);
void stopTailScroll(al::LiveActor* actor);
void restartTailScroll(al::LiveActor* actor);
al::LiveActor* getCapManHero(const al::IUseSceneObjHolder* sceneObjHolder);
void setTalkDemoFirstMoonGet(StageTalkDemoNpcCap* talkDemoActor);
void setTalkDemoStageStart(StageTalkDemoNpcCap* talkDemoActor);
void setTalkDemoMoonRock(StageTalkDemoNpcCap* talkDemoActor);
void preEventFromSceneFirstMoonGet(const al::Scene* scene, const char* eventName);
void startTalkDemoFirstMoonGet(const al::Scene* scene);
void startTalkDemoStageStart(const al::Scene* scene);
void startTalkDemoMoonRockFind(const al::Scene* scene);
void startTalkDemoAfterMoonRockBreakDemo(const al::Scene* scene);
bool isExistTalkDemoStageStart(const al::Scene* scene);
bool isExistTalkDemoMoonRockFind(const al::Scene* scene);
bool isExistTalkDemoAfterMoonRockBreakDemo(const al::Scene* scene);
bool isEndDemo(const al::Scene* scene);
void tryCloseDemoFadeIfExistStageStartTalk(const al::LiveActor* actor);
void startActionCapManHero(al::LiveActor* actor, const char* actionName);
f32 calcAngleDemoPlayerToTargetH(const al::LiveActor* actor, const sead::Vector3f& targetTrans);
bool turnDemoPlayerToTargetH(const al::LiveActor* actor, const sead::Vector3f& targetTrans,
                             f32 turnDegree);
bool isEnablePlayerTurnNeck(al::LiveActor* actor, f32 angle);
bool tryPlayerTurnNeck(al::LiveActor* actor, const sead::Vector3f& targetTrans);
void invalidateDitherAnimIfExist(al::LiveActor* actor);
}  // namespace CapManHeroDemoUtil

namespace CapManHeroDemoFunction {
void setDemoStartInfo(const al::IUseSceneObjHolder* sceneObjHolder,
                      const sead::Quatf& demoStartInfo);
void getDemoStartInfo(const al::IUseSceneObjHolder* sceneObjHolder, sead::Quatf* demoStartInfo);
}  // namespace CapManHeroDemoFunction
