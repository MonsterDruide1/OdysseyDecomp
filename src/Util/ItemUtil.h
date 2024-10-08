#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class ActorInitInfo;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class Shine;

namespace rs {
void getItemType(const al::ActorInitInfo&);
void getItemType(const char*);
bool isItemTypeKuriboMini(s32*, s32);
void initItemByPlacementInfo(al::LiveActor*, const al::ActorInitInfo&, bool);
void tryInitItemByPlacementInfo(al::LiveActor*, const al::ActorInitInfo&, bool);
void initItem2DByPlacementInfo(al::LiveActor*, const al::ActorInitInfo&);
void tryInitItem2DByPlacementInfo(al::LiveActor*, const al::ActorInitInfo&);
void tryInitItem(al::LiveActor*, s32, const al::ActorInitInfo&, bool);
void tryInitShineByPlacementInfoWithItemMenu(const al::ActorInitInfo&);
void initShineByPlacementInfo(const al::ActorInitInfo&);
Shine* tryInitLinkShine(const al::ActorInitInfo&, const char*, s32);
void tryInitLinkShineHintPhoto(const al::ActorInitInfo&, const char*, s32);
Shine* initLinkShine(const al::ActorInitInfo&, const char*, s32);
void initLinkShopShine(const al::ActorInitInfo&, const char*);
void initLinkShineChipShine(const al::ActorInitInfo&);
void initLinkBossShine(const al::ActorInitInfo&);
void createShineEffectInsideObject(Shine*, const al::LiveActor*, const al::ActorInitInfo&);
void appearPopupShine(Shine*);
void appearPopupShine(Shine*, const al::LiveActor*);
void appearPopupShine(Shine*, const sead::Vector3f&);
void appearPopupShineWithoutDemo(Shine*);
void appearPopupShineWithoutDemo(Shine*, const al::LiveActor*);
void appearPopupShineWithoutDemo(Shine*, const sead::Vector3f&);
void appearPopupShineWithoutWarp(Shine*);
void appearPopupShineGrandByBoss(Shine*, s32);
void appearWaitShine(Shine*);
void appearWaitShine(Shine*, const sead::Vector3f&);
bool isEndAppearShine(const Shine*);
bool isGotShine(const Shine*);
bool isAliveShine(const Shine*);
bool isMainShine(const Shine*);
void updateHintTrans(const Shine*, const sead::Vector3f&);
void appearShineAndJoinBossDemo(Shine*, const char*, const sead::Quat<f32>&, const sead::Vector3f&);
void endShineBossDemo(Shine*);
void endShineBossDemoAndStartFall(Shine*, f32);
void setAppearItemFactorByMsg(const al::LiveActor*, const al::SensorMsg*, const al::HitSensor*);
void setAppearItemFactorAndOffsetByMsg(const al::LiveActor*, const al::SensorMsg*,
                                       const al::HitSensor*);
void setAppearItemFactorAndOffsetForCombo(const al::LiveActor*, const al::SensorMsg*,
                                          const al::HitSensor*, bool);
void appearItemFromObj(al::LiveActor*, const sead::Vector3f&, const sead::Quatf&, f32);
void appearItemFromObj(al::LiveActor*, al::HitSensor*, f32);
void appearItemFromObj(al::LiveActor*, al::HitSensor*, const sead::Vector3f&);
void appearItemFromObjGravity(al::LiveActor*, al::HitSensor*, const sead::Vector3f&);
void appearRandomItemFromObj(al::LiveActor*, al::HitSensor*, f32);
void tryAppearMultiCoinFromObj(al::LiveActor*, const sead::Vector3f&, s32, f32);
void tryAppearMultiCoinFromObj(al::LiveActor*, const sead::Vector3f&, const sead::Quatf&, s32, f32);
void tryAppearMultiCoinFromObj(al::LiveActor*, al::HitSensor*, s32, f32);
void tryAppearMultiCoinFromObj(al::LiveActor*, al::HitSensor*, s32, const sead::Vector3f&);
void syncCoin2DAnimFrame(al::LiveActor*, const char*);
void getStageCoinCollectArchiveName(const al::LiveActor*);
void getStageCoinCollectEmptyArchiveName(const al::LiveActor*);
void getStageCoinCollect2DArchiveName(const al::LiveActor*);
void getStageCoinCollect2DEmptyArchiveName(const al::LiveActor*);
void getStageShineAnimFrame(const al::LiveActor*);
void getStageShineAnimFrame(const al::LiveActor*, s32);
void getStageShineAnimFrame(const al::LiveActor*, const char*);
void setStageShineAnimFrame(al::LiveActor*, const char*, s32, bool);
void getStageShineArchiveName(const al::LiveActor*, const char*);
void getStageShineEmptyArchiveName(const al::LiveActor*, const char*);
void setMaterialAsEmptyModel(al::LiveActor*);
void setProjectionMtxAsEmptyModel2d(al::LiveActor*, const sead::Vector2f&);
void addDemoRacePrizeCoin(al::LiveActor*);
}  // namespace rs

namespace StageSceneFunction {
void appearPlayerDeadCoin(al::LiveActor*);
}
