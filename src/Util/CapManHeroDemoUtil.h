#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
class LiveActor;
class Scene;
}  // namespace al

class StageTalkDemoNpcCap;

namespace CapManHeroDemoUtil {
void initCapManHeroDemoDirector(const al::Scene*, const al::ActorInitInfo&);
void initCapManHeroTailJointController(al::LiveActor*);
void startCapManHeroCommonSettingAfterShowModel(al::LiveActor*);
al::LiveActor* createDemoCapManHero(const char*, const al::ActorInitInfo&, const char*);
void capManHeroControl(al::LiveActor*);
void stopTailScroll(al::LiveActor*);
void restartTailScroll(al::LiveActor*);
al::LiveActor* getCapManHero(const al::IUseSceneObjHolder*);
void setTalkDemoFirstMoonGet(StageTalkDemoNpcCap*);
void setTalkDemoStageStart(StageTalkDemoNpcCap*);
void setTalkDemoMoonRock(StageTalkDemoNpcCap*);
void preEventFromSceneFirstMoonGet(const al::Scene*, const char*);
void startTalkDemoFirstMoonGet(const al::Scene*);
void startTalkDemoStageStart(const al::Scene*);
void startTalkDemoMoonRockFind(const al::Scene*);
void startTalkDemoAfterMoonRockBreakDemo(const al::Scene*);
bool isExistTalkDemoStageStart(const al::Scene*);
bool isExistTalkDemoMoonRockFind(const al::Scene*);
bool isExistTalkDemoAfterMoonRockBreakDemo(const al::Scene*);
bool isEndDemo(const al::Scene*);
bool tryCloseDemoFadeIfExistStageStartTalk(const al::LiveActor*);
void startActionCapManHero(al::LiveActor*, const char*);
f32 calcAngleDemoPlayerToTargetH(const al::LiveActor*, const sead::Vector3f&);
void turnDemoPlayerToTargetH(const al::LiveActor*, const sead::Vector3f&, f32);
bool isEnablePlayerTurnNeck(al::LiveActor*, f32);
void tryPlayerTurnNeck(al::LiveActor*, const sead::Vector3f&);
void invalidateDitherAnimIfExist(al::LiveActor*);
}  // namespace CapManHeroDemoUtil
