#include "Util/AchievementUtil.h"

#include <container/seadBuffer.h>

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Message/MessageHolder.h"

#include "Layout/ShopLayoutInfo.h"
#include "Npc/AchievementHolder.h"
#include "Npc/AchievementInfoReader.h"
#include "System/AchievementSaveData.h"
#include "System/GameDataFile.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolder.h"
#include "System/GameDataUtil.h"
#include "System/WorldList.h"
#include "Util/ClothUtil.h"

class AchievementNpc : public al::LiveActor {};

namespace {

const sead::Vector3f cAchievementTrans{};

GameDataHolderAccessor makeAccessor(const GameDataHolder* holder) {
    return GameDataHolderAccessor(const_cast<GameDataHolder*>(holder));
}

GameDataFile* getGameDataFile(const al::LiveActor* actor) {
    return static_cast<GameDataHolder*>(actor->getSceneInfo()->gameDataHolder)->getGameDataFile();
}

}  // namespace

namespace rs {

const char16* getAchievementName(const al::IUseMessageSystem* user, const char* label) {
    if (al::isExistLabelInSystemMessage(user, "AchievementName", label))
        return al::getSystemMessageString(user, "AchievementName", label);
    return nullptr;
}

const char16* getAchievementName(const al::LayoutActor* layout, s32 achievementIdx) {
    const al::IUseMessageSystem* messageUser = layout;
    GameDataHolderAccessor accessor(layout);
    const char* label = accessor->getAchievementInfoReader()->get(achievementIdx)->name;

    if (al::isExistLabelInSystemMessage(messageUser, "AchievementName", label))
        return al::getSystemMessageString(messageUser, "AchievementName", label);
    return nullptr;
}

const char16* getAchievementHint(const al::IUseMessageSystem* user, const char* label) {
    if (al::isExistLabelInSystemMessage(user, "AchievementHint", label))
        return al::getSystemMessageString(user, "AchievementHint", label);
    return nullptr;
}

const char16* getAchievementHint(const al::LayoutActor* layout, s32 achievementIdx) {
    const al::IUseMessageSystem* messageUser = layout;
    GameDataHolderAccessor accessor(layout);
    const char* label = accessor->getAchievementInfoReader()->get(achievementIdx)->name;

    if (al::isExistLabelInSystemMessage(messageUser, "AchievementHint", label))
        return al::getSystemMessageString(messageUser, "AchievementHint", label);
    return nullptr;
}

AchievementHolder* getAchievementHolder(const al::LayoutActor* layout) {
    return GameDataHolderAccessor(layout)->getAchievementHolder();
}

void unlockAchievementShineName(GameDataHolderAccessor accessor) {
    accessor->getGameDataFile()->unlockAchievementShineName();
}

bool isUnlockAchievementShineName(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isUnlockAchievementShineName();
}

bool checkGetAchievement(GameDataHolderAccessor accessor, const char* label) {
    return accessor->getGameDataFile()->getAchievementSaveData()->checkGetAchievement(label);
}

bool checkGetAchievement(const al::LayoutActor* layout, s32 achievementIdx) {
    GameDataHolderAccessor accessor(layout);
    return accessor->getGameDataFile()->getAchievementSaveData()->checkGetAchievement(
        achievementIdx);
}

u64 findAchievementGetTime(const al::LayoutActor* layout, s32 achievementIdx) {
    GameDataHolderAccessor accessor(layout);
    return accessor->getGameDataFile()->getAchievementSaveData()->findAchievementGetTime(
        achievementIdx);
}

u64 findAchievementGetTime(const al::LayoutActor* layout, const char* label) {
    GameDataHolderAccessor accessor(layout);
    AchievementSaveData* saveData = accessor->getGameDataFile()->getAchievementSaveData();
    return saveData->findAchievementGetTime(saveData->findAchievementIndex(label));
}

const sead::Vector3f* findAchievementTrans(const al::LayoutActor*, s32) {
    return &cAchievementTrans;
}

s32 getAchievementNum(const al::LayoutActor* layout) {
    return GameDataHolderAccessor(layout)->getAchievementInfoReader()->size();
}

s32 calcReceivedAchievementNum(const GameDataHolder* holder) {
    AchievementInfoReader* reader = holder->getAchievementInfoReader();
    s32 achievementNum = reader->size();
    if (achievementNum < 1)
        return 0;

    s32 count = 0;
    for (s32 i = 0; i != achievementNum; i++) {
        const char* name = reader->get(i)->name;
        count += holder->getGameDataFile()->getAchievementSaveData()->checkGetAchievement(name);
    }

    return count;
}

s32 getTotalCoinNum(const GameDataHolder* holder) {
    return holder->getGameDataFile()->getTotalCoinNum();
}

s32 calcWorldWarpHoleThroughNum(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcWorldWarpHoleThroughNum();
}

s32 getWorldWarpHoleThroughNumMax(const GameDataHolder* holder) {
    return holder->getGameDataFile()->getWorldWarpHoleThroughNumMax();
}

s32 calcGetCheckpointNum(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetCheckpointNum();
}

s32 calcBuyItemNumForCoinCollectByWorld(const GameDataHolder* holder) {
    GameDataHolderAccessor accessor = makeAccessor(holder);
    const sead::PtrArray<ShopItem::ShopItemInfo>& shopItemInfoList =
        GameDataFunction::getShopItemInfoList(accessor);
    s32 worldNum = holder->getWorldList()->getWorldNum();
    u32 worldCount = static_cast<u32>(worldNum);

    sead::Buffer<bool> isBoughtWorldBuffer(worldCount,
                                           static_cast<bool*>(__builtin_alloca(worldCount)));
    bool* isBoughtWorld = isBoughtWorldBuffer.getBufferPtr();

    if (worldNum > 0)
        isBoughtWorldBuffer.fill(false);

    if (shopItemInfoList.size() < 1)
        return 0;

    s32 count = 0;
    for (s32 i = 0; i < shopItemInfoList.size(); i++) {
        if (shopItemInfoList[i]->coinType != ShopItem::CoinType::Collect)
            continue;

        if (!isBuyItem(accessor, &shopItemInfoList[i]->info))
            continue;

        s32 worldIndex =
            holder->getWorldList()->tryFindWorldIndexByDevelopName(shopItemInfoList[i]->storeName);
        if (worldIndex >= 0 && !isBoughtWorld[worldIndex]) {
            count++;
            isBoughtWorld[worldIndex] = true;
        }
    }

    return count;
}

s32 calcClearWorldNum(const GameDataHolder* holder) {
    WorldList* worldList = holder->getWorldList();
    s32 count = 0;

    for (s32 i = 0; i != 14; i++) {
        s32 scenarioNo = holder->getGameDataFile()->getScenarioNo(i);
        if (worldList->isEqualClearMainScenarioNo(i, scenarioNo) ||
            worldList->isEqualAfterEndingScenarioNo(i, scenarioNo) ||
            worldList->isEqualMoonRockScenarioNo(i, scenarioNo))
            count++;
    }

    return count;
}

void addPlayerJumpCount(const al::LiveActor* actor) {
    getGameDataFile(actor)->addPlayerJumpCount();
}

s32 getPlayerJumpCount(const GameDataHolder* holder) {
    return holder->getGameDataFile()->getPlayerJumpCount();
}

void addPlayerThrowCapCount(const al::LiveActor* actor) {
    getGameDataFile(actor)->addPlayerThrowCapCount();
}

s32 getPlayerThrowCapCount(const GameDataHolder* holder) {
    return holder->getGameDataFile()->getPlayerThrowCapCount();
}

bool checkGetCoinCollectAllInWorld(const al::LiveActor* actor, s32 worldId) {
    const al::IUseSceneObjHolder* user = actor;
    s32 max = GameDataFunction::getCoinCollectNumMax(GameDataHolderAccessor(user), worldId);
    return max <= GameDataFunction::getCoinCollectGotNum(GameDataHolderAccessor(user), worldId);
}

bool checkGetCoinCollectHalfInWorld(const AchievementNpc* achievementNpc, s32 worldId) {
    const al::IUseSceneObjHolder* user = achievementNpc;
    s32 max = GameDataFunction::getCoinCollectNumMax(GameDataHolderAccessor(user), worldId);
    s32 half = max / 2;
    return half <= GameDataFunction::getCoinCollectGotNum(GameDataHolderAccessor(user), worldId);
}

bool checkGetShineAllInWorld(const AchievementNpc* achievementNpc, s32 worldId) {
    return GameDataFunction::calcIsGetShineAllInWorld(GameDataHolderAccessor(achievementNpc),
                                                      worldId);
}

bool checkGetClothAndCapNum(const AchievementNpc* achievementNpc, s32 num) {
    const al::IUseSceneObjHolder* user = achievementNpc;
    s32 clothNum = calcHaveClothNum(GameDataHolderAccessor(user));
    return calcHaveCapNum(GameDataHolderAccessor(user)) + clothNum >= num;
}

bool checkUnlockMoonRockAllWorld(GameDataHolderAccessor accessor) {
    if (GameDataFunction::getWorldNum(accessor) < 1)
        return true;

    s32 worldId = 0;
    while (accessor->getGameDataFile()->isOpenMoonRock(worldId)) {
        worldId++;
        if (worldId >= GameDataFunction::getWorldNum(accessor))
            return true;
    }

    return false;
}

s32 calcUnlockMoonRockNum(GameDataHolderAccessor accessor) {
    if (GameDataFunction::getWorldNum(accessor) < 1)
        return 0;

    s32 count = 0;
    for (s32 i = 0;; i++) {
        count += accessor->getGameDataFile()->isOpenMoonRock(i);
        if (i + 1 >= GameDataFunction::getWorldNum(accessor))
            break;
    }

    return count;
}

s32 calcHackObjNum(GameDataHolderAccessor accessor) {
    const sead::PtrArray<HackObjInfo>& hackObjList = accessor->getHackObjList();
    if (hackObjList.size() < 1)
        return 0;

    s32 count = 0;
    for (s32 i = 0; i < hackObjList.size(); i++)
        count += accessor->getGameDataFile()->isExistInHackDictionary(hackObjList[i]->hackName);

    return count;
}

s32 calcSphinxQuizCompleteNum(GameDataHolderAccessor accessor) {
    s32 worldNum = GameDataFunction::getWorldNum(accessor);
    if (worldNum < 1)
        return 0;

    s32 count = 0;
    for (s32 i = 0; i != worldNum; i++) {
        const char* worldName = GameDataFunction::getWorldDevelopName(accessor, i);
        if (al::isEqualString("Forest", worldName))
            count += isAnswerCorrectSphinxQuiz(accessor, worldName);
        else
            count += isAnswerCorrectSphinxQuizAll(accessor, worldName);
    }

    return count;
}

s32 calcGetShineNumDot(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "_2D", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumTreasureBox(const GameDataHolder* holder) {
    s32 count = holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "宝箱", GameDataFile::CountType::Value_2);
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
               "順序宝箱", GameDataFile::CountType::Value_2) +
           count;
}

s32 calcGetShineNumNoteObj(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "音符オブジェ", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumTimerAthletic(const GameDataHolder* holder) {
    s32 count = holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "タイマー", GameDataFile::CountType::Value_1);
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
               "TimerAthleticShine", GameDataFile::CountType::Value_1) +
           count;
}

s32 calcGetShineNumKinopioBrigade(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "キノピオ隊長", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumWorldTravelingPeach(const GameDataHolder* holder) {
    GameDataHolderAccessor accessor = makeAccessor(holder);
    s32 worldNum = GameDataFunction::getWorldNum(accessor);
    if (worldNum < 1)
        return 0;

    s32 i = 0;
    s32 count = 0;
    while (i != worldNum) {
        const char* worldName = GameDataFunction::getWorldDevelopName(accessor, i);
        count += checkGetShineForWorldTravelingPeach(accessor, worldName);
        i++;
    }

    return count;
}

s32 calcGetShineNumCollectAnimal(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "コレクトアニマル監視者", GameDataFile::CountType::Value_1);
}

s32 calcAllShineNumCollectAnimal(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcAllShineNumByObjectNameOrOptionalId(
        "コレクトアニマル監視者");
}

s32 calcGetShineNumKuriboGirl(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "ガールクリボー", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumJugemFish(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "魚影", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumGrowPlant(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "育つ草", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumRabbit(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "ウサギ", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumDigPoint(const GameDataHolder* holder) {
    s32 count = holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "ここ掘れポイント", GameDataFile::CountType::Value_1);
    count += holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "DigPointShine", GameDataFile::CountType::Value_1);
    return count + holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
                       "ヒント写真", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumCapHanger(const GameDataHolder* holder) {
    s32 count = holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "帽子ひっかけポイント", GameDataFile::CountType::Value_1);
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
               "ドクロ看板", GameDataFile::CountType::Value_1) +
           count;
}

s32 calcGetShineNumBird(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "鳥", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumCostumeRoom(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "CostumeShine", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumSlot(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "帽子スロット台座", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumRace(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "駆けっこ開始Npc", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumFigureWalking(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "フィギュアウォーキングNpc", GameDataFile::CountType::Value_1);
}

s32 calcGetShineNumHideAndSeekCapMan(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "かくれんぼ帽子の民", GameDataFile::CountType::Value_1);
}

s32 calcAllShineNumHideAndSeekCapMan(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcAllShineNumByObjectNameOrOptionalId("かくれんぼ帽子の民");
}

s32 calcGetShineNumCollectBgmNpc(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "コレクトBGMNPC", GameDataFile::CountType::Value_1);
}

s32 calcAllShineNumCollectBgmNpc(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcAllShineNumByObjectNameOrOptionalId("コレクトBGMNPC");
}

s32 calcGetShineNumHintPhoto(const GameDataHolder* holder) {
    return holder->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "ヒント写真", GameDataFile::CountType::Value_1);
}

}  // namespace rs
