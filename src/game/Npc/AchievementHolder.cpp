#include "al/resource/AchievementHolder.h"

#include "al/resource/Achievement.h"
#include "al/resource/AchievementInfoReader.h"
#include "al/util/ResourceUtil.h"
#include "al/util/StringUtil.h"

AchievementHolder::AchievementHolder() = default;

void AchievementHolder::init(const al::ActorInitInfo&) {
    mAchievementInfoReader = new AchievementInfoReader();
    mAchievementInfoReader->init();
    mArray.allocBuffer(mAchievementInfoReader->size(), nullptr);

    for (int i = 0; i < mArray.capacity(); i++) {
        mArray.pushBack(new Achievement(mAchievementInfoReader->get(i)));
    }
}
void AchievementHolder::init() {
    mAchievementInfoReader = new AchievementInfoReader();
    mAchievementInfoReader->init();
    mArray.allocBuffer(mAchievementInfoReader->size(), nullptr);

    for (int i = 0; i < mArray.capacity(); i++) {
        mArray.pushBack(new Achievement(mAchievementInfoReader->get(i)));
    }
}

bool AchievementHolder::isGetMoon(int index, GameDataHolderAccessor accessor) const {
    return mArray[index]->isGet(accessor);
}

bool AchievementHolder::isAchieve(int index, GameDataHolderAccessor accessor) const {
    return getAchievementProgressCurrent(index, accessor) >=
           mAchievementInfoReader->get(index)->mNum;
}

int AchievementHolder::getAchievementProgressCurrent(int index,
                                                     GameDataHolderAccessor accessor) const {
    int progressCurrentRow = getAchievementProgressCurrentRow(index, accessor);
    auto* infoItem = mAchievementInfoReader->get(index);
    const char** name = infoItem ? &infoItem->mName : nullptr;

    if (al::isEqualSubString(*name, "Shine_")) {
        int max = 999;

        if (progressCurrentRow <= 999)
            max = progressCurrentRow;

        return progressCurrentRow < 0 ? 0 : max;
    }
    if (!al::isEqualSubString(*name, "Other_Coin_") && !al::isEqualString(*name, "Other_Jump") &&
        !al::isEqualString(*name, "Other_CapThrow")) {
        return progressCurrentRow;
    }

    int max = 999999999;

    if (progressCurrentRow <= max)
        max = progressCurrentRow;

    return progressCurrentRow < 0 ? 0 : max;
}

int AchievementHolder::getAchievementProgressMax(int index, GameDataHolderAccessor) const {
    return mAchievementInfoReader->get(index)->mNum;
}

int AchievementHolder::getAchievementProgressCurrentRow(int index,
                                                        GameDataHolderAccessor accessor) const {
    auto* infoItem = mAchievementInfoReader->get(index);
    const char** name = infoItem ? &infoItem->mName : nullptr;

    if (al::isEqualString(*name, "Scenario_Ending"))
        return 1;
    if (al::isEqualString(*name, "Scenario_WorldAll"))
        return rs::calcClearWorldNum(accessor);
    if (al::isEqualString(*name, "Shine_Gather_1") || al::isEqualString(*name, "Shine_Gather_2") ||
        al::isEqualString(*name, "Shine_Gather_3"))
        return GameDataFunction::getTotalShineNum(accessor, -1);
    if (al::isEqualString(*name, "Shine_CollectCoinShop"))
        return rs::calcBuyItemNumForCoinCollectByWorld(accessor);
    if (al::isEqualString(*name, "Shine_Shine2D_1") || al::isEqualString(*name, "Shine_Shine2D_2"))
        return rs::calcGetShineNumDot(accessor);
    if (al::isEqualString(*name, "Shine_TreasureBox_1") ||
        al::isEqualString(*name, "Shine_TreasureBox_2"))
        return rs::calcGetShineNumTreasureBox(accessor);
    if (al::isEqualString(*name, "Shine_MusicNote_1") ||
        al::isEqualString(*name, "Shine_MusicNote_2"))
        return rs::calcGetShineNumNoteObj(accessor);
    if (al::isEqualString(*name, "Shine_TimerAthretic_1") ||
        al::isEqualString(*name, "Shine_TimerAthretic_2"))
        return rs::calcGetShineNumTimerAthletic(accessor);
    if (al::isEqualString(*name, "Shine_CaptainKinopio_1") ||
        al::isEqualString(*name, "Shine_CaptainKinopio_2"))
        return rs::calcGetShineNumKinopioBrigade(accessor);
    if (al::isEqualString(*name, "Shine_TravelingPeach_1") ||
        al::isEqualString(*name, "Shine_TravelingPeach_2"))
        return rs::calcGetShineNumWorldTravelingPeach(accessor);
    if (al::isEqualString(*name, "Shine_CollectAnimalAll"))
        return rs::calcGetShineNumCollectAnimal(accessor);
    if (al::isEqualString(*name, "Shine_KuriboGirl"))
        return rs::calcGetShineNumKuriboGirl(accessor);
    if (al::isEqualString(*name, "Shine_Jugem"))
        return rs::calcGetShineNumJugemFish(accessor);
    if (al::isEqualString(*name, "Shine_Seed_1") || al::isEqualString(*name, "Shine_Seed_2"))
        return rs::calcGetShineNumGrowPlant(accessor);
    if (al::isEqualString(*name, "Shine_Rabbit_1") || al::isEqualString(*name, "Shine_Rabbit_2"))
        return rs::calcGetShineNumRabbit(accessor);
    if (al::isEqualString(*name, "Shine_DigPoint_1") ||
        al::isEqualString(*name, "Shine_DigPoint_2"))
        return rs::calcGetShineNumDigPoint(accessor);
    if (al::isEqualString(*name, "Shine_CapHanger_1") ||
        al::isEqualString(*name, "Shine_CapHanger_2"))
        return rs::calcGetShineNumCapHanger(accessor);
    if (al::isEqualString(*name, "Shine_Bird"))
        return rs::calcGetShineNumBird(accessor);
    if (al::isEqualString(*name, "Shine_CostumeRoom_1") ||
        al::isEqualString(*name, "Shine_CostumeRoom_2") ||
        al::isEqualString(*name, "Shine_CostumeRoom_3"))
        return rs::calcGetShineNumCostumeRoom(accessor);
    if (al::isEqualString(*name, "Shine_CapThrottle"))
        return rs::calcGetShineNumSlot(accessor);
    if (al::isEqualString(*name, "Shine_HideAndSeekCapMan"))
        return rs::calcGetShineNumHideAndSeekCapMan(accessor);
    if (al::isEqualString(*name, "Shine_CollectBgm"))
        return rs::calcGetShineNumCollectedBgm(accessor);
    if (al::isEqualString(*name, "Shine_HintPhoto_1") ||
        al::isEqualString(*name, "Shine_HintPhoto_2"))
        return rs::calcGetShineNumHintPhoto(accessor);
    if (al::isEqualString(*name, "MiniGame_RaceMan_1") ||
        al::isEqualString(*name, "MiniGame_RaceMan_2"))
        return rs::calcGetShineNumRace(accessor);
    if (al::isEqualString(*name, "MiniGame_FigureWalker"))
        return rs::calcGetShineNumFigureWalking(accessor);
    if (al::isEqualString(*name, "MiniGame_SphinxQuiz"))
        return rs::calcSphinxQuizCompleteNum(accessor);
    if (al::isEqualString(*name, "Souvenir_Count_1") ||
        al::isEqualString(*name, "Souvenir_Count_2") ||
        al::isEqualString(*name, "Souvenir_Count_3"))
        return rs::calcHaveStickerNum(accessor) + rs::calcHaveGiftNum(accessor);
    if (al::isEqualString(*name, "Capture_Count_1") ||
        al::isEqualString(*name, "Capture_Count_2") || al::isEqualString(*name, "Capture_Count_3"))
        return rs::calcHackObjNum(accessor);
    if (al::isEqualString(*name, "Costume_Cap_1") || al::isEqualString(*name, "Costume_Cap_2"))
        return rs::calcHaveCapNum(accessor);
    if (al::isEqualString(*name, "Costume_Clothes_1") ||
        al::isEqualString(*name, "Costume_Clothes_2"))
        return rs::calcHaveClothNum(accessor);
    if (al::isEqualString(*name, "Other_MoonStoneAll"))
        return rs::calcUnlockMoonRockNum(accessor);
    if (al::isEqualString(*name, "Other_WorldWarpHoleAll"))
        return rs::calcWorldWarpHoleThroughNum(accessor);
    if (al::isEqualString(*name, "Other_CheckPoint_1") ||
        al::isEqualString(*name, "Other_CheckPoint_2"))
        return rs::calcGetCheckpointNum(accessor);
    if (al::isEqualString(*name, "Other_Coin_1") || al::isEqualString(*name, "Other_Coin_2") ||
        al::isEqualString(*name, "Other_Coin_3"))
        return rs::getTotalCoinNum(accessor);
    if (al::isEqualString(*name, "Other_Jump"))
        return rs::getPlayerJumpCount(accessor);
    if (al::isEqualString(*name, "Other_CapThrow"))
        return rs::getPlayerThrowCapCount(accessor);

    return 0;
}

int AchievementHolder::calcAchieveTotalNum(GameDataHolderAccessor accessor) const {
    int count = 0;

    for (int i = 0; i < mArray.capacity(); i++) {
        if (getAchievementProgressCurrent(i, accessor) >= mAchievementInfoReader->get(i)->mNum)
            count++;
    }

    return count;
}

int AchievementHolder::calcMoonGetTotalNum(GameDataHolderAccessor accessor) const {
    int count = 0;

    for (int i = 0; i < mArray.capacity(); i++) {
        if (mArray[i]->isGet(accessor))
            count++;
    }

    return count;
}

Achievement*
AchievementHolder::tryGetNewAchievement(GameDataHolderAccessor accessor) const {  // TODO mismatch
    int i = 0;
    for (; i < mArray.capacity(); i++) {
        if (!mArray[i]->isGet(accessor) &&
            getAchievementProgressCurrent(i, accessor) >= mAchievementInfoReader->get(i)->mNum)
            break;
    }
    return mArray[i];
}
