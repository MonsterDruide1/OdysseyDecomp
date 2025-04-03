#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadEnum.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
class HitSensor;
class SensorMsg;
}  // namespace al
class Shine;

namespace rs {
// TODO: Replace this with SEAD_ENUM_EX when its ValueArray constructor matches
SEAD_ENUM(ItemType, None = -1, Coin, Coin2D, CoinBlow, CoinBlowVeryLittle, CoinPopUp, 
CoinPopUpWithoutHitReaction, Coin3, Coin5, Coin10, Coin10Auto, Coin5Count, Coin100, LifeUpItem, 
LifeUpItemBack, LifeUpItem2D, LifeMaxUpItem, LifeMaxUpItem2D, Shine, AirBubble, DotMarioCat, 
KuriboMini3, KuriboMini8, CoinStackBound, Random);

ItemType::ValueType getItemType(const al::ActorInitInfo& info);
ItemType::ValueType getItemType(const char* name);
bool isItemTypeKuriboMini(s32* out, s32 type);

void initItemByPlacementInfo(al::LiveActor* actor, const al::ActorInitInfo& info,
                             bool isAppearAbove);
bool tryInitItemByPlacementInfo(al::LiveActor* actor, const al::ActorInitInfo& info,
                                bool isAppearAbove);
void initItem2DByPlacementInfo(al::LiveActor* actor, const al::ActorInitInfo& info);
bool tryInitItem2DByPlacementInfo(al::LiveActor* actor, const al::ActorInitInfo& info);
bool tryInitItem(al::LiveActor* actor, s32 itemType, const al::ActorInitInfo& info,
                 bool isAppearAbove);

Shine* tryInitShineByPlacementInfoWithItemMenu(const al::ActorInitInfo& info);
Shine* initShineByPlacementInfo(const al::ActorInitInfo& info);
Shine* tryInitLinkShine(const al::ActorInitInfo& info, const char* name, s32 linkIndex);
Shine* tryInitLinkShineHintPhoto(const al::ActorInitInfo& info, const char* name, s32 linkIndex);
Shine* initLinkShine(const al::ActorInitInfo& info, const char* name, s32 linkIndex);
Shine* initLinkShopShine(const al::ActorInitInfo& info, const char* name);
Shine* initLinkShineChipShine(const al::ActorInitInfo& info);
Shine* initLinkBossShine(const al::ActorInitInfo& info);

void createShineEffectInsideObject(Shine* shine, const al::LiveActor* actor,
                                   const al::ActorInitInfo& info);

void appearPopupShine(Shine* shine);
void appearPopupShine(Shine* shine, const al::LiveActor* actor);
void appearPopupShine(Shine* shine, const sead::Vector3f& trans);
void appearPopupShineWithoutDemo(Shine* shine);
void appearPopupShineWithoutDemo(Shine* shine, const al::LiveActor* actor);
void appearPopupShineWithoutDemo(Shine* shine, const sead::Vector3f& trans);
void appearPopupShineWithoutWarp(Shine* shine);
void appearPopupShineGrandByBoss(Shine* shine, s32 a2);
void appearWaitShine(Shine* shine);
void appearWaitShine(Shine* shine, const sead::Vector3f& trans);

bool isEndAppearShine(const Shine* shine);
bool isGotShine(const Shine* shine);
bool isAliveShine(const Shine* shine);
bool isMainShine(const Shine* shine);

void updateHintTrans(const Shine* shine, const sead::Vector3f& trans);
void appearShineAndJoinBossDemo(Shine* shine, const char* name, const sead::Quatf& quat,
                                const sead::Vector3f& trans);
void endShineBossDemo(Shine* shine);
void endShineBossDemoAndStartFall(Shine* shine, f32 a2);

void setAppearItemFactorByMsg(const al::LiveActor* actor, const al::SensorMsg* msg,
                              const al::HitSensor* sensor);
void setAppearItemFactorAndOffsetByMsg(const al::LiveActor* actor, const al::SensorMsg* msg,
                                       const al::HitSensor* sensor);
void setAppearItemFactorAndOffsetForCombo(const al::LiveActor* actor, const al::SensorMsg* msg,
                                          const al::HitSensor* sensor, bool isSuperCombo);
void appearItemFromObj(al::LiveActor* actor, const sead::Vector3f& trans, const sead::Quatf& quat,
                       f32 offset);
void appearItemFromObj(al::LiveActor* actor, al::HitSensor* sensor, f32 offset);
void appearItemFromObj(al::LiveActor* actor, al::HitSensor* sensor, const sead::Vector3f& offset);
void appearItemFromObjGravity(al::LiveActor* actor, al::HitSensor* sensor,
                              const sead::Vector3f& offset);
void appearRandomItemFromObj(al::LiveActor* actor, al::HitSensor* sensor, f32 offset);
bool tryAppearMultiCoinFromObj(al::LiveActor* actor, const sead::Vector3f& trans, s32 step,
                               f32 offsetAbove);
bool tryAppearMultiCoinFromObj(al::LiveActor* actor, const sead::Vector3f& trans,
                               const sead::Quatf& quat, s32 step, f32 offsetAbove);
bool tryAppearMultiCoinFromObj(al::LiveActor* actor, al::HitSensor* sensor, s32 step,
                               f32 offsetAbove);
bool tryAppearMultiCoinFromObj(al::LiveActor* actor, al::HitSensor* sensor, s32 step,
                               const sead::Vector3f& offset);

void syncCoin2DAnimFrame(al::LiveActor* actor, const char* name);

const char* getStageCoinCollectArchiveName(const al::LiveActor* actor);
const char* getStageCoinCollectEmptyArchiveName(const al::LiveActor* actor);
const char* getStageCoinCollect2DArchiveName(const al::LiveActor* actor);
const char* getStageCoinCollect2DEmptyArchiveName(const al::LiveActor* actor);

s32 getStageShineAnimFrame(const al::LiveActor* actor);
s32 getStageShineAnimFrame(const al::LiveActor* actor, s32 worldId);
s32 getStageShineAnimFrame(const al::LiveActor* actor, const char* stageName);
void setStageShineAnimFrame(al::LiveActor* actor, const char* stageName, s32 shineAnimFrame,
                            bool isMatAnim);

const char* getStageShineArchiveName(const al::LiveActor* actor, const char* stageName);
const char* getStageShineEmptyArchiveName(const al::LiveActor* actor, const char* stageName);

void setMaterialAsEmptyModel(al::LiveActor* actor);
void setProjectionMtxAsEmptyModel2d(al::LiveActor* actor, const sead::Vector2f& vec);
void addDemoRacePrizeCoin(al::LiveActor* actor);

}  // namespace rs

namespace StageSceneFunction {
void appearPlayerDeadCoin(al::LiveActor* actor);
}
