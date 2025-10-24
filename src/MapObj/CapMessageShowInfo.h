#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
class LiveActor;
}  // namespace al
class CapMessageEnableChecker;
class StageSceneStateGetShine;

class CapMessageShowInfo {
public:
    CapMessageShowInfo();
    CapMessageShowInfo(const char*, const char*, s32, s32, bool, bool);
    void set(const char*, const char*, s32, s32, bool, bool);
    void setReplace(const char*, s32);
    s32 getDelayTime() const;
    s32 getWaitTime() const;
    const char* getMstxtName() const;
    const char* getLabelName() const;
    bool isStageMessage() const;
    bool isContinueDemo() const;
    const char* getReplaceTagName() const;
    s32 getReplaceNumber() const;

private:
    const char* mMstxtName;
    const char* mLabelName;
    s32 mDelayTime;
    s32 mWaitTime;
    bool mIsStageMessage;
    const char* mReplaceTagName;
    s32 mReplaceNumber;
    bool mIsContinueDemo;
};

namespace rs {
bool tryCancelCapMessage(const al::IUseSceneObjHolder*, const char*);
void showCapMessage(const al::IUseSceneObjHolder*, const char*, s32, s32);
void showCapMessageDemo(const al::IUseSceneObjHolder*, const char*, s32, s32);
void showCapMessageBossHint(const al::IUseSceneObjHolder*, const char*, s32, s32);
void showCapMessageBossDamage(const al::IUseSceneObjHolder*, const char*, s32, s32);
void showCapMessageMinigame(const al::IUseSceneObjHolder*, const char*, s32, s32);
void showCapMessageJango(const al::IUseSceneObjHolder*, const char*, s32, s32);
void showCapMessageWanderBossBattle(const al::IUseSceneObjHolder*, const char*, s32, s32);
void showCapMessageEnterExStage(const al::IUseSceneObjHolder*, const char*, s32, s32);
bool tryShowCapMessagePriorityLow(const al::IUseSceneObjHolder*, const char*, s32, s32);
bool tryShowCapMessage(const al::IUseSceneObjHolder*, const char*, s32, s32);
bool isActiveCapMessage(const al::IUseSceneObjHolder*);
bool tryShowCapMessageFromStageMsg(const al::IUseSceneObjHolder*, const char*, s32, s32);
bool tryShowCapMessageFromCurrentStageMsg(const al::IUseSceneObjHolder*, const char*, s32, s32);
bool isActiveCapMessageLabel(const al::IUseSceneObjHolder*, const char*);
bool isDelayCapMessage(const al::IUseSceneObjHolder*);
bool isDelayCapMessageLabel(const al::IUseSceneObjHolder*, const char*);
void showCapMessageGetMoonSpecial(const al::IUseSceneObjHolder*, const char*, s32);
void requestShowCapMessageGetMoon(const al::IUseSceneObjHolder*);
void validateAppearCapMessage(const al::IUseSceneObjHolder*);
void invalidateAppearCapMessage(const al::IUseSceneObjHolder*);
void endForceCapMessage(const al::IUseSceneObjHolder*);
bool tryStartCapMessageContinue(const al::IUseSceneObjHolder*, const char*);
void endCapMessageContinue(const al::IUseSceneObjHolder*);
void createCapMessageEnableChecker(CapMessageEnableChecker**, al::LiveActor*,
                                   const al::ActorInitInfo&);
bool isShowCapMsgGrowFlowerPotLookFirst(const al::IUseSceneObjHolder*);
bool isShowCapMsgPlayerInIceWaterFirst(const al::IUseSceneObjHolder*);
bool isShowCapMsgPackunFireChokeCap(const al::IUseSceneObjHolder*);
bool tryShowCapMsgPlayerInIceWaterFirst(const al::IUseSceneObjHolder*);
bool tryShowCapMsgPlayerSinkSandFirst(const al::IUseSceneObjHolder*);
bool tryShowCapMsgPackunFireChokeCap(const al::IUseSceneObjHolder*);
bool tryShowCapMsgPackunFireRescueCap(const al::IUseSceneObjHolder*);
bool tryShowCapMsgCatchBombCatchFirst(const al::IUseSceneObjHolder*);
bool tryShowCapMsgCollectCoinGetFirst(const al::IUseSceneObjHolder*);
bool tryShowCapMsgCorkNearFirst(const al::IUseSceneObjHolder*);
bool tryShowCapMsgCorkNearLauncherFirst(const al::IUseSceneObjHolder*);
bool tryShowCapMsgCorkNearSecond(const al::IUseSceneObjHolder*);
bool tryCheckShowCapMsgWorldWarpCloseLookFirst(const al::IUseSceneObjHolder*,
                                               CapMessageEnableChecker*);
bool tryCheckShowCapMsgWorldWarpLook(const al::IUseSceneObjHolder*, CapMessageEnableChecker*);
bool tryCheckShowCapMsgFlowerPotLookFirst(const al::IUseSceneObjHolder*, CapMessageEnableChecker*);
bool tryCheckShowCapMsgFlowerPotAfterImplant(const al::IUseSceneObjHolder*,
                                             CapMessageEnableChecker*);
bool tryCheckShowCapMsgWorldWarpOut(const al::IUseSceneObjHolder*);
bool tryCheckShowCapMsgShineChipGetFirst(const al::IUseSceneObjHolder*);
bool tryShowCapMsgKakkuHideFirst(const al::IUseSceneObjHolder*);
bool tryCheckShowCapMsgMoonRockLook(const al::IUseSceneObjHolder*, CapMessageEnableChecker*);
bool tryCheckShowCapMsgMoonRockLookLongTime(const al::IUseSceneObjHolder*,
                                            CapMessageEnableChecker*);
bool tryCheckShowCapMsgCapCatapultLookFirst(const al::IUseSceneObjHolder*);
bool tryShowCapMsgCapCatapultAfterShootFirst(const al::IUseSceneObjHolder*);
bool tryCheckShowCapMsgGrowFlowerSeedLookFirst(const al::IUseSceneObjHolder*);
bool tryShowCapMsgMoonGetSpecial1(const al::IUseSceneObjHolder*);
bool tryShowCapMsgMoonGetSpecial2(const al::IUseSceneObjHolder*);
bool tryShowCapMsgIntroducePowerStar(const al::IUseSceneObjHolder*, const StageSceneStateGetShine*);
bool tryShowCapMsgWarpDisableInMiniGameDokan(const al::IUseSceneObjHolder*);
bool tryShowCapMsgWarpDisableInMiniGameDoor(const al::IUseSceneObjHolder*);
bool tryShowCapMsgWarpDisableInMiniGameDoorCap(const al::IUseSceneObjHolder*);
bool tryShowCapMsgWarpDisableInMiniGameArea(const al::IUseSceneObjHolder*);
void onShowCapMsgGrowFlowerPotLookFirst(const al::IUseSceneObjHolder*);
void onShowCapMsgWorldWarpEnterAlready(const al::IUseSceneObjHolder*);
bool tryShowCapMsgWarningBikeOn(const al::IUseSceneObjHolder*);
bool tryShowCapMsgWarningBikeSideWalkIn(const al::IUseSceneObjHolder*);
bool tryShowCapMsgWarningBikeReverseRun(const al::IUseSceneObjHolder*);
bool tryShowCapMsgWarningPrankNpc(const al::IUseSceneObjHolder*);
void showCapMsgPlayerFallDead(const al::IUseSceneObjHolder*, s32);
void showCapMsgPlayerLife1(const al::IUseSceneObjHolder*, s32);
bool isShowCapMsgPlayerFallDead(const al::IUseSceneObjHolder*, s32);
void saveCapMsgCapThrow(const al::IUseSceneObjHolder*);
void saveCapMsgWorldWarpCloseLookFirst(const al::IUseSceneObjHolder*);
void saveCapMsgWorldWarpLookFirst(const al::IUseSceneObjHolder*);
bool isEnableShowCapMsgCapThrow(const al::IUseSceneObjHolder*);
bool isShowCapMsgPlayerLife1(const al::IUseSceneObjHolder*);
}  // namespace rs

static_assert(sizeof(CapMessageShowInfo) == 0x30);
