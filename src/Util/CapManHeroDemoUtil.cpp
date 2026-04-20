#include "Util/CapManHeroDemoUtil.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Scene/Scene.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Demo/CapManHeroDemoDirector.h"
#include "Demo/StageTalkDemoNpcCap.h"
#include "Npc/CapWorldNpcTailJointController.h"
#include "Util/PlayerDemoUtil.h"

namespace CapManHeroDemoUtil {
void initCapManHeroDemoDirector(const al::Scene* scene, const al::ActorInitInfo& initInfo) {
    al::getSceneObj<CapManHeroDemoDirector>(scene)->init(initInfo);
}

void initCapManHeroTailJointController(al::LiveActor* capWorldNpc) {
    rs::initCapWorldNpcTailJointController(capWorldNpc);
}

void startCapManHeroCommonSettingAfterShowModel(al::LiveActor* actor) {
    CapManHeroDemoFunction::startCapManHeroCommonSettingAfterShowModel(actor);
}

al::LiveActor* createDemoCapManHero(const char* name, const al::ActorInitInfo& initInfo,
                                    const char* suffix) {
    return CapManHeroDemoFunction::createDemoCapManHero(name, initInfo, suffix);
}

void capManHeroControl(al::LiveActor* actor) {
    CapManHeroDemoFunction::capManHeroControl(actor);
}

void stopTailScroll(al::LiveActor* actor) {
    al::setMatAnimFrameRate(actor, 0.0f);
}

void restartTailScroll(al::LiveActor* actor) {
    al::setMatAnimFrameRate(actor, 1.0f);
}

al::LiveActor* getCapManHero(const al::IUseSceneObjHolder* sceneObjHolder) {
    return al::getSceneObj<CapManHeroDemoDirector>(sceneObjHolder)->getCapManHero();
}

void setTalkDemoFirstMoonGet(StageTalkDemoNpcCap* talkDemoActor) {
    al::getSceneObj<CapManHeroDemoDirector>(talkDemoActor)->setTalkDemoFirstMoonGet(talkDemoActor);
}

void setTalkDemoStageStart(StageTalkDemoNpcCap* talkDemoActor) {
    al::getSceneObj<CapManHeroDemoDirector>(talkDemoActor)->setTalkDemoStageStart(talkDemoActor);
}

void setTalkDemoMoonRock(StageTalkDemoNpcCap* talkDemoActor) {
    al::getSceneObj<CapManHeroDemoDirector>(talkDemoActor)->setTalkDemoMoonRock(talkDemoActor);
}

void preEventFromSceneFirstMoonGet(const al::Scene* scene, const char* eventName) {
    al::getSceneObj<CapManHeroDemoDirector>(scene)->preEventFromSceneFirstMoonGet(eventName);
}

void startTalkDemoFirstMoonGet(const al::Scene* scene) {
    al::getSceneObj<CapManHeroDemoDirector>(scene)->startTalkDemoFirstMoonGet();
}

void startTalkDemoStageStart(const al::Scene* scene) {
    al::getSceneObj<CapManHeroDemoDirector>(scene)->startTalkDemoStageStart();
}

void startTalkDemoMoonRockFind(const al::Scene* scene) {
    al::getSceneObj<CapManHeroDemoDirector>(scene)->startTalkDemoMoonRockFind();
}

void startTalkDemoAfterMoonRockBreakDemo(const al::Scene* scene) {
    al::getSceneObj<CapManHeroDemoDirector>(scene)->startTalkDemoAfterMoonRockBreakDemo();
}

bool isExistTalkDemoStageStart(const al::Scene* scene) {
    return al::getSceneObj<CapManHeroDemoDirector>(scene)->isExistTalkDemoStageStart();
}

bool isExistTalkDemoMoonRockFind(const al::Scene* scene) {
    return al::getSceneObj<CapManHeroDemoDirector>(scene)->isExistTalkDemoMoonRockFind();
}

bool isExistTalkDemoAfterMoonRockBreakDemo(const al::Scene* scene) {
    return al::getSceneObj<CapManHeroDemoDirector>(scene)->isExistTalkDemoAfterMoonRockBreakDemo();
}

bool isEndDemo(const al::Scene* scene) {
    return al::getSceneObj<CapManHeroDemoDirector>(scene)->isEndDemo();
}

void tryCloseDemoFadeIfExistStageStartTalk(const al::LiveActor* actor) {}

void startActionCapManHero(al::LiveActor* actor, const char* actionName) {
    al::startAction(actor, actionName);
}

f32 calcAngleDemoPlayerToTargetH(const al::LiveActor* actor, const sead::Vector3f& targetTrans) {
    sead::Vector3f targetDir;
    targetDir.setSub(targetTrans, rs::getDemoPlayerTrans(actor));
    targetDir.y = 0.0f;
    al::normalize(&targetDir);

    sead::Quatf demoPlayerQuat = rs::getDemoPlayerQuat(actor);
    sead::Vector3f demoPlayerFront;
    al::calcQuatFront(&demoPlayerFront, demoPlayerQuat);
    return al::calcAngleOnPlaneDegree(demoPlayerFront, targetDir, sead::Vector3f::ey);
}

bool turnDemoPlayerToTargetH(const al::LiveActor* actor, const sead::Vector3f& targetTrans,
                             f32 turnDegree) {
    sead::Vector3f targetDir;
    targetDir.setSub(targetTrans, rs::getDemoPlayerTrans(actor));
    targetDir.y = 0.0f;
    al::normalize(&targetDir);

    sead::Quatf demoPlayerQuat = rs::getDemoPlayerQuat(actor);
    bool isTurned = al::turnQuatFrontToDirDegreeH(&demoPlayerQuat, targetDir, turnDegree);
    rs::setDemoPlayerQuat(actor, demoPlayerQuat);
    return isTurned;
}

bool isEnablePlayerTurnNeck(al::LiveActor* actor, f32 angle) {
    return sead::Mathf::abs(angle) <= 90.0f;
}

bool tryPlayerTurnNeck(al::LiveActor* actor, const sead::Vector3f& targetTrans) {
    f32 angle = calcAngleDemoPlayerToTargetH(actor, targetTrans);

    if (isEnablePlayerTurnNeck(actor, angle)) {
        rs::validateWatchTarget(actor, targetTrans);
        return true;
    }

    return false;
}

void invalidateDitherAnimIfExist(al::LiveActor* actor) {
    if (al::isExistDitherAnimator(actor))
        al::invalidateDitherAnim(actor);
}
}  // namespace CapManHeroDemoUtil

namespace CapManHeroDemoFunction {
void setDemoStartInfo(const al::IUseSceneObjHolder* sceneObjHolder,
                      const sead::Quatf& demoStartInfo) {
    al::getSceneObj<CapManHeroDemoDirector>(sceneObjHolder)->setDemoStartInfo(demoStartInfo);
}

void getDemoStartInfo(const al::IUseSceneObjHolder* sceneObjHolder, sead::Quatf* demoStartInfo) {
    sead::Quatf demoStartInfoRef =
        al::getSceneObj<CapManHeroDemoDirector>(sceneObjHolder)->getDemoStartInfo();

    *demoStartInfo = demoStartInfoRef;
}
}  // namespace CapManHeroDemoFunction
