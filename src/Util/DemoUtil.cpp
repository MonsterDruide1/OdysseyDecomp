#include "Util/DemoUtil.h"

#include <basis/seadTypes.h>

#include "Library/Base/StringUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/LiveActor/LiveActorKit.h"
#include "Library/Scene/DemoDirector.h"
#include "Library/Scene/Scene.h"

#include "Boss/BarrierField.h"
#include "Demo/IUseDemoSkip.h"
#include "Demo/ProjectDemoDirector.h"
#include "Item/Shine.h"
#include "Layout/CinemaCaption.h"
#include "MapObj/OceanWaveActor.h"
#include "MapObj/RiseMapPartsHolder.h"
#include "MapObj/ShineTowerRocket.h"
#include "Scene/QuestInfoHolder.h"
#include "System/GameDataFile.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolder.h"
#include "System/GameDataHolderAccessor.h"
#include "System/SequenceDemoSkipData.h"
#include "Util/NpcAnimUtil.h"
#include "Util/StageInputFunction.h"

namespace DemoName {
const char* cHackStartFirstTimeFrog = "初回憑依開始デモ[カエル]";
const char* cHackStartFirstTimeKoopa = "初回憑依開始デモ[クッパ]";
const char* cGiantWanderBossBattleStart = "バトル開始デモ(はいかいボス)";
}  // namespace DemoName

namespace {

ProjectDemoDirector* getDirector(const al::Scene* scene) {
    return static_cast<ProjectDemoDirector*>(scene->getLiveActorKit()->getDemoDirector());
}

ProjectDemoDirector* getDirector(const al::LiveActor* actor) {
    return static_cast<ProjectDemoDirector*>(actor->getSceneInfo()->demoDirector);
}

ProjectDemoDirector::DemoType demoTypeNormal() {
    return ProjectDemoDirector::DemoType::Normal;
}

ProjectDemoDirector::DemoType demoTypeNormalWithCinemaFrame() {
    return ProjectDemoDirector::DemoType::NormalWithCinemaFrame;
}

ProjectDemoDirector::DemoType demoTypeWithPlayer() {
    return ProjectDemoDirector::DemoType::WithPlayer;
}

ProjectDemoDirector::DemoType demoTypeWithPlayerCinemaFrame() {
    return ProjectDemoDirector::DemoType::WithPlayerCinemaFrame;
}

ProjectDemoDirector::DemoType demoTypeShineGet() {
    return ProjectDemoDirector::DemoType::ShineGetDemo;
}

ProjectDemoDirector::DemoType demoTypeWithPlayerKeepCarry() {
    return ProjectDemoDirector::DemoType::WithPlayerKeepCarry;
}

ProjectDemoDirector::DemoType demoTypeKeepHackTalk() {
    return ProjectDemoDirector::DemoType::KeepHackTalk;
}

ProjectDemoDirector::DemoType demoTypeSceneStartPlayerWalk() {
    return ProjectDemoDirector::DemoType::SceneStartPlayerWalk;
}

ProjectDemoDirector::DemoType demoTypeWithPlayerKeepBindTalk() {
    return ProjectDemoDirector::DemoType::WithPlayerKeepBindTalk;
}

inline bool requestStartDemoActor(al::LiveActor* actor, bool isSubActor, const char* demoName,
                                  ProjectDemoDirector::DemoType demoType) {
    ProjectDemoDirector* director = getDirector(actor);
    if (!director->requestStartDemo(demoName, demoType))
        return false;

    rs::addDemoActor(actor, isSubActor);

    getDirector(actor)->setRequestActor(actor);

    return true;
}

inline bool requestStartDemoScene(const al::Scene* scene, const char* demoName,
                                  ProjectDemoDirector::DemoType demoType) {
    return getDirector(scene)->requestStartDemo(demoName, demoType);
}

inline void requestEndDemoActor(const al::LiveActor* actor, const char* demoName,
                                ProjectDemoDirector::DemoType demoType) {
    getDirector(actor)->requestEndDemo(demoName, demoType);
}

inline void requestEndDemoScene(const al::Scene* scene, const char* demoName,
                                ProjectDemoDirector::DemoType demoType) {
    getDirector(scene)->requestEndDemo(demoName, demoType);
}

inline bool isActiveDemoScene(const al::Scene* scene, const char* demoName) {
    return getDirector(scene)->isActiveDemo(demoName);
}

inline bool isActiveDemoActor(const al::LiveActor* actor, const char* demoName) {
    return getDirector(actor)->isActiveDemo(demoName);
}
}  // namespace

namespace rs {
al::LiveActor* createDemoPeachWedding(const al::ActorInitInfo& initInfo) {
    al::LiveActor* actor = new al::LiveActor("[デモ用]ピーチウェディング");
    al::initChildActorWithArchiveNameNoPlacementInfo(actor, initInfo, "PeachWedding", "Demo");
    actor->makeActorDead();
    return actor;
}

al::LiveActor* createDemoCapManHeroine(const al::ActorInitInfo& initInfo) {
    al::LiveActor* actor = new al::LiveActor("[デモ用]ティアラ");
    al::initChildActorWithArchiveNameNoPlacementInfo(actor, initInfo, "CapManHeroine", "Demo");
    al::initJointControllerKeeper(actor, 1);
    initCapWorldNpcTail(actor);
    actor->makeActorDead();
    return actor;
}

bool isRandomActionDemoChangeWorld(const al::LiveActor*, const char* actionName) {
    if (actionName == nullptr)
        return false;

    return al::isEqualString(actionName, "DemoWorldMoveForwardNormal") ||
           al::isEqualString(actionName, "DemoWorldMoveForward") ||
           al::isEqualString(actionName, "DemoWorldMoveBackward");
}

bool tryGetDemoChangeWorldRandomActionName(sead::BufferedSafeString* actionNameOut, s32* indexOut,
                                           const al::LiveActor* actor, const char* actionName) {
    if (isRandomActionDemoChangeWorld(actor, actionName) && GameDataFunction::isGameClear(actor)) {
        *indexOut = getRandomActionIndexDemoChangeWorld(actor);
        if (*indexOut != 0)
            actionNameOut->format("%s%d", actionName, *indexOut);
        else
            actionNameOut->format("%s", actionName);
        return true;
    }

    actionNameOut->format(actionName);
    return false;
}

bool requestStartDemoNormal(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "通常デモ", demoTypeNormal());
}

void addDemoActor(al::LiveActor* actor, bool isSubActor) {
    if (isSubActor)
        getDirector(actor)->addDemoActorWithSubActor(actor);
    else
        getDirector(actor)->addDemoActor(actor);
}

void requestEndDemoNormal(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "通常デモ", demoTypeNormal());
}

bool requestStartDemoNormal(const al::Scene* scene) {
    return requestStartDemoScene(scene, "通常デモ", demoTypeNormal());
}

void requestEndDemoNormal(const al::Scene* scene) {
    requestEndDemoScene(scene, "通常デモ", demoTypeNormal());
}

bool requestStartDemoNormalWithCinemaFrame(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "通常デモ(シネマフレームあり)",
                                 demoTypeNormalWithCinemaFrame());
}

void requestEndDemoNormalWithCinemaFrame(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "通常デモ(シネマフレームあり)", demoTypeNormalWithCinemaFrame());
}

bool requestStartDemoWithPlayer(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "プレイヤー含むデモ", demoTypeWithPlayer());
}

void requestEndDemoWithPlayer(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ", demoTypeWithPlayer());
}

bool requestStartDemoWithPlayer(const al::Scene* scene) {
    return requestStartDemoScene(scene, "プレイヤー含むデモ", demoTypeWithPlayer());
}

void requestEndDemoWithPlayer(const al::Scene* scene) {
    requestEndDemoScene(scene, "プレイヤー含むデモ", demoTypeWithPlayer());
}

bool requestStartDemoWithPlayerCinemaFrame(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "プレイヤー含むデモ(シネマフレームあり)",
                                 demoTypeWithPlayerCinemaFrame());
}

void requestEndDemoWithPlayerCinemaFrame(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ(シネマフレームあり)",
                        demoTypeWithPlayerCinemaFrame());
}

bool requestStartDemoWithPlayerKeepCarry(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "プレイヤー含むデモ(持ち物維持)",
                                 demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWithPlayerKeepCarry(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ(持ち物維持)", demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoWithPlayerUseCoin(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "プレイヤー含むデモ(コイン使用)",
                                 demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWithPlayerUseCoin(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ(コイン使用)", demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoWithPlayerKeepBindTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "プレイヤー含むデモ(バインド継続)[会話]",
                                 demoTypeWithPlayerKeepBindTalk());
}

void requestEndDemoWithPlayerKeepBindTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ(バインド継続)[会話]",
                        demoTypeWithPlayerKeepBindTalk());
}

bool requestStartDemoKeepHackTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "キャプチャ継続デモ[会話]",
                                 demoTypeKeepHackTalk());
}

void requestEndDemoKeepHackTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "キャプチャ継続デモ[会話]", demoTypeKeepHackTalk());
}

bool requestStartDemoWithPlayerCinemaFrameTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "プレイヤー含むデモ(シネマフレームあり)[会話]",
                                 demoTypeWithPlayerCinemaFrame());
}

void requestEndDemoWithPlayerCinemaFrameTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ(シネマフレームあり)[会話]",
                        demoTypeWithPlayerCinemaFrame());
}

bool requestStartDemoWithPlayerKeepCarryTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "プレイヤー含むデモ(持ち物維持)[会話]",
                                 demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWithPlayerKeepCarryTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ(持ち物維持)[会話]",
                        demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoWithPlayerUseCoinTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "プレイヤー含むデモ(コイン使用)[会話]",
                                 demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWithPlayerUseCoinTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ(コイン使用)[会話]",
                        demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoNormalTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, "通常デモ[会話]", demoTypeNormal());
}

void requestEndDemoNormalTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "通常デモ[会話]", demoTypeNormal());
}

bool requestStartDemoWarpToCheckpoint(const al::Scene* scene) {
    return requestStartDemoScene(scene, "中間ワープ", demoTypeWithPlayer());
}

bool requestStartDemoBossBattleStart(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "プレイヤー含むデモ(シネマフレームあり)",
                                 demoTypeWithPlayerCinemaFrame());
}

void requestEndDemoBossBattleStart(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ(シネマフレームあり)",
                        demoTypeWithPlayerCinemaFrame());
}

bool requestStartDemoBossBattleEnd(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "プレイヤー含むデモ(シネマフレームあり)",
                                 demoTypeWithPlayerCinemaFrame());
}

void requestEndDemoBossBattleEnd(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "プレイヤー含むデモ(シネマフレームあり)",
                        demoTypeWithPlayerCinemaFrame());
}

bool requestStartDemoShineGet(Shine* shine) {
    if (!getDirector(shine)->requestStartDemo("シャイン取得デモ", demoTypeShineGet()))
        return false;

    getDirector(shine)->addDemoActor(shine);
    getDirector(shine)->setRequestActor(shine);
    getDirector(shine)->setShine(shine);
    return true;
}

bool requestStartDemoAchievementShineGet(al::Scene* scene) {
    if (!getDirector(scene)->requestStartDemo("シャイン取得デモ", demoTypeShineGet()))
        return false;

    endDemoShineGet(scene);
    return true;
}

void endDemoShineGet(al::Scene* scene) {
    getDirector(scene)->clearShine();
}

void requestEndDemoShineGet(const al::Scene* scene) {
    getDirector(scene)->requestEndDemo("シャイン取得デモ", demoTypeShineGet());
    getDirector(scene)->clearShine();
}

void requestEndDemoShineGet(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "シャイン取得デモ", demoTypeShineGet());
}

bool requestStartDemoShineDotGet(Shine* shine) {
    if (!getDirector(shine)->requestStartDemo("シャインドット取得デモ", demoTypeShineGet()))
        return false;

    getDirector(shine)->addDemoActor(shine);
    getDirector(shine)->setRequestActor(shine);
    getDirector(shine)->setShine(shine);
    return true;
}

void requestEndDemoShineDotGet(const al::Scene* scene) {
    requestEndDemoScene(scene, "シャインドット取得デモ", demoTypeShineGet());
}

bool requestStartDemoShineMainGet(Shine* shine) {
    if (!getDirector(shine)->requestStartDemoShineMainGet(shine, "メインシャイン取得デモ[全部]"))
        return false;

    getDirector(shine)->addDemoActor(shine);
    getDirector(shine)->setRequestActor(shine);
    setDemoInfoDemoName(shine, "メインシャイン取得デモ[準備]");
    return true;
}

void setDemoInfoDemoName(const al::LiveActor* actor, const char* demoName) {
    getDirector(actor)->setDemoInfoDemoName(demoName);
}

void requestEndDemoShineMainGet(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoShineMainGet("メインシャイン取得デモ[全部]");
}

void requestEndDemoShineMainGet(const al::Scene* scene) {
    getDirector(scene)->requestEndDemoShineMainGet("メインシャイン取得デモ[全部]");
}

bool requestStartDemoShineGrandGet(Shine* shine) {
    if (!getDirector(shine)->requestStartDemoShineGrandGet(shine, "グランドシャイン取得デモ"))
        return false;

    getDirector(shine)->addDemoActor(shine);
    getDirector(shine)->setRequestActor(shine);
    return true;
}

void requestEndDemoShineGrandGet(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoShineGrandGet("グランドシャイン取得デモ");
}

void requestEndDemoShineGrandGet(const al::Scene* scene) {
    getDirector(scene)->requestEndDemoShineGrandGet("グランドシャイン取得デモ");
}

bool requestStartDemoShineAppear(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "シャイン出現デモ", demoTypeNormal());
}

void requestEndDemoShineAppear(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "シャイン出現デモ", demoTypeNormal());
}

bool requestStartDemoShineAppear(const al::Scene* scene) {
    return requestStartDemoScene(scene, "シャイン出現デモ", demoTypeNormal());
}

void requestEndDemoShineAppear(const al::Scene* scene) {
    requestEndDemoScene(scene, "シャイン出現デモ", demoTypeNormal());
}

bool requestStartDemoWarp(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "ワープデモ", demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoWarp(const al::Scene* scene) {
    return requestStartDemoScene(scene, "ワープデモ", demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWarp(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "ワープデモ", demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWarp(const al::Scene* scene) {
    requestEndDemoScene(scene, "ワープデモ", demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoHackStart(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "憑依開始デモ", demoTypeWithPlayer());
}

void requestEndDemoHackStart(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "憑依開始デモ", demoTypeWithPlayer());
}

bool requestStartDemoAppearByBazookaElectric(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "電気大砲から出てきたデモ", demoTypeWithPlayer());
}

void requestEndDemoAppearByBazookaElectric(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "電気大砲から出てきたデモ", demoTypeWithPlayer());
}

bool requestStartDemoLaunchBazookaElectric(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "電気大砲発射デモ", demoTypeNormal());
}

void requestEndDemoLaunchBazookaElectric(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "電気大砲発射デモ", demoTypeNormal());
}

bool requestStartDemoSceneStartPlayerWalk(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "シーン開始プレイヤー歩きデモ",
                                 demoTypeSceneStartPlayerWalk());
}

void requestEndDemoSceneStartPlayerWalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "シーン開始プレイヤー歩きデモ", demoTypeSceneStartPlayerWalk());
}

bool requestStartDemoGetLifeMaxUpItem(al::LiveActor* actor) {
    if (!getDirector(actor)->requestStartDemo("最大ライフアップアイテムゲットデモ",
                                              demoTypeNormal()))
        return false;

    getDirector(actor)->setRequestActor(actor);
    return true;
}

void requestEndDemoGetLifeMaxUpItem(const al::Scene* scene) {
    requestEndDemoScene(scene, "最大ライフアップアイテムゲットデモ", demoTypeNormal());
}

bool requestStartDemoBirdCarryMeat(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "肉運びデモ", demoTypeNormalWithCinemaFrame());
}

void requestEndDemoBirdCarryMeat(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "肉運びデモ", demoTypeNormal());
}

bool requestStartDemoBirdFindMeat(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "肉発見デモ", demoTypeNormal());
}

void requestEndDemoBirdFindMeat(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "肉発見デモ", demoTypeNormal());
}

bool requestStartDemoGetTimeBalloon(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "風船割り風船取得デモ", demoTypeWithPlayer());
}

void requestEndDemoGetTimeBalloon(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "風船割り風船取得デモ", demoTypeWithPlayer());
}

bool requestStartDemoJango(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "ジャンゴデモ", demoTypeWithPlayer());
}

void requestEndDemoJango(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "ジャンゴデモ", demoTypeWithPlayer());
}

bool requestStartDemoTitleLogo(const al::LiveActor* actor) {
    return getDirector(actor)->requestStartDemo("タイトルデモ", demoTypeNormal());
}

bool requestStartDemoGoToEnding(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "エンディングに行くデモ", demoTypeNormal());
}

bool isActiveDemo(const al::LiveActor* actor) {
    return static_cast<al::DemoDirector*>(getDirector(actor))->isActiveDemo();
}

bool isActiveDemo(const al::Scene* scene) {
    return static_cast<al::DemoDirector*>(getDirector(scene))->isActiveDemo();
}

bool isActiveDemoNormal(const al::Scene* scene) {
    return isActiveDemoScene(scene, "通常デモ");
}

bool isActiveDemoNormalWithCinemaFrame(const al::Scene* scene) {
    return isActiveDemoScene(scene, "通常デモ(シネマフレームあり)");
}

bool isActiveDemoWithPlayer(const al::Scene* scene) {
    return getDirector(scene)->isActiveDemoWithPlayer() ||
           getDirector(scene)->isActiveDemo("キャプチャ継続デモ[会話]");
}

bool isActiveDemoWithPlayer(const al::LiveActor* actor) {
    return getDirector(actor)->isActiveDemoWithPlayer() ||
           getDirector(actor)->isActiveDemo("キャプチャ継続デモ[会話]");
}

bool isActiveDemoWithPlayerCinemaFrame(const al::Scene* scene) {
    return isActiveDemoScene(scene, "プレイヤー含むデモ(シネマフレームあり)") ||
           isActiveDemoCinemaFrameTalk(scene);
}

bool isActiveDemoWithPlayerCinemaFrame(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "プレイヤー含むデモ(シネマフレームあり)") ||
           isActiveDemoActor(actor, "プレイヤー含むデモ(シネマフレームあり)[会話]");
}

bool isActiveDemoWithPlayerKeepCarry(const al::Scene* scene) {
    return isActiveDemoScene(scene, "プレイヤー含むデモ(持ち物維持)") ||
           isActiveDemoKeepCarryTalk(scene);
}

bool isActiveDemoWithPlayerKeepCarry(const al::LiveActor* actor) {
    return (isActiveDemoActor(actor, "プレイヤー含むデモ(持ち物維持)")) ||
           isActiveDemoActor(actor, "プレイヤー含むデモ(持ち物維持)[会話]");
}

bool isActiveDemoWithPlayerUseCoin(const al::Scene* scene) {
    return isActiveDemoScene(scene, "プレイヤー含むデモ(コイン使用)") ||
           isActiveDemoUseCoinTalk(scene);
}

bool isActiveDemoWithPlayerUseCoin(const al::LiveActor* actor) {
    return (isActiveDemoActor(actor, "プレイヤー含むデモ(コイン使用)")) ||
           isActiveDemoActor(actor, "プレイヤー含むデモ(コイン使用)[会話]");
}

bool isActiveDemoWithPlayerKeepBindTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, "プレイヤー含むデモ(バインド継続)[会話]");
}

bool isActiveDemoWithPlayerKeepBindTalk(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "プレイヤー含むデモ(バインド継続)[会話]");
}

bool isActiveDemoWarpToCheckpoint(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "中間ワープ");
}

bool isActiveDemoShineGet(const al::Scene* scene) {
    return isActiveDemoScene(scene, "シャイン取得デモ") ||
           isActiveDemoScene(scene, "シャインドット取得デモ");
}

bool isActiveDemoShineGet(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "シャイン取得デモ") ||
           isActiveDemoActor(actor, "シャインドット取得デモ");
}

bool isActiveDemoShineDotGet(const al::Scene* scene) {
    return isActiveDemoScene(scene, "シャインドット取得デモ");
}

bool isActiveDemoShineDotGet(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "シャインドット取得デモ");
}

bool isActiveDemoShineGrandGet(const al::Scene* scene) {
    return isActiveDemoScene(scene, "グランドシャイン取得デモ");
}

bool isActiveDemoShineGrandGet(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "グランドシャイン取得デモ");
}

bool isActiveDemoShineMainGet(const al::Scene* scene) {
    return isActiveDemoScene(scene, "メインシャイン取得デモ[全部]");
}

bool isActiveDemoShineMainGet(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "メインシャイン取得デモ[全部]");
}

bool isActiveDemoWarp(const al::Scene* scene) {
    return isActiveDemoScene(scene, "ワープデモ");
}

bool isActiveDemoWarp(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "ワープデモ");
}

bool isActiveDemoHackStart(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "憑依開始デモ");
}

bool isActiveDemoHackStart(const al::Scene* scene) {
    return isActiveDemoScene(scene, "憑依開始デモ");
}

bool isActiveDemoHackStartFirst(const al::Scene* scene) {
    return isEqualDemoInfoDemoName(scene, DemoName::cHackStartFirstTimeFrog) ||
           isEqualDemoInfoDemoName(scene, DemoName::cHackStartFirstTimeKoopa);
}

bool isEqualDemoInfoDemoName(const al::Scene* scene, const char* demoName) {
    const char* activeDemoName = getDirector(scene)->getDemoInfoDemoName();
    return activeDemoName && al::isEqualString(activeDemoName, demoName);
}

bool isActiveDemoScenarioCamera(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "シナリオカメラデモ");
}

bool isActiveDemoGetLifeMaxUpItem(const al::Scene* scene) {
    return isActiveDemoScene(scene, "最大ライフアップアイテムゲットデモ");
}

bool isActiveDemoSceneStartPlayerWalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, "シーン開始プレイヤー歩きデモ");
}

bool isActiveDemoBirdCarryMeat(const al::Scene* scene) {
    return isActiveDemoScene(scene, "肉運びデモ");
}

bool isActiveDemoBirdFindMeat(const al::Scene* scene) {
    return isActiveDemoScene(scene, "肉発見デモ");
}

bool isActiveDemoJango(const al::Scene* scene) {
    return isActiveDemoScene(scene, "ジャンゴデモ");
}

bool isActiveDemoJango(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, "ジャンゴデモ");
}

bool isActiveDemoTitleLogo(const al::Scene* scene) {
    return isActiveDemoScene(scene, "タイトルデモ");
}

bool isActiveDemoGoToEnding(const al::Scene* scene) {
    return isActiveDemoScene(scene, "エンディングに行くデモ");
}

bool isActiveDemoTalk(const al::Scene* scene) {
    return isActiveDemoCinemaFrameTalk(scene) || isActiveDemoKeepCarryTalk(scene) ||
           isActiveDemoKeepHackTalk(scene) || isActiveDemoNormalTalk(scene) ||
           isActiveDemoWithPlayerKeepBindTalk(scene);
}

bool isActiveDemoCinemaFrameTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, "プレイヤー含むデモ(シネマフレームあり)[会話]");
}

bool isActiveDemoKeepCarryTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, "プレイヤー含むデモ(持ち物維持)[会話]");
}

bool isActiveDemoUseCoinTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, "プレイヤー含むデモ(コイン使用)[会話]");
}

bool isActiveDemoKeepHackTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, "キャプチャ継続デモ[会話]");
}

bool isActiveDemoNormalTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, "通常デモ[会話]");
}

bool isActiveDemoLaunchBazookaElectric(const al::Scene* scene) {
    return isActiveDemoScene(scene, "電気大砲発射デモ");
}

bool isActiveDemoGiantWanderBossBattleStart(const al::Scene* scene) {
    return isEqualDemoInfoDemoName(scene, DemoName::cGiantWanderBossBattleStart);
}

bool isActiveDemoGiantWanderBossBattleStart(const OceanWaveActor* actor) {
    const char* demoName = getDirector(actor)->getDemoInfoDemoName();
    if (demoName == nullptr)
        return false;
    return al::isEqualString(demoName, DemoName::cGiantWanderBossBattleStart);
}

bool isActiveDemoPlayerPuppetable(const al::LiveActor* actor) {
    if (getDirector(actor)->isActiveDemoWithPlayer())
        return true;
    if (isActiveDemoActor(actor, "キャプチャ継続デモ[会話]"))
        return true;
    if (isActiveDemoActor(actor, "プレイヤー含むデモ(シネマフレームあり)"))
        return true;
    if (isActiveDemoActor(actor, "プレイヤー含むデモ(シネマフレームあり)[会話]"))
        return true;
    if (isActiveDemoActor(actor, "プレイヤー含むデモ(持ち物維持)"))
        return true;
    if (isActiveDemoActor(actor, "プレイヤー含むデモ(持ち物維持)[会話]"))
        return true;
    if (isActiveDemoActor(actor, "プレイヤー含むデモ(コイン使用)"))
        return true;
    if (isActiveDemoActor(actor, "プレイヤー含むデモ(コイン使用)[会話]"))
        return true;
    if (isActiveDemoActor(actor, "プレイヤー含むデモ(バインド継続)[会話]"))
        return true;
    if (isActiveDemoActor(actor, "シャイン取得デモ"))
        return true;
    if (isActiveDemoActor(actor, "シャインドット取得デモ"))
        return true;
    if (isActiveDemoActor(actor, "グランドシャイン取得デモ"))
        return true;
    if (isActiveDemoActor(actor, "メインシャイン取得デモ[全部]"))
        return true;
    if (isActiveDemoActor(actor, "ワープデモ"))
        return true;
    if (isActiveDemoActor(actor, "憑依開始デモ"))
        return true;
    if (isActiveDemoActor(actor, "ホームから出現するデモ"))
        return true;
    return isActiveDemoActor(actor, "ホームに戻るデモ");
}

void addDemoSubActor(al::LiveActor* actor) {
    if (!al::isExistSubActorKeeper(actor))
        return;

    s32 subActorNum = al::getSubActorNum(actor);

    for (s32 i = 0; i < subActorNum; i++) {
        al::LiveActor* subActor = al::getSubActor(actor, i);
        getDirector(subActor)->addDemoActorWithSubActor(subActor);
    }
}

void registerShineTowerRocketToDemoDirector(ShineTowerRocket* shineTowerRocket) {
    getDirector(shineTowerRocket)->setShineTowerRocket(shineTowerRocket);
}

ShineTowerRocket* tryGetShineTowerRocketFromDemoDirector(al::LiveActor* actor) {
    return getDirector(actor)->getShineTowerRocket();
}

void addDemoAppearFromHomeToList(ShineTowerRocket* shineTowerRocket) {
    getDirector(shineTowerRocket)->addDemoAppearFromHomeToList(shineTowerRocket);
}

void addDemoReturnToHomeToList(ShineTowerRocket* shineTowerRocket) {
    getDirector(shineTowerRocket)->addDemoReturnToHomeToList(shineTowerRocket);
}

void addDemoRiseMapPartsToList(RiseMapPartsHolder* riseMapPartsHolder) {
    getDirector(riseMapPartsHolder)->addDemoRiseMapPartsToList(riseMapPartsHolder);
}

bool requestStartDemoWorldIntroCamera(const al::Scene* scene) {
    return requestStartDemoScene(scene, "ワールド紹介カメラデモ", demoTypeWithPlayer());
}

void requestEndDemoWorldIntroCamera(const al::Scene* scene) {
    getDirector(scene)->requestEndDemo("ワールド紹介カメラデモ", demoTypeWithPlayer());
    GameDataHolderAccessor skipAccessor(scene);
    skipAccessor->getGameDataFile()->getSequenceDemoSkipData()->showWorldIntroCamera(
        GameDataFunction::getCurrentWorldIdNoDevelop(scene));
}

bool requestStartDemoScenarioCamera(const al::Scene* scene) {
    return getDirector(scene)->requestStartDemoScenarioCamera("シナリオカメラデモ");
}

void requestEndDemoScenarioCamera(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoScenarioCamera("シナリオカメラデモ");
    GameDataHolderAccessor skipAccessor(actor);
    skipAccessor->getGameDataFile()->getSequenceDemoSkipData()->showScenarioStartCamera(
        GameDataFunction::getCurrentWorldIdNoDevelop(actor), getActiveQuestNo(actor));
}

bool requestStartDemoAppearFromHome(const al::Scene* scene) {
    return getDirector(scene)->requestStartDemoAppearFromHome("ホームから出現するデモ");
}

void requestEndDemoAppearFromHome(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoAppearFromHome("ホームから出現するデモ");
}

bool requestStartDemoReturnToHome(const al::Scene* scene) {
    return getDirector(scene)->requestStartDemoReturnToHome("ホームに戻るデモ");
}

void requestEndDemoReturnToHome(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoReturnToHome("ホームに戻るデモ");
}

bool requestStartDemoRiseMapParts(const al::Scene* scene) {
    return getDirector(scene)->requestStartDemoRiseMapParts("地形隆起デモ");
}

void requestEndDemoRiseMapParts(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoRiseMapParts("地形隆起デモ");
}

bool requestStartDemoPeachCastleCap(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, "ピーチ城帽子デモ", demoTypeWithPlayer());
}

void requestEndDemoPeachCastleCap(const al::LiveActor* actor) {
    requestEndDemoActor(actor, "ピーチ城帽子デモ", demoTypeWithPlayer());
}

bool isExistDemoAppearFromHomeInList(const al::Scene* scene) {
    return getDirector(scene)->isExistDemoAppearFromHomeInList();
}

bool isExistDemoReturnToHome(const al::Scene* scene) {
    return getDirector(scene)->isExistDemoReturnToHome();
}

bool isExistDemoRiseMapPartsInList(const al::Scene* scene) {
    return getDirector(scene)->isExistDemoRiseMapPartsInList();
}

al::LiveActor* getRequestActor(const al::Scene* scene) {
    return getDirector(scene)->getRequestActor();
}

void requestValidateDemoSkip(IUseDemoSkip* demoSkipRequester, const al::LiveActor* actor) {
    getDirector(actor)->setDemoSkipRequester(demoSkipRequester);
}

IUseDemoSkip* getDemoSkipRequester(const al::Scene* scene) {
    return getDirector(scene)->getDemoSkipRequester();
}

bool isDemoEnableSkip(const al::Scene* scene) {
    return getDemoSkipRequester(scene) != nullptr;
}

bool isFirstDemo(const al::Scene* scene) {
    return getDemoSkipRequester(scene)->isFirstDemo();
}

bool isEnableSkipDemo(const al::Scene* scene) {
    return getDemoSkipRequester(scene)->isEnableSkipDemo();
}

void skipDemo(const al::Scene* scene) {
    getDemoSkipRequester(scene)->skipDemo();
}

void updateOnlyDemoGraphics(const al::Scene* scene) {
    getDemoSkipRequester(scene)->updateOnlyDemoGraphics();
}

bool isFirstDemoWorldIntroCamera(const al::Scene* scene) {
    GameDataHolderAccessor skipAccessor(scene);
    return !skipAccessor->getGameDataFile()
                ->getSequenceDemoSkipData()
                ->isAlreadyShowWorldIntroCamera(
                    GameDataFunction::getCurrentWorldIdNoDevelop(scene));
}

bool isFirstDemoScenarioStartCamera(const al::LiveActor* actor) {
    GameDataHolderAccessor skipAccessor(actor);
    return !skipAccessor->getGameDataFile()
                ->getSequenceDemoSkipData()
                ->isAlreadyShowScenarioStartCamera(
                    GameDataFunction::getCurrentWorldIdNoDevelop(actor), getActiveQuestNo(actor));
}

void noticeDemoSkipStart(const al::Scene* scene) {
    getDirector(scene)->setDemoSkipping(true);
}

void noticeDemoSkipEnd(const al::Scene* scene) {
    getDirector(scene)->setDemoSkipping(false);
}

bool isWaitDemoSkipEnd(const al::LiveActor* actor) {
    return getDirector(actor)->isDemoSkipping();
}

CaptionInfoHolder* createCaptionInfoHolder(const char* captionName) {
    return CinemaCaptionFunction::createCaptionInfoHolder(captionName);
}

void validateDemoCaption(const al::LiveActor* actor, CaptionInfoHolder* captionInfoHolder) {
    getDirector(actor)->setCaptionInfoHolder(captionInfoHolder);
}

bool isActiveDemoWithCinemaCaption(const al::Scene* scene) {
    return getDirector(scene)->isActiveDemoWithCinemaCaption();
}

bool tryAppearCinemaCaption(const al::Scene* scene, CinemaCaption* cinemaCaption) {
    return isActiveDemoWithCinemaCaption(scene) &&
           cinemaCaption->tryAppear(getDirector(scene)->getCaptionInfoHolder());
}

void startCloseDemoFade(const al::LiveActor* actor, s32 frames) {
    getDirector(actor)->startCloseDemoFade(frames);
}

void startOpenDemoFade(const al::LiveActor* actor, s32 frames) {
    getDirector(actor)->startOpenDemoFade(frames);
}

bool isDemoFadeCloseEnd(const al::LiveActor* actor) {
    return getDirector(actor)->isDemoFadeCloseEnd();
}

bool isDemoFadeOpenEnd(const al::LiveActor* actor) {
    return getDirector(actor)->isDemoFadeOpenEnd();
}

bool isDemoFadeWipeStateEnableOpenMenuOrSnapShotMode(const al::Scene* scene) {
    ProjectDemoDirector* director = getDirector(scene);
    if (!director)
        return true;
    return director->isWipeStateEnableOpenMenuOrSnapShotMode();
}

DemoHackFirstDirector* tryCreateDemoHackFirstDirector(al::LiveActor* actor, s32 num,
                                                      const al::ActorInitInfo& initInfo) {
    return getDirector(actor)->tryCreateDemoHackFirstDirector(actor, num, initInfo);
}

void noticeDemoStartToDemoHackFirstDirector(const al::Scene* scene) {
    getDirector(scene)->noticeDemoStartToDemoHackFirstDirector();
}

void setBossBarrierField(BarrierField* barrierField) {
    getDirector(barrierField)->setBossBarrierField(barrierField);
}

BarrierField* tryGetBossBarrierField(const al::LiveActor* actor) {
    return getDirector(actor)->getBossBarrierField();
}

void setDemoEnvironmentChangeFlag(const al::Scene* scene, bool isDemoEnvironmentChange) {
    getDirector(scene)->setDemoEnvironmentChangeFlag(isDemoEnvironmentChange);
}

void setDemoEnvironmentChangeFlag(const al::LiveActor* actor, bool isDemoEnvironmentChange) {
    getDirector(actor)->setDemoEnvironmentChangeFlag(isDemoEnvironmentChange);
}

bool isDemoEnvironmentChange(const al::Scene* scene) {
    return getDirector(scene)->isDemoEnvironmentChangeFlag();
}

bool isDemoEnvironmentChange(const al::LiveActor* actor) {
    return getDirector(actor)->isDemoEnvironmentChangeFlag();
}

__attribute__((noinline)) s32 getRandomActionIndexDemoChangeWorld(const al::LiveActor* actor) {
    return getDirector(actor)->getRandomActionIndexDemoChangeWorld();
}

void setDemoInfoDemoName(const al::Scene* scene, const char* demoName) {
    getDirector(scene)->setDemoInfoDemoName(demoName);
}

const char* getDemoInfoDemoName(const al::LiveActor* actor) {
    return getDirector(actor)->getDemoInfoDemoName();
}

bool isTriggerDemoCancel(const al::IUseSceneObjHolder* holder) {
    return isTriggerUiPause(holder);
}

void updateDemoSystemOnly(const al::Scene* scene) {
    getDirector(scene)->updateSystemOnly();
}

void notifyDisableUpdateCamera(const al::LiveActor* actor) {
    getDirector(actor)->setDisableUpdateCamera();
}

bool isDisableUpdateCamera(const al::Scene* scene) {
    return getDirector(scene)->isDisableUpdateCamera();
}
}  // namespace rs
