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
constexpr const char* cDemoNormal = "通常デモ";
constexpr const char* cDemoNormalWithCinemaFrame = "通常デモ(シネマフレームあり)";
constexpr const char* cDemoWithPlayer = "プレイヤー含むデモ";
constexpr const char* cDemoWithPlayerCinemaFrame = "プレイヤー含むデモ(シネマフレームあり)";
constexpr const char* cDemoWithPlayerKeepCarry = "プレイヤー含むデモ(持ち物維持)";
constexpr const char* cDemoWithPlayerUseCoin = "プレイヤー含むデモ(コイン使用)";
constexpr const char* cDemoWithPlayerKeepBindTalk = "プレイヤー含むデモ(バインド継続)[会話]";
constexpr const char* cDemoKeepHackTalk = "キャプチャ継続デモ[会話]";
constexpr const char* cDemoWithPlayerCinemaFrameTalk =
    "プレイヤー含むデモ(シネマフレームあり)[会話]";
constexpr const char* cDemoWithPlayerKeepCarryTalk = "プレイヤー含むデモ(持ち物維持)[会話]";
constexpr const char* cDemoWithPlayerUseCoinTalk = "プレイヤー含むデモ(コイン使用)[会話]";
constexpr const char* cDemoNormalTalk = "通常デモ[会話]";
constexpr const char* cDemoWarpToCheckpoint = "中間ワープ";
constexpr const char* cDemoShineGet = "シャイン取得デモ";
constexpr const char* cDemoShineDotGet = "シャインドット取得デモ";
constexpr const char* cDemoShineMainGet = "メインシャイン取得デモ[全部]";
constexpr const char* cDemoShineMainGetPrepare = "メインシャイン取得デモ[準備]";
constexpr const char* cDemoShineGrandGet = "グランドシャイン取得デモ";
constexpr const char* cDemoShineAppear = "シャイン出現デモ";
constexpr const char* cDemoWarp = "ワープデモ";
constexpr const char* cDemoHackStart = "憑依開始デモ";
constexpr const char* cDemoAppearByBazookaElectric = "電気大砲から出てきたデモ";
constexpr const char* cDemoLaunchBazookaElectric = "電気大砲発射デモ";
constexpr const char* cDemoSceneStartPlayerWalk = "シーン開始プレイヤー歩きデモ";
constexpr const char* cDemoGetLifeMaxUpItem = "最大ライフアップアイテムゲットデモ";
constexpr const char* cDemoBirdCarryMeat = "肉運びデモ";
constexpr const char* cDemoBirdFindMeat = "肉発見デモ";
constexpr const char* cDemoGetTimeBalloon = "風船割り風船取得デモ";
constexpr const char* cDemoJango = "ジャンゴデモ";
constexpr const char* cDemoTitleLogo = "タイトルデモ";
constexpr const char* cDemoGoToEnding = "エンディングに行くデモ";
constexpr const char* cDemoWorldIntroCamera = "ワールド紹介カメラデモ";
constexpr const char* cDemoScenarioCamera = "シナリオカメラデモ";
constexpr const char* cDemoAppearFromHome = "ホームから出現するデモ";
constexpr const char* cDemoReturnToHome = "ホームに戻るデモ";
constexpr const char* cDemoRiseMapParts = "地形隆起デモ";
constexpr const char* cDemoPeachCastleCap = "ピーチ城帽子デモ";

constexpr const char* cActionDemoWorldMoveForwardNormal = "DemoWorldMoveForwardNormal";
constexpr const char* cActionDemoWorldMoveForward = "DemoWorldMoveForward";
constexpr const char* cActionDemoWorldMoveBackward = "DemoWorldMoveBackward";

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

    return al::isEqualString(actionName, cActionDemoWorldMoveForwardNormal) ||
           al::isEqualString(actionName, cActionDemoWorldMoveForward) ||
           al::isEqualString(actionName, cActionDemoWorldMoveBackward);
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
    return requestStartDemoActor(actor, isSubActor, cDemoNormal, demoTypeNormal());
}

void addDemoActor(al::LiveActor* actor, bool isSubActor) {
    if (isSubActor)
        getDirector(actor)->addDemoActorWithSubActor(actor);
    else
        getDirector(actor)->addDemoActor(actor);
}

void requestEndDemoNormal(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoNormal, demoTypeNormal());
}

bool requestStartDemoNormal(const al::Scene* scene) {
    return requestStartDemoScene(scene, cDemoNormal, demoTypeNormal());
}

void requestEndDemoNormal(const al::Scene* scene) {
    requestEndDemoScene(scene, cDemoNormal, demoTypeNormal());
}

bool requestStartDemoNormalWithCinemaFrame(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoNormalWithCinemaFrame,
                                 demoTypeNormalWithCinemaFrame());
}

void requestEndDemoNormalWithCinemaFrame(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoNormalWithCinemaFrame, demoTypeNormalWithCinemaFrame());
}

bool requestStartDemoWithPlayer(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoWithPlayer, demoTypeWithPlayer());
}

void requestEndDemoWithPlayer(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayer, demoTypeWithPlayer());
}

bool requestStartDemoWithPlayer(const al::Scene* scene) {
    return requestStartDemoScene(scene, cDemoWithPlayer, demoTypeWithPlayer());
}

void requestEndDemoWithPlayer(const al::Scene* scene) {
    requestEndDemoScene(scene, cDemoWithPlayer, demoTypeWithPlayer());
}

bool requestStartDemoWithPlayerCinemaFrame(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoWithPlayerCinemaFrame,
                                 demoTypeWithPlayerCinemaFrame());
}

void requestEndDemoWithPlayerCinemaFrame(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayerCinemaFrame, demoTypeWithPlayerCinemaFrame());
}

bool requestStartDemoWithPlayerKeepCarry(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoWithPlayerKeepCarry,
                                 demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWithPlayerKeepCarry(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayerKeepCarry, demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoWithPlayerUseCoin(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoWithPlayerUseCoin,
                                 demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWithPlayerUseCoin(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayerUseCoin, demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoWithPlayerKeepBindTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoWithPlayerKeepBindTalk,
                                 demoTypeWithPlayerKeepBindTalk());
}

void requestEndDemoWithPlayerKeepBindTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayerKeepBindTalk, demoTypeWithPlayerKeepBindTalk());
}

bool requestStartDemoKeepHackTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoKeepHackTalk, demoTypeKeepHackTalk());
}

void requestEndDemoKeepHackTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoKeepHackTalk, demoTypeKeepHackTalk());
}

bool requestStartDemoWithPlayerCinemaFrameTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoWithPlayerCinemaFrameTalk,
                                 demoTypeWithPlayerCinemaFrame());
}

void requestEndDemoWithPlayerCinemaFrameTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayerCinemaFrameTalk, demoTypeWithPlayerCinemaFrame());
}

bool requestStartDemoWithPlayerKeepCarryTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoWithPlayerKeepCarryTalk,
                                 demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWithPlayerKeepCarryTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayerKeepCarryTalk, demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoWithPlayerUseCoinTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoWithPlayerUseCoinTalk,
                                 demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWithPlayerUseCoinTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayerUseCoinTalk, demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoNormalTalk(al::LiveActor* actor, bool isSubActor) {
    return requestStartDemoActor(actor, isSubActor, cDemoNormalTalk, demoTypeNormal());
}

void requestEndDemoNormalTalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoNormalTalk, demoTypeNormal());
}

bool requestStartDemoWarpToCheckpoint(const al::Scene* scene) {
    return requestStartDemoScene(scene, cDemoWarpToCheckpoint, demoTypeWithPlayer());
}

bool requestStartDemoBossBattleStart(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoWithPlayerCinemaFrame,
                                 demoTypeWithPlayerCinemaFrame());
}

void requestEndDemoBossBattleStart(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayerCinemaFrame, demoTypeWithPlayerCinemaFrame());
}

bool requestStartDemoBossBattleEnd(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoWithPlayerCinemaFrame,
                                 demoTypeWithPlayerCinemaFrame());
}

void requestEndDemoBossBattleEnd(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWithPlayerCinemaFrame, demoTypeWithPlayerCinemaFrame());
}

bool requestStartDemoShineGet(Shine* shine) {
    if (!getDirector(shine)->requestStartDemo(cDemoShineGet, demoTypeShineGet()))
        return false;

    getDirector(shine)->addDemoActor(shine);
    getDirector(shine)->setRequestActor(shine);
    getDirector(shine)->setShine(shine);
    return true;
}

bool requestStartDemoAchievementShineGet(al::Scene* scene) {
    if (!getDirector(scene)->requestStartDemo(cDemoShineGet, demoTypeShineGet()))
        return false;

    endDemoShineGet(scene);
    return true;
}

void endDemoShineGet(al::Scene* scene) {
    getDirector(scene)->clearShine();
}

void requestEndDemoShineGet(const al::Scene* scene) {
    getDirector(scene)->requestEndDemo(cDemoShineGet, demoTypeShineGet());
    getDirector(scene)->clearShine();
}

void requestEndDemoShineGet(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoShineGet, demoTypeShineGet());
}

bool requestStartDemoShineDotGet(Shine* shine) {
    if (!getDirector(shine)->requestStartDemo(cDemoShineDotGet, demoTypeShineGet()))
        return false;

    getDirector(shine)->addDemoActor(shine);
    getDirector(shine)->setRequestActor(shine);
    getDirector(shine)->setShine(shine);
    return true;
}

void requestEndDemoShineDotGet(const al::Scene* scene) {
    requestEndDemoScene(scene, cDemoShineDotGet, demoTypeShineGet());
}

bool requestStartDemoShineMainGet(Shine* shine) {
    if (!getDirector(shine)->requestStartDemoShineMainGet(shine, cDemoShineMainGet))
        return false;

    getDirector(shine)->addDemoActor(shine);
    getDirector(shine)->setRequestActor(shine);
    getDirector(shine)->setDemoInfoDemoName(cDemoShineMainGetPrepare);
    return true;
}

void setDemoInfoDemoName(const al::LiveActor* actor, const char* demoName) {
    getDirector(actor)->setDemoInfoDemoName(demoName);
}

void requestEndDemoShineMainGet(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoShineMainGet(cDemoShineMainGet);
}

void requestEndDemoShineMainGet(const al::Scene* scene) {
    getDirector(scene)->requestEndDemoShineMainGet(cDemoShineMainGet);
}

bool requestStartDemoShineGrandGet(Shine* shine) {
    if (!getDirector(shine)->requestStartDemoShineGrandGet(shine, cDemoShineGrandGet))
        return false;

    getDirector(shine)->addDemoActor(shine);
    getDirector(shine)->setRequestActor(shine);
    return true;
}

void requestEndDemoShineGrandGet(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoShineGrandGet(cDemoShineGrandGet);
}

void requestEndDemoShineGrandGet(const al::Scene* scene) {
    getDirector(scene)->requestEndDemoShineGrandGet(cDemoShineGrandGet);
}

bool requestStartDemoShineAppear(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoShineAppear, demoTypeNormal());
}

void requestEndDemoShineAppear(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoShineAppear, demoTypeNormal());
}

bool requestStartDemoShineAppear(const al::Scene* scene) {
    return requestStartDemoScene(scene, cDemoShineAppear, demoTypeNormal());
}

void requestEndDemoShineAppear(const al::Scene* scene) {
    requestEndDemoScene(scene, cDemoShineAppear, demoTypeNormal());
}

bool requestStartDemoWarp(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoWarp, demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoWarp(const al::Scene* scene) {
    return requestStartDemoScene(scene, cDemoWarp, demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWarp(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoWarp, demoTypeWithPlayerKeepCarry());
}

void requestEndDemoWarp(const al::Scene* scene) {
    requestEndDemoScene(scene, cDemoWarp, demoTypeWithPlayerKeepCarry());
}

bool requestStartDemoHackStart(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoHackStart, demoTypeWithPlayer());
}

void requestEndDemoHackStart(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoHackStart, demoTypeWithPlayer());
}

bool requestStartDemoAppearByBazookaElectric(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoAppearByBazookaElectric, demoTypeWithPlayer());
}

void requestEndDemoAppearByBazookaElectric(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoAppearByBazookaElectric, demoTypeWithPlayer());
}

bool requestStartDemoLaunchBazookaElectric(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoLaunchBazookaElectric, demoTypeNormal());
}

void requestEndDemoLaunchBazookaElectric(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoLaunchBazookaElectric, demoTypeNormal());
}

bool requestStartDemoSceneStartPlayerWalk(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoSceneStartPlayerWalk,
                                 demoTypeSceneStartPlayerWalk());
}

void requestEndDemoSceneStartPlayerWalk(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoSceneStartPlayerWalk, demoTypeSceneStartPlayerWalk());
}

bool requestStartDemoGetLifeMaxUpItem(al::LiveActor* actor) {
    if (!getDirector(actor)->requestStartDemo(cDemoGetLifeMaxUpItem, demoTypeNormal()))
        return false;

    getDirector(actor)->setRequestActor(actor);
    return true;
}

void requestEndDemoGetLifeMaxUpItem(const al::Scene* scene) {
    requestEndDemoScene(scene, cDemoGetLifeMaxUpItem, demoTypeNormal());
}

bool requestStartDemoBirdCarryMeat(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoBirdCarryMeat, demoTypeNormalWithCinemaFrame());
}

void requestEndDemoBirdCarryMeat(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoBirdCarryMeat, demoTypeNormal());
}

bool requestStartDemoBirdFindMeat(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoBirdFindMeat, demoTypeNormal());
}

void requestEndDemoBirdFindMeat(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoBirdFindMeat, demoTypeNormal());
}

bool requestStartDemoGetTimeBalloon(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoGetTimeBalloon, demoTypeWithPlayer());
}

void requestEndDemoGetTimeBalloon(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoGetTimeBalloon, demoTypeWithPlayer());
}

bool requestStartDemoJango(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoJango, demoTypeWithPlayer());
}

void requestEndDemoJango(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoJango, demoTypeWithPlayer());
}

bool requestStartDemoTitleLogo(const al::LiveActor* actor) {
    return getDirector(actor)->requestStartDemo(cDemoTitleLogo, demoTypeNormal());
}

bool requestStartDemoGoToEnding(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoGoToEnding, demoTypeNormal());
}

bool isActiveDemo(const al::LiveActor* actor) {
    return static_cast<al::DemoDirector*>(getDirector(actor))->isActiveDemo();
}

bool isActiveDemo(const al::Scene* scene) {
    return static_cast<al::DemoDirector*>(getDirector(scene))->isActiveDemo();
}

bool isActiveDemoNormal(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoNormal);
}

bool isActiveDemoNormalWithCinemaFrame(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoNormalWithCinemaFrame);
}

bool isActiveDemoWithPlayer(const al::Scene* scene) {
    return getDirector(scene)->isActiveDemoWithPlayer() ||
           getDirector(scene)->isActiveDemo(cDemoKeepHackTalk);
}

bool isActiveDemoWithPlayer(const al::LiveActor* actor) {
    return getDirector(actor)->isActiveDemoWithPlayer() ||
           getDirector(actor)->isActiveDemo(cDemoKeepHackTalk);
}

bool isActiveDemoWithPlayerCinemaFrame(const al::Scene* scene) {
    return (isActiveDemoScene(scene, cDemoWithPlayerCinemaFrame)) ||
           isActiveDemoScene(scene, cDemoWithPlayerCinemaFrameTalk);
}

bool isActiveDemoWithPlayerCinemaFrame(const al::LiveActor* actor) {
    return (isActiveDemoActor(actor, cDemoWithPlayerCinemaFrame)) ||
           isActiveDemoActor(actor, cDemoWithPlayerCinemaFrameTalk);
}

bool isActiveDemoWithPlayerKeepCarry(const al::Scene* scene) {
    return (isActiveDemoScene(scene, cDemoWithPlayerKeepCarry)) ||
           isActiveDemoScene(scene, cDemoWithPlayerKeepCarryTalk);
}

bool isActiveDemoWithPlayerKeepCarry(const al::LiveActor* actor) {
    return (isActiveDemoActor(actor, cDemoWithPlayerKeepCarry)) ||
           isActiveDemoActor(actor, cDemoWithPlayerKeepCarryTalk);
}

bool isActiveDemoWithPlayerUseCoin(const al::Scene* scene) {
    return (isActiveDemoScene(scene, cDemoWithPlayerUseCoin)) ||
           isActiveDemoScene(scene, cDemoWithPlayerUseCoinTalk);
}

bool isActiveDemoWithPlayerUseCoin(const al::LiveActor* actor) {
    return (isActiveDemoActor(actor, cDemoWithPlayerUseCoin)) ||
           isActiveDemoActor(actor, cDemoWithPlayerUseCoinTalk);
}

bool isActiveDemoWithPlayerKeepBindTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoWithPlayerKeepBindTalk);
}

bool isActiveDemoWithPlayerKeepBindTalk(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, cDemoWithPlayerKeepBindTalk);
}

bool isActiveDemoWarpToCheckpoint(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, cDemoWarpToCheckpoint);
}

bool isActiveDemoShineGet(const al::Scene* scene) {
    if (isActiveDemoScene(scene, cDemoShineGet))
        return true;
    return isActiveDemoScene(scene, cDemoShineDotGet);
}

bool isActiveDemoShineGet(const al::LiveActor* actor) {
    if (isActiveDemoActor(actor, cDemoShineGet))
        return true;
    return isActiveDemoActor(actor, cDemoShineDotGet);
}

bool isActiveDemoShineDotGet(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoShineDotGet);
}

bool isActiveDemoShineDotGet(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, cDemoShineDotGet);
}

bool isActiveDemoShineGrandGet(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoShineGrandGet);
}

bool isActiveDemoShineGrandGet(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, cDemoShineGrandGet);
}

bool isActiveDemoShineMainGet(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoShineMainGet);
}

bool isActiveDemoShineMainGet(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, cDemoShineMainGet);
}

bool isActiveDemoWarp(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoWarp);
}

bool isActiveDemoWarp(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, cDemoWarp);
}

bool isActiveDemoHackStart(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, cDemoHackStart);
}

bool isActiveDemoHackStart(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoHackStart);
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
    return isActiveDemoActor(actor, cDemoScenarioCamera);
}

bool isActiveDemoGetLifeMaxUpItem(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoGetLifeMaxUpItem);
}

bool isActiveDemoSceneStartPlayerWalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoSceneStartPlayerWalk);
}

bool isActiveDemoBirdCarryMeat(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoBirdCarryMeat);
}

bool isActiveDemoBirdFindMeat(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoBirdFindMeat);
}

bool isActiveDemoJango(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoJango);
}

bool isActiveDemoJango(const al::LiveActor* actor) {
    return isActiveDemoActor(actor, cDemoJango);
}

bool isActiveDemoTitleLogo(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoTitleLogo);
}

bool isActiveDemoGoToEnding(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoGoToEnding);
}

bool isActiveDemoTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoWithPlayerCinemaFrameTalk) ||
           isActiveDemoScene(scene, cDemoWithPlayerKeepCarryTalk) ||
           isActiveDemoScene(scene, cDemoKeepHackTalk) ||
           isActiveDemoScene(scene, cDemoNormalTalk) ||
           isActiveDemoScene(scene, cDemoWithPlayerKeepBindTalk);
}

bool isActiveDemoCinemaFrameTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoWithPlayerCinemaFrameTalk);
}

bool isActiveDemoKeepCarryTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoWithPlayerKeepCarryTalk);
}

bool isActiveDemoUseCoinTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoWithPlayerUseCoinTalk);
}

bool isActiveDemoKeepHackTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoKeepHackTalk);
}

bool isActiveDemoNormalTalk(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoNormalTalk);
}

bool isActiveDemoLaunchBazookaElectric(const al::Scene* scene) {
    return isActiveDemoScene(scene, cDemoLaunchBazookaElectric);
}

bool isActiveDemoGiantWanderBossBattleStart(const al::Scene* scene) {
    return isEqualDemoInfoDemoName(scene, DemoName::cGiantWanderBossBattleStart);
}

bool isActiveDemoGiantWanderBossBattleStart(const OceanWaveActor* actor) {
    const al::LiveActor* liveActor = reinterpret_cast<const al::LiveActor*>(actor);
    const char* demoName = getDirector(liveActor)->getDemoInfoDemoName();
    if (demoName == nullptr)
        return false;
    return al::isEqualString(demoName, DemoName::cGiantWanderBossBattleStart);
}

bool isActiveDemoPlayerPuppetable(const al::LiveActor* actor) {
    if (getDirector(actor)->isActiveDemoWithPlayer())
        return true;
    if (isActiveDemoActor(actor, cDemoKeepHackTalk))
        return true;
    if (isActiveDemoActor(actor, cDemoWithPlayerCinemaFrame))
        return true;
    if (isActiveDemoActor(actor, cDemoWithPlayerCinemaFrameTalk))
        return true;
    if (isActiveDemoActor(actor, cDemoWithPlayerKeepCarry))
        return true;
    if (isActiveDemoActor(actor, cDemoWithPlayerKeepCarryTalk))
        return true;
    if (isActiveDemoActor(actor, cDemoWithPlayerUseCoin))
        return true;
    if (isActiveDemoActor(actor, cDemoWithPlayerUseCoinTalk))
        return true;
    if (isActiveDemoActor(actor, cDemoWithPlayerKeepBindTalk))
        return true;
    if (isActiveDemoActor(actor, cDemoShineGet))
        return true;
    if (isActiveDemoActor(actor, cDemoShineDotGet))
        return true;
    if (isActiveDemoActor(actor, cDemoShineGrandGet))
        return true;
    if (isActiveDemoActor(actor, cDemoShineMainGet))
        return true;
    if (isActiveDemoActor(actor, cDemoWarp))
        return true;
    if (isActiveDemoActor(actor, cDemoHackStart))
        return true;
    if (isActiveDemoActor(actor, cDemoAppearFromHome))
        return true;
    return isActiveDemoActor(actor, cDemoReturnToHome);
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
    return requestStartDemoScene(scene, cDemoWorldIntroCamera, demoTypeWithPlayer());
}

void requestEndDemoWorldIntroCamera(const al::Scene* scene) {
    getDirector(scene)->requestEndDemo(cDemoWorldIntroCamera, demoTypeWithPlayer());
    GameDataHolderAccessor skipAccessor(scene);
    skipAccessor->getGameDataFile()->getSequenceDemoSkipData()->showWorldIntroCamera(
        GameDataFunction::getCurrentWorldIdNoDevelop(scene));
}

bool requestStartDemoScenarioCamera(const al::Scene* scene) {
    return getDirector(scene)->requestStartDemoScenarioCamera(cDemoScenarioCamera);
}

void requestEndDemoScenarioCamera(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoScenarioCamera(cDemoScenarioCamera);
    GameDataHolderAccessor skipAccessor(actor);
    skipAccessor->getGameDataFile()->getSequenceDemoSkipData()->showScenarioStartCamera(
        GameDataFunction::getCurrentWorldIdNoDevelop(actor), getActiveQuestNo(actor));
}

bool requestStartDemoAppearFromHome(const al::Scene* scene) {
    return getDirector(scene)->requestStartDemoAppearFromHome(cDemoAppearFromHome);
}

void requestEndDemoAppearFromHome(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoAppearFromHome(cDemoAppearFromHome);
}

bool requestStartDemoReturnToHome(const al::Scene* scene) {
    return getDirector(scene)->requestStartDemoReturnToHome(cDemoReturnToHome);
}

void requestEndDemoReturnToHome(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoReturnToHome(cDemoReturnToHome);
}

bool requestStartDemoRiseMapParts(const al::Scene* scene) {
    return getDirector(scene)->requestStartDemoRiseMapParts(cDemoRiseMapParts);
}

void requestEndDemoRiseMapParts(const al::LiveActor* actor) {
    getDirector(actor)->requestEndDemoRiseMapParts(cDemoRiseMapParts);
}

bool requestStartDemoPeachCastleCap(al::LiveActor* actor) {
    return requestStartDemoActor(actor, false, cDemoPeachCastleCap, demoTypeWithPlayer());
}

void requestEndDemoPeachCastleCap(const al::LiveActor* actor) {
    requestEndDemoActor(actor, cDemoPeachCastleCap, demoTypeWithPlayer());
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
    return getDirector(scene)->getDemoSkipRequester() != nullptr;
}

bool isFirstDemo(const al::Scene* scene) {
    return getDirector(scene)->getDemoSkipRequester()->isFirstDemo();
}

bool isEnableSkipDemo(const al::Scene* scene) {
    return getDirector(scene)->getDemoSkipRequester()->isEnableSkipDemo();
}

void skipDemo(const al::Scene* scene) {
    getDirector(scene)->getDemoSkipRequester()->skipDemo();
}

void updateOnlyDemoGraphics(const al::Scene* scene) {
    getDirector(scene)->getDemoSkipRequester()->updateOnlyDemoGraphics();
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
    if (!getDirector(scene)->isActiveDemoWithCinemaCaption())
        return false;

    return cinemaCaption->tryAppear(getDirector(scene)->getCaptionInfoHolder());
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
