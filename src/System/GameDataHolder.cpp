#include "System/GameDataHolder.h"

#include <heap/seadFrameHeap.h>
#include <heap/seadHeap.h>
#include <stream/seadRamStream.h>

#include "Library/Base/StringUtil.h"
#include "Library/File/FileUtil.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Message/LanguageUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/SaveData/SaveDataFunction.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

#include "Item/Coin.h"
#include "Layout/ShopLayoutInfo.h"
#include "Npc/AchievementHolder.h"
#include "Npc/AchievementInfoReader.h"
#include "Scene/QuestInfoHolder.h"
#include "Sequence/GameSequenceInfo.h"
#include "System/CapMessageBossData.h"
#include "System/GameConfigData.h"
#include "System/GameDataFile.h"
#include "System/GameDataFunction.h"
#include "System/GameProgressData.h"
#include "System/MapDataHolder.h"
#include "System/SaveDataAccessFunction.h"
#include "System/SaveDataAccessSequence.h"
#include "System/TempSaveData.h"
#include "System/UniqObjInfo.h"
#include "System/WorldList.h"
#include "Util/ScenePrepoFunction.h"
#include "Util/SpecialBuildUtil.h"

void initializeShopItemList(sead::PtrArray<ShopItem::ShopItemInfo>* shopItemList,
                            const char* name) {
    if (!al::isExistArchive("SystemData/ItemList"))
        return;

    al::Resource* resource = al::findOrCreateResource("SystemData/ItemList", nullptr);
    const u8* yaml = al::findResourceYaml(resource, name, nullptr);
    al::ByamlIter iter(yaml);

    s32 size = iter.getSize();
    if (size <= 0)
        return;

    shopItemList->allocBuffer(size, nullptr);

    for (s32 i = 0; i < size; i++) {
        ShopItem::ShopItemInfo* itemInfo = new ShopItem::ShopItemInfo();
        al::ByamlIter itemIter;
        iter.tryGetIterByIndex(&itemIter, i);
        itemInfo->info.index = i;

        al::copyString(itemInfo->info.name, al::tryGetByamlKeyStringOrNULL(itemIter, "ItemName"),
                       0x80);

        const char* itemType = al::tryGetByamlKeyStringOrNULL(itemIter, "ItemType");
        if (al::isEqualString(itemType, "Clothes"))
            itemInfo->info.type = ShopItem::ItemType::Cloth;
        else if (al::isEqualString(itemType, "Cap"))
            itemInfo->info.type = ShopItem::ItemType::Cap;
        else if (al::isEqualString(itemType, "Gift"))
            itemInfo->info.type = ShopItem::ItemType::Gift;
        else if (al::isEqualString(itemType, "Sticker"))
            itemInfo->info.type = ShopItem::ItemType::Sticker;
        else if (al::isEqualString(itemType, "UseItem"))
            itemInfo->info.type = ShopItem::ItemType::UseItem;
        else if (al::isEqualString(itemType, "Shine"))
            itemInfo->info.type = ShopItem::ItemType::Moon;

        al::tryGetByamlS32(&itemInfo->price, itemIter, "Price");

        const char* coinType = al::tryGetByamlKeyStringOrNULL(itemIter, "CoinType");
        if (coinType) {
            if (al::isEqualString(coinType, "Coin"))
                itemInfo->coinType = ShopItem::CoinType::Coin;
            else if (al::isEqualString(coinType, "Collect"))
                itemInfo->coinType = ShopItem::CoinType::Collect;
        }

        const char* storeName = al::tryGetByamlKeyStringOrNULL(itemIter, "StoreName");
        if (storeName && storeName[0] != '\0')
            al::copyString(itemInfo->storeName, storeName, 0x80);

        const char* clearWorld = al::tryGetByamlKeyStringOrNULL(itemIter, "ClearWorld");
        if (clearWorld && clearWorld[0] != '\0')
            al::copyString(itemInfo->clearWorld, clearWorld, 0x80);

        al::tryGetByamlS32(&itemInfo->moonNum, itemIter, "MoonNum");

        shopItemList->pushBack(itemInfo);
    }
}

void initializeItemList(sead::PtrArray<ShopItem::ItemInfo>* shopItemList, const char* name) {
    if (!al::isExistArchive("SystemData/ItemList"))
        return;

    al::Resource* resource = al::findOrCreateResource("SystemData/ItemList", nullptr);
    const u8* yaml = al::findResourceYaml(resource, name, nullptr);
    al::ByamlIter iter(yaml);

    s32 size = iter.getSize();
    if (size <= 0)
        return;

    shopItemList->allocBuffer(size, nullptr);

    for (s32 i = 0; i < size; i++) {
        ShopItem::ItemInfo* itemInfo = new ShopItem::ItemInfo();

        al::ByamlIter itemIter;
        iter.tryGetIterByIndex(&itemIter, i);
        itemInfo->index = i;

        const char* itemName = al::tryGetByamlKeyStringOrNULL(itemIter, "ItemName");
        if (!itemName)
            continue;

        al::copyString(itemInfo->name, itemName, 0x80);

        if (al::isEqualString(name, "ItemCap"))
            itemInfo->type = ShopItem::ItemType::Cap;
        else if (al::isEqualString(name, "ItemCloth"))
            itemInfo->type = ShopItem::ItemType::Cloth;
        else if (al::isEqualString(name, "ItemGift"))
            itemInfo->type = ShopItem::ItemType::Gift;
        else if (al::isEqualString(name, "ItemSticker"))
            itemInfo->type = ShopItem::ItemType::Sticker;

        al::ByamlIter amiiboIter;
        if (itemIter.tryGetIterByKey(&amiiboIter, "Amiibo")) {
            s32 amiiboSize = amiiboIter.getSize();
            if (amiiboSize > 0) {
                itemInfo->amiiboInfoList.tryAllocBuffer(amiiboSize, nullptr);

                for (s32 j = 0; j < amiiboSize; j++) {
                    al::ByamlIter amiiboJiter;
                    amiiboIter.tryGetIterByIndex(&amiiboJiter, j);

                    s32 characterId = -1;
                    if (al::tryGetByamlS32(&characterId, amiiboJiter, "CharacterId"))
                        itemInfo->amiiboInfoList[j].characterId = characterId;

                    s32 numberingId = -1;
                    if (al::tryGetByamlS32(&numberingId, amiiboJiter, "NumberingId"))
                        itemInfo->amiiboInfoList[j].numberingId = numberingId;
                }
            }
        }

        bool isAOC;
        if (itemIter.tryGetBoolByKey(&isAOC, "IsAOC"))
            itemInfo->isAOC = isAOC;

        shopItemList->pushBack(itemInfo);
    }
}

// NON-MATCHING: Major issues https://decomp.me/scratch/CyzkJ
GameDataHolder::GameDataHolder(const al::MessageSystem* messageSystem)
    : mMessageSystem(messageSystem) {
    setLanguage(al::getLanguageString());
    mSaveDataWriteHeap =
        sead::FrameHeap::create(0x200000, "セーブデータByamlIter書き込み用", nullptr, 8,
                                sead::FrameHeap::cHeapDirection_Forward, false);
    mSaveDataWorkBuffer = new u8[0x200000];
    mGameConfigData = new GameConfigData();
    mGameConfigData->init();

    mWorldList = new WorldList();

    al::ByamlIter changeStageListIter(
        al::tryGetBymlFromArcName("SystemData/WorldList", "ChangeStageList"));
    s32 changeStageListSize = changeStageListIter.getSize();
    mChangeStageList.allocBuffer(changeStageListSize, nullptr);

    for (s32 i = 0; i < changeStageListSize; i++) {
        al::ByamlIter iter;
        changeStageListIter.tryGetIterByIndex(&iter, i);
        const char* srcStageName = nullptr;
        iter.tryGetStringByKey(&srcStageName, "SrcStageName");
        const char* srcLabel = nullptr;
        iter.tryGetStringByKey(&srcLabel, "SrcLabel");
        const char* destStageName = nullptr;
        iter.tryGetStringByKey(&destStageName, "DestStageName");
        const char* destLabel = nullptr;
        iter.tryGetStringByKey(&destLabel, "DestLabel");

        ChangeStageItem* stageItem = new ChangeStageItem();
        stageItem->srcStageName.format("%s", srcStageName);
        stageItem->srcLabel.format("%s", srcLabel);
        stageItem->destStageName.format("%s", destStageName);
        stageItem->destLabel.format("%s", destLabel);

        mChangeStageList.pushBack(stageItem);
    }

    al::ByamlIter exStageListIter(al::tryGetBymlFromArcName("SystemData/WorldList", "ExStageList"));
    s32 exStageListSize = exStageListIter.getSize();
    mExStageList.allocBuffer(exStageListSize, nullptr);

    for (s32 i = 0; i < exStageListSize; i++) {
        const char* name = nullptr;
        exStageListIter.tryGetStringByIndex(&name, i);

        ExStageItem* stageItem = new ExStageItem;
        stageItem->name.format("%s", name);

        mExStageList.pushBack(stageItem);
    }

    al::ByamlIter invalidOpenMapListIter(
        al::tryGetBymlFromArcName("SystemData/WorldList", "InvalidOpenMapList"));
    s32 invalidOpenMapListSize = invalidOpenMapListIter.getSize();
    mInvalidOpenMapList.allocBuffer(invalidOpenMapListSize, nullptr);

    for (s32 i = 0; i < invalidOpenMapListSize; i++) {
        al::ByamlIter iter;
        invalidOpenMapListIter.tryGetIterByIndex(&iter, i);

        InvalidOpenMapInfo* mapInfo = new InvalidOpenMapInfo();

        iter.tryGetStringByKey(&mapInfo->name, "Name");
        iter.tryGetIntByKey(&mapInfo->scenario, "Scenario");

        mInvalidOpenMapList.pushBack(mapInfo);
    }

    al::ByamlIter stageLockListIter(
        al::tryGetBymlFromArcName("SystemData/WorldList", "StageLockList"));
    stageLockListIter.tryGetIterByKey(&stageLockListIter, "StageLockList");
    s32 stageLockListSize = stageLockListIter.getSize();

    mIsPlayAlreadyScenarioStartCamera = new bool[0x10];
    mAchievementInfoReader = new AchievementInfoReader();
    mAchievementInfoReader->init();

    mAchievementHolder = new AchievementHolder();
    mAchievementHolder->init();

    mStageLockList.allocBuffer(stageLockListSize, nullptr);

    for (s32 i = 0; i < stageLockListSize; i++) {
        al::ByamlIter iter;
        stageLockListIter.tryGetIterByIndex(&iter, i);
        StageLockInfo* lockInfo = new StageLockInfo();

        al::tryGetByamlBool(&lockInfo->isCountTotal, iter, "IsCountTotal");
        al::tryGetByamlBool(&lockInfo->isCrash, iter, "IsCrash");

        al::ByamlIter shineInfoIter;
        iter.tryGetIterByKey(&shineInfoIter, "ShineNumInfo");
        lockInfo->shineNumInfoNum = shineInfoIter.getSize();
        lockInfo->shineNumInfo = new s32[lockInfo->shineNumInfoNum];

        for (s32 j = 0; j < lockInfo->shineNumInfoNum; j++)
            shineInfoIter.tryGetIntByIndex(&lockInfo->shineNumInfo[j], j);

        mStageLockList.pushBack(lockInfo);
    }

    initializeShopItemList(&mShopItemList, "ItemList");
    if (rs::isModeE3Rom() || rs::isModeE3LiveRom())
        initializeShopItemList(&mShopItemListE3, "ItemListE3");

    initializeItemList(&mItemCloth, "ItemCloth");
    initializeItemList(&mItemCap, "ItemCap");
    initializeItemList(&mItemGift, "ItemGift");
    initializeItemList(&mItemSticker, "ItemSticker");

    s32 sizer = mShopItemList.size();
    s32 shopItemSize = 0;
    s32 shopTalkDataSize = 0;
    const char* nameList[10];
    for (s32 i = 0; i < sizer; i++) {
        if (!al::isEqualString(mShopItemList[i]->clearWorld, "")) {
            bool found = false;
            for (s32 j = 0; j < shopTalkDataSize; j++) {
                if (al::isEqualString(nameList[j], mShopItemList[i]->clearWorld)) {
                    found = true;
                    break;
                }
            }

            if (!found)
                nameList[shopTalkDataSize++] = mShopItemList[i]->clearWorld;
        }

        if (mShopItemList[i]->moonNum != -1)
            shopItemSize++;
    }

    mWorldsForNewReleaseShop.allocBuffer(shopTalkDataSize, nullptr);
    for (s32 i = 0; i < shopTalkDataSize; i++) {
        sead::FixedSafeString<64>* newWorld = new sead::FixedSafeString<64>(nameList[i]);
        mWorldsForNewReleaseShop.pushBack(newWorld);
    }

    mShopTalkDataInfos = new s32[shopItemSize];

    for (s32 i = 0; i < sizer; i++)
        if (mShopItemList[i]->moonNum != -1)
            mShopTalkDataInfos[mShopTalkDataSize++] = mShopItemList[i]->moonNum;

    al::ByamlIter hackObjListIter(al::findResourceYaml(
        al::findOrCreateResource("SystemData/HackObjList", nullptr), "HackObjList", nullptr));
    s32 hackObjListSize = hackObjListIter.getSize();
    mHackObjList.allocBuffer(hackObjListSize, nullptr);

    for (s32 i = 0; i < hackObjListSize; i++) {
        al::ByamlIter iter;
        hackObjListIter.tryGetIterByIndex(&iter, i);
        HackObjInfo* objInfo = new HackObjInfo;

        objInfo->hackName = al::tryGetByamlKeyStringOrNULL(iter, "HackName");
        objInfo->isScare = al::tryGetByamlKeyBoolOrFalse(iter, "IsScare");
        objInfo->isNoCollisionMsg = al::tryGetByamlKeyBoolOrFalse(iter, "IsNoCollisionMsg");
        objInfo->isNoSeparateCameraInput =
            al::tryGetByamlKeyBoolOrFalse(iter, "IsNoSeparateCameraInput");
        objInfo->isUsePlayerCollision = al::tryGetByamlKeyBoolOrFalse(iter, "IsUsePlayerCollision");
        objInfo->isUseCollisionPartsFilterActor =
            al::tryGetByamlKeyBoolOrFalse(iter, "IsUseCollisionPartsFilterActor");
        objInfo->tutorialName = al::tryGetByamlKeyStringOrNULL(iter, "TutorialName");
        al::tryGetByamlF32(&objInfo->guideHeight, iter, "GuideHeight");
        al::tryGetByamlBool(&objInfo->isGuideEnable, iter, "IsGuideEnable");
        al::tryGetByamlF32(&objInfo->stayGravityMargine, iter, "StayGravityMargine");

        mHackObjList.pushBack(objInfo);
    }

    s32 tutoralLabelNum = al::getSystemMessageLabelNum(this, "Tutorial");
    mShowHackTutorialList.allocBuffer(tutoralLabelNum, nullptr);

    for (s32 i = 0; i < mShowHackTutorialList.capacity(); i++)
        mShowHackTutorialList.pushBack(new sead::FixedSafeString<128>(""));

    mFiles = new GameDataFile*[5];
    for (s32 i = 0; i < 5; i++)
        mFiles[i] = new GameDataFile(this);
    mSequenceInfo = new GameSequenceInfo();
    mTempSaveData = new TempSaveData();
    mTempSaveDataBackup = new TempSaveData();
    mCapMessageBossData = new CapMessageBossData();
    mIsShowBindTutorial = new bool[3];
    mLocationName = new UniqObjInfo();
    mCoinTransForDeadPlayer = new sead::Vector3f[10];
    mQuestInfoHolder = new QuestInfoHolder(0x40);
    mMapDataHolder = new MapDataHolder(this);

    al::ByamlIter worldItemTypeListIter(
        al::tryGetBymlFromArcName("SystemData/WorldList", "WorldItemTypeList"));
    s32 worldItemTypeListSize = worldItemTypeListIter.getSize();

    mWorldItemTypeInfo.allocBuffer(worldItemTypeListSize, nullptr);
    if (worldItemTypeListSize > 0) {
        for (s32 i = 0; i < worldItemTypeListSize; i++) {
            WorldItemTypeInfo* info = new WorldItemTypeInfo;
            mWorldItemTypeInfo.pushBack(info);
        }

        for (s32 i = 0; i < worldItemTypeListSize; i++) {
            al::ByamlIter iter;
            s32 shine = 0;
            const char* coinCollect = "";
            worldItemTypeListIter.tryGetIterByIndex(&iter, i);

            iter.tryGetStringByKey(&coinCollect, "CoinCollect");
            iter.tryGetIntByKey(&shine, "Shine");
            const char* worldName = al::getByamlKeyString(iter, "WorldName");
            s32 worldIndex = mWorldList->tryFindWorldIndexByDevelopName(worldName);
            WorldItemTypeInfo* info = mWorldItemTypeInfo[worldIndex];
            info->coinCollect.format("CoinCollect%s", coinCollect);
            info->coinCollectEmpty.format("CoinCollectEmpty%s", coinCollect);
            info->coinCollect2D.format("CoinCollect2D_%s", coinCollect);
            info->coinCollectEmpty2D.format("CoinCollectEmpty2D_%s", coinCollect);
            info->shineAnimFrame = shine;
        }
    }

    mCoinCollectNumMax = new s32[mWorldList->getWorldNum()];

    al::ByamlIter collectCoinNumIter(
        al::tryGetBymlFromArcName("SystemData/WorldList", "CollectCoinNum"));

    for (s32 i = 0; i < collectCoinNumIter.getSize(); i++) {
        al::ByamlIter iter;
        collectCoinNumIter.tryGetIterByIndex(&iter, i);

        s32 collectCoinNum = al::getByamlKeyInt(iter, "CollectCoinNum");
        s32 worldIndex =
            mWorldList->tryFindWorldIndexByDevelopName(al::getByamlKeyString(iter, "WorldName"));
        mCoinCollectNumMax[worldIndex] = collectCoinNum;
    }

    mWorldWarpHoleDestIds = new s32[mWorldList->getWorldNum()];

    al::ByamlIter worldWarpHoleInfoIter(
        al::tryGetBymlFromArcName("SystemData/WorldList", "WorldWarpHoleInfo"));
    al::ByamlIter worldLinkInfoIter;
    al::getByamlIterByKey(&worldLinkInfoIter, worldWarpHoleInfoIter, "WorldLinkInfo");

    for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
        s32 destId = -1;

        worldLinkInfoIter.tryGetIntByKey(&destId, al::StringTmp<32>("%d", i).cstr());
        mWorldWarpHoleDestIds[i] = destId;
    }

    al::ByamlIter worldWarpHoleInfoIter2;
    al::getByamlIterByKey(&worldWarpHoleInfoIter2, worldWarpHoleInfoIter, "WorldWarpHoleInfo");
    mWorldWarpHoleInfoNum = worldWarpHoleInfoIter2.getSize();
    mWorldWarpHoleInfos = new WorldWarpHoleInfo[mWorldWarpHoleInfoNum];

    for (s32 i = 0; i < mWorldWarpHoleInfoNum; i++) {
        al::ByamlIter infoIter;
        al::getByamlIterByIndex(&infoIter, worldWarpHoleInfoIter2, i);

        mWorldWarpHoleInfos[i].stageName.format("%s", al::getByamlKeyString(infoIter, "StageName"));
        mWorldWarpHoleInfos[i].name.format("%s", al::getByamlKeyString(infoIter, "Name"));
        mWorldWarpHoleInfos[i].worldId = al::getByamlKeyInt(infoIter, "WorldId");
        mWorldWarpHoleInfos[i].scenarioNo = al::tryGetByamlKeyIntOrZero(infoIter, "ScenarioNo");
    }

    setPlayingFileId(0);
    initializeData();
}

// NON-MATCHING: Initializer is broken https://decomp.me/scratch/vbHSh
GameDataHolder::GameDataHolder() {
    setLanguage(al::getLanguageString());
}

GameDataHolder::~GameDataHolder() = default;

const char* GameDataHolder::getSceneObjName() const {
    return "ゲームデータ保持";
}

const al::MessageSystem* GameDataHolder::getMessageSystem() const {
    return mMessageSystem;
}

void GameDataHolder::setPlayingFileId(s32 fileId) {
    mPlayingFile = getGameDataFile(fileId);
    resetTempSaveData(false);
    if (fileId != -1)
        mPlayingFileId = fileId;
}

void GameDataHolder::initializeData() {
    for (s32 i = 0; i < 5; i++)
        initializeDataId(i);

    initializeDataCommon();
    setPlayingFileId(0);
}

void GameDataHolder::initializeDataCommon() {
    _244 = false;
    mPlayTimeAcrossFile = 0;
    mIsExistKoopaShip = false;
    resetTempSaveData(false);
    mIsValidCheckpointWarp = true;
    mStageMapPlayerPos = sead::Vector3f::zero;
    mIsPlayDemoLavaErupt = false;
    mIsDisableExplainAmiibo = false;
    mSearchHintByAmiiboCount = 0;
    mCapMessageBossData->init();

    for (s32 i = 0; i < mShowHackTutorialList.size(); i++)
        mShowHackTutorialList[i]->clear();

    for (s32 i = 0; i < 3; i++)
        mIsShowBindTutorial[i] = false;

    for (s32 i = 0; i < 10; i++)
        mCoinTransForDeadPlayer[i].set(sead::Vector3f::zero);

    mDeadPlayerCoinIdx = 0;
    resetLocationName();
    mQuestInfoHolder->clearAll();
    mSequenceInfo->init();
}

void GameDataHolder::resetTempSaveData(bool isSwapTempSaveData) {
    if (isSwapTempSaveData) {
        s32 worldIndex = mWorldList->tryFindWorldIndexByMainStageName(getNextStageName());
        TempSaveData* tempSaveData = mTempSaveData;
        s32 worldIndexBackup = mTempSaveDataBackup->getWorldIndex();

        mTempSaveData = mTempSaveDataBackup;
        mTempSaveDataBackup = tempSaveData;
        if (worldIndex != worldIndexBackup)
            mTempSaveData->init();
    } else {
        mTempSaveData->init();
        mTempSaveDataBackup->init();
    }

    for (s32 i = 0; i < 5; i++)
        getGameDataFile(i)->resetTempData();

    mCapMessageBossData->init();
    resetLocationName();
    resetScenarioStartCamera();

    mQuestInfoHolder->clearAll();
    mIsExistKoopaShip = false;
}

void GameDataHolder::initializeDataId(s32 fileId) {
    getGameDataFile(fileId)->initializeData();
}

void GameDataHolder::readByamlData(s32 fileId, const char* fileName) {
    resetTempSaveData(false);
    initializeDataId(fileId);

    const u8* bymlData = al::getBymlFromArcName("DebugData/DebugSaveData", fileName);
    getGameDataFile(fileId)->tryReadByamlData(bymlData);

    SaveDataAccessFunction::startSaveDataWriteSync(this);
}

s32 GameDataHolder::tryFindEmptyFileId() const {
    for (s32 i = 0; i < 5; i++)
        if (getGameDataFile(i)->isEmpty())
            return i;
    return -1;
}

void GameDataHolder::createSaveDataAccessSequence(const al::LayoutInitInfo& layoutInitInfo) {
    mSaveDataAccessSequence = new SaveDataAccessSequence(this, layoutInitInfo);
}

void GameDataHolder::createSaveDataAccessSequenceDevelop(const al::LayoutInitInfo& layoutInitInfo) {
    createSaveDataAccessSequence(layoutInitInfo);
    mSaveDataAccessSequence->setDevelop();
}

bool GameDataHolder::isRequireSave() const {
    return mIsRequireSave && mRequireSaveFrame == 0;
}

void GameDataHolder::setRequireSave() {
    mIsRequireSave = true;
    mRequireSaveFrame = 0;
}

void GameDataHolder::setRequireSaveFalse() {
    mIsRequireSave = false;
    mRequireSaveFrame = 0;
}

void GameDataHolder::setRequireSaveFrame() {
    if (!mIsRequireSave || mRequireSaveFrame > 0) {
        mRequireSaveFrame = 60;
        mIsRequireSave = true;
    }
}

void GameDataHolder::updateRequireSaveFrame() {
    mRequireSaveFrame = sead::Mathi::clampMin(mRequireSaveFrame - 1, 0);
}

bool GameDataHolder::isInvalidSaveForMoonGet() const {
    return mIsInvalidSaveForMoonGet;
}

void GameDataHolder::invalidateSaveForMoonGet() {
    mIsInvalidSaveForMoonGet = true;
}

void GameDataHolder::validateSaveForMoonGet() {
    mIsInvalidSaveForMoonGet = false;
}

void GameDataHolder::setLanguage(const char* language) {
    mLanguage.format("%s", language);
}

const char* GameDataHolder::getLanguage() const {
    return mLanguage.cstr();
}

void GameDataHolder::changeNextStage(const ChangeStageInfo* changeStageInfo, s32 raceType) {
    if (mIsStageChanging)
        return;

    mPlayingFile->changeNextStage(changeStageInfo, raceType);
    mIsStageChanging = true;
    resetLocationName();
}

void GameDataHolder::resetLocationName() {
    mLocationName->clear();
}

void GameDataHolder::changeNextStageWithDemoWorldWarp(const char* stageName) {
    if (mIsStageChanging)
        return;

    resetTempSaveData(false);
    mPlayingFile->changeNextStageWithDemoWorldWarp(stageName);
    mIsStageChanging = false;
}

bool GameDataHolder::tryChangeNextStageWithWorldWarpHole(const char* stageName) {
    if (mIsStageChanging)
        return false;

    mPlayingFile->changeNextStageWithWorldWarpHole(stageName);
    mIsStageChanging = true;
    resetTempSaveData(true);
    return true;
}

void GameDataHolder::returnPrevStage() {
    if (mIsStageChanging)
        return;

    mPlayingFile->returnPrevStage();
    mIsStageChanging = true;
    resetLocationName();
}

const char* GameDataHolder::getNextStageName() const {
    return mPlayingFile->getStageNameNext();
}

const char* GameDataHolder::getNextStageName(s32 fileId) const {
    return getGameDataFile(fileId)->getStageNameNext();
}

GameDataFile* GameDataHolder::getGameDataFile(s32 fileId) const {
    return mFiles[fileId];
}

const char* GameDataHolder::getNextPlayerStartId() const {
    return mPlayingFile->getPlayerStartId();
}

const char* GameDataHolder::getCurrentStageName() const {
    return mPlayingFile->getStageNameCurrent();
}

const char* GameDataHolder::tryGetCurrentStageName() const {
    return mPlayingFile->tryGetStageNameCurrent();
}

const char* GameDataHolder::getCurrentStageName(s32 fileId) const {
    return getGameDataFile(fileId)->getStageNameCurrent();
}

void GameDataHolder::setCheckpointId(const al::PlacementId* placementId) {
    mPlayingFile->setCheckpointId(placementId);
    setRequireSaveFrame();
}

const char* GameDataHolder::tryGetRestartPointIdString() const {
    return mPlayingFile->tryGetRestartPointIdString();
}

void GameDataHolder::endStage() {
    if (mIsStageEnding)
        return;
    mPlayingFile->endStage();
}

void GameDataHolder::startStage(const char* stageName, s32 scenarioNo) {
    mPlayingFile->startStage(stageName, scenarioNo);
    mTempSaveData->setInfo(mPlayingFile->getCurrentWorldId(), scenarioNo);
}

void GameDataHolder::onObjNoWriteSaveData(const al::PlacementId* placementId) {
    mTempSaveData->writeInWorld(placementId, getCurrentStageName());
}

void GameDataHolder::offObjNoWriteSaveData(const al::PlacementId* placementId) {
    mTempSaveData->deleteInWorld(placementId, getCurrentStageName());
}

bool GameDataHolder::isOnObjNoWriteSaveData(const al::PlacementId* placementId) const {
    return mTempSaveData->isOnInWorld(placementId, getCurrentStageName());
}

void GameDataHolder::onObjNoWriteSaveDataResetMiniGame(const al::PlacementId* placementId) {
    mTempSaveData->writeInWorldResetMiniGame(placementId, getCurrentStageName());
}

void GameDataHolder::offObjNoWriteSaveDataResetMiniGame(const al::PlacementId* placementId) {
    mTempSaveData->deleteInWorldResetMiniGame(placementId, getCurrentStageName());
}

bool GameDataHolder::isOnObjNoWriteSaveDataResetMiniGame(const al::PlacementId* placementId) const {
    return mTempSaveData->isOnInWorldResetMiniGame(placementId, getCurrentStageName());
}

void GameDataHolder::onObjNoWriteSaveDataInSameScenario(const al::PlacementId* placementId) {
    mTempSaveData->writeInScenario(placementId, getCurrentStageName());
}

bool GameDataHolder::isOnObjNoWriteSaveDataInSameScenario(
    const al::PlacementId* placementId) const {
    return mTempSaveData->isOnInScenario(placementId, getCurrentStageName());
}

void GameDataHolder::writeTempSaveDataToHash(const char* hashName, bool value) {
    mTempSaveData->writeHashInWorld(hashName, value);
}

bool GameDataHolder::findValueFromTempSaveDataHash(const char* hashName) {
    return mTempSaveData->findHashValueInWorld(hashName);
}

void GameDataHolder::resetMiniGameData() {
    mTempSaveData->resetMiniGame();
}

s32 GameDataHolder::getPlayingFileId() const {
    for (s32 i = 0; i < 5; i++)
        if (mPlayingFile == getGameDataFile(i))
            return i;

    return -1;
}

s32 GameDataHolder::getPlayingOrNextFileId() const {
    GameDataFile* gameDataFile = mPlayingFile;

    if (mNextFile)
        gameDataFile = mNextFile;

    for (s32 i = 0; i < 5; i++)
        if (gameDataFile == getGameDataFile(i))
            return i;

    return -1;
}

void GameDataHolder::requestSetPlayingFileId(s32 fileId) {
    mNextFile = getGameDataFile(fileId);
}

void GameDataHolder::receiveSetPlayingFileIdMsg() {
    mPlayingFile = mNextFile;
    mNextFile = nullptr;

    s32 fileId = getPlayingFileId();
    if (fileId != -1)
        mPlayingFileId = fileId;

    resetTempSaveData(false);
}

GameDataFile* GameDataHolder::findGameDataFile(const char* fileName) const {
    for (s32 i = 0; i < 5; i++)
        if (al::isEqualString(fileName, al::StringTmp<32>("%s%d.bin", "File", i + 1)))
            return getGameDataFile(i);

    return nullptr;
}

GameDataFile* GameDataHolder::findFileByName(const char* fileName) const {
    return findGameDataFile(fileName);
}

void GameDataHolder::resetScenarioStartCamera() {
    for (s32 i = 0; i < 16; i++)
        mIsPlayAlreadyScenarioStartCamera[i] = false;
}

void GameDataHolder::resetTempSaveDataInSameScenario() {
    mTempSaveData->initForScenario();
    mIsExistKoopaShip = false;
}

struct SaveDataBuffer {
    s32 _0;
    s32 _4;
    s32 playingFileId;
    char language[0x20];
    u64 playTime;
};

static_assert(sizeof(SaveDataBuffer) == 0x38);

void GameDataHolder::readFromSaveDataBuffer(const char* fileName) {
    u8* saveDataBuffer = al::getSaveDataWorkBuffer();
    memset(mSaveDataWorkBuffer, 0, 0x200000);

    if (al::isEqualString("Common.bin", fileName)) {
        sead::RamReadStream readStream(saveDataBuffer, 0x400, sead::Stream::Modes::Binary);
        initializeDataCommon();

        SaveDataBuffer buffer;
        memset(&buffer, 0, sizeof(SaveDataBuffer));
        readStream.readMemBlock((void*)&buffer, sizeof(SaveDataBuffer));

        if (buffer._0 != 0) {
            initializeDataCommon();
            return;
        }

        mLanguage.format("%s", buffer.language);
        mPlayTimeAcrossFile = buffer.playTime;
        s32 id = sead::Mathi::clampMin(buffer.playingFileId, 0);
        setPlayingFileId(id);

        s32 saveDataSize = 0;
        readStream.readS32(saveDataSize);
        if (saveDataSize > 0x200000)
            return;

        readStream.readMemBlock((void*)mSaveDataWorkBuffer, saveDataSize);
        tryReadByamlDataCommon(mSaveDataWorkBuffer);
    } else {
        GameDataFile* dataFile = findGameDataFile(fileName);
        sead::RamReadStream readStream(saveDataBuffer, 0x200000, sead::Stream::Modes::Binary);
        dataFile->initializeData();
        if (!dataFile->readFromStream(&readStream, mSaveDataWorkBuffer))
            dataFile->initializeData();
    }
}

bool GameDataHolder::tryReadByamlDataCommon(const u8* byamlData) {
    if (alByamlLocalUtil::verifiByaml(byamlData)) {
        al::ByamlIter save{byamlData};
        mGameConfigData->read(save);
        return true;
    }
    return false;
}

void GameDataHolder::readFromSaveDataBufferCommonFileOnlyLanguage() {
    sead::RamReadStream readStream(al::getSaveDataWorkBuffer(), 0x400, sead::Stream::Modes::Binary);

    SaveDataBuffer buffer;
    memset(&buffer, 0, sizeof(SaveDataBuffer));
    readStream.readMemBlock((void*)&buffer, sizeof(SaveDataBuffer));

    if (buffer._0 == 0)
        mLanguage.format("%s", buffer.language);
}

void GameDataHolder::writeToSaveDataBuffer(const char* fileName) {
    u8* saveDataBuffer = al::getSaveDataWorkBuffer();
    mSaveDataWriteHeap->freeAll();

    if (al::isEqualString(fileName, "Common.bin")) {
        sead::RamWriteStream writeStream(saveDataBuffer, 0x400, sead::Stream::Modes::Binary);

        SaveDataBuffer buffer;
        memset(&buffer, 0, sizeof(SaveDataBuffer));

        buffer.playingFileId = sead::Mathi::clampMin(getPlayingOrNextFileId(), 0);
        buffer._4 = 0;
        al::copyString(buffer.language, getLanguage(), 0x20);
        buffer.playTime = mPlayTimeAcrossFile;
        writeStream.writeMemBlock(&buffer, sizeof(SaveDataBuffer));

        al::ByamlWriter byamlWriter{mSaveDataWriteHeap, false};
        byamlWriter.pushHash();
        mGameConfigData->write(&byamlWriter);
        byamlWriter.pop();
        writeStream.writeS32(byamlWriter.calcPackSize());
        byamlWriter.write(&writeStream);
    } else {
        GameDataFile* dataFile = findGameDataFile(fileName);
        sead::RamWriteStream writeStream(saveDataBuffer, 0x200000, sead::Stream::Modes::Binary);

        dataFile->updateSaveTime();
        dataFile->writeToStream(&writeStream, mSaveDataWriteHeap);
    }

    setRequireSaveFalse();
}

void GameDataHolder::updateSaveInfoForDisp(const char* fileName) {
    if (al::isEqualString(fileName, "Common.bin"))
        return;

    findGameDataFile(fileName)->updateSaveInfoForDisp();
}

void GameDataHolder::updateSaveTimeForDisp(const char* fileName) {
    if (al::isEqualString(fileName, "Common.bin"))
        return;

    findGameDataFile(fileName)->updateSaveTimeForDisp();
}

// NON-MATCHING: Add operation references "j" variable https://decomp.me/scratch/l1ctd
s32 GameDataHolder::findUnlockShineNum(bool* isCountTotal, s32 worldId) const {
    s32 worldNumMax = 0;
    for (s32 i = 0; i < mStageLockList.size(); i++) {
        if (mStageLockList[i]->shineNumInfoNum <= 0)
            continue;

        for (s32 j = 0; j < mStageLockList[i]->shineNumInfoNum; j++) {
            if (j + worldNumMax == worldId) {
                if (mStageLockList[i]->isCountTotal && isCountTotal)
                    *isCountTotal = true;

                return mStageLockList[i]->shineNumInfo[j];
            }
        }

        worldNumMax += mStageLockList[i]->shineNumInfoNum;
    }

    return -1;
}

s32 GameDataHolder::calcBeforePhaseWorldNumMax(s32 worldId) const {
    s32 worldNumMax = -1;
    for (s32 i = 0; i < mStageLockList.size(); i++) {
        if (mStageLockList[i]->shineNumInfoNum + worldNumMax >= worldId)
            return worldNumMax;

        worldNumMax += mStageLockList[i]->shineNumInfoNum;
    }

    return -1;
}

bool GameDataHolder::isFindKoopaNext(s32 worldId) const {
    if ((mPlayingFile->getGameProgressData()->getUnlockWorldNum() ==
             GameDataFunction::getWorldIndexCloud() ||
         mPlayingFile->getGameProgressData()->getUnlockWorldNum() ==
             GameDataFunction::getWorldIndexCloud() + 1) &&
        GameDataFunction::getWorldIndexCity() == worldId) {
        return true;
    }
    return false;
}

bool GameDataHolder::isBossAttackedHomeNext(s32 worldId) const {
    if ((mPlayingFile->getGameProgressData()->getUnlockWorldNum() ==
             GameDataFunction::getWorldIndexBoss() ||
         mPlayingFile->getGameProgressData()->getUnlockWorldNum() ==
             GameDataFunction::getWorldIndexBoss() + 1) &&
        GameDataFunction::getWorldIndexSky() == worldId) {
        return true;
    }
    return false;
}

void GameDataHolder::playScenarioStartCamera(s32 questNo) {
    mIsPlayAlreadyScenarioStartCamera[questNo] = true;
}

bool GameDataHolder::isPlayAlreadyScenarioStartCamera(s32 questNo) const {
    return mIsPlayAlreadyScenarioStartCamera[questNo];
}

const sead::PtrArray<ShopItem::ShopItemInfo>& GameDataHolder::getShopItemInfoList() const {
    if (rs::isModeE3Rom() || rs::isModeE3LiveRom())
        return mShopItemListE3;
    return mShopItemList;
}

bool GameDataHolder::checkNeedTreasureMessageStage(const char* stageName) const {
    return mWorldList->checkNeedTreasureMessageStage(stageName);
}

bool GameDataHolder::tryFindLinkDestStageInfo(const char** destStageName, const char** destLabel,
                                              const char* srcStageName,
                                              const char* srcLabel) const {
    for (s32 i = 0; i < mChangeStageList.size(); i++) {
        if (!al::isEqualString(srcStageName, mChangeStageList[i]->srcStageName.cstr()))
            continue;
        if (!al::isEqualString(srcLabel, mChangeStageList[i]->srcLabel.cstr()))
            continue;

        *destLabel = mChangeStageList[i]->destLabel.cstr();
        *destStageName = mChangeStageList[i]->destStageName.cstr();
        return true;
    }
    return false;
}

bool GameDataHolder::isShowHackTutorial(const char* hackName, const char* suffix) const {
    for (s32 i = 0; i < mShowHackTutorialList.size(); i++) {
        if (mShowHackTutorialList[i]->isEmpty())
            continue;

        if (mShowHackTutorialList[i]->isEqual(al::StringTmp<128>{"%s%s", hackName, suffix}))
            return true;
    }
    return false;
}

void GameDataHolder::setShowHackTutorial(const char* hackName, const char* suffix) {
    if (isShowHackTutorial(hackName, suffix))
        return;

    for (s32 i = 0; i < mShowHackTutorialList.size(); i++) {
        if (mShowHackTutorialList[i]->isEmpty()) {
            mShowHackTutorialList[i]->format("%s%s", hackName, suffix);
            return;
        }
    }
}

bool GameDataHolder::isShowBindTutorial(const char* bindName) const {
    s32 index;

    if (al::isEqualString("SphinxRide", bindName))
        index = 0;
    else if (al::isEqualString("Motorcycle", bindName))
        index = 1;
    else if (al::isEqualString("WorldWarpHole", bindName))
        index = 2;
    else
        return false;

    return mIsShowBindTutorial[index];
}

const char* GameDataHolder::getCoinCollectArchiveName(s32 worldId) const {
    if (worldId > -1)
        return mWorldItemTypeInfo[worldId]->coinCollect.cstr();
    return "CoinCollect";
}

const char* GameDataHolder::getCoinCollectEmptyArchiveName(s32 worldId) const {
    if (worldId > -1)
        return mWorldItemTypeInfo[worldId]->coinCollectEmpty.cstr();
    return "CoinCollectEmptyA";
}

const char* GameDataHolder::getCoinCollect2DArchiveName(s32 worldId) const {
    if (worldId > -1)
        return mWorldItemTypeInfo[worldId]->coinCollect2D.cstr();
    return "CoinCollect2D";
}

const char* GameDataHolder::getCoinCollect2DEmptyArchiveName(s32 worldId) const {
    if (worldId > -1)
        return mWorldItemTypeInfo[worldId]->coinCollectEmpty2D.cstr();
    return "CoinCollectEmpty2D_A";
}

s32 GameDataHolder::getShineAnimFrame(s32 worldId) const {
    if (worldId < 0)
        return 0;

    return mWorldItemTypeInfo[worldId]->shineAnimFrame;
}

s32 GameDataHolder::getCoinCollectNumMax(s32 worldId) const {
    return mCoinCollectNumMax[worldId];
}

bool GameDataHolder::isInvalidOpenMapStage(const char* stageName, s32 scenarioNo) const {
    s32 size = mInvalidOpenMapList.size();
    for (s32 i = 0; i < size; i++) {
        auto* list = mInvalidOpenMapList[i];
        if (al::isEqualString(stageName, list->name)) {
            if (list->scenario < 0 || list->scenario == scenarioNo)
                return true;
        }
    }
    return false;
}

void GameDataHolder::setShowBindTutorial(const char* bindName) {
    s32 index;

    if (al::isEqualString("SphinxRide", bindName))
        index = 0;
    else if (al::isEqualString("Motorcycle", bindName))
        index = 1;
    else if (al::isEqualString("WorldWarpHole", bindName))
        index = 2;
    else
        return;

    mIsShowBindTutorial[index] = true;
}

s32 GameDataHolder::tryCalcWorldWarpHoleSrcId(s32 destId) const {
    s32 holeSrcId = -1;
    for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
        if (calcWorldWarpHoleDestId(i) != destId)
            continue;
        if (holeSrcId != -1)
            return -1;
        holeSrcId = i;
    }

    return holeSrcId;
}

s32 GameDataHolder::calcWorldWarpHoleDestId(s32 srcId) const {
    return mWorldWarpHoleDestIds[srcId];
}

s32 GameDataHolder::calcWorldWarpHoleIdFromWorldId(s32 worldId) const {
    if (GameDataFunction::getWorldIndexPeach() == worldId ||
        GameDataFunction::getWorldIndexLava() == worldId ||
        GameDataFunction::getWorldIndexCity() == worldId) {
        return worldId;
    }

    for (s32 i = 0; i < mWorldList->getWorldNum(); i++)
        if (calcWorldIdFromWorldWarpHoleId(i) == worldId)
            return i;

    return -1;
}

s32 GameDataHolder::calcWorldIdFromWorldWarpHoleId(s32 worldWarpHoleId) const {
    return mPlayingFile->getGameProgressData()->getWorldIdForWorldWarpHole(worldWarpHoleId);
}

void GameDataHolder::calcWorldWarpHoleLabelAndStageName(sead::BufferedSafeString* label,
                                                        sead::BufferedSafeString* stageName,
                                                        const char* srcLabel, s32 worldId) const {
    label->clear();
    s32 warpHoleId = calcWorldWarpHoleIdFromWorldId(worldId);
    if (warpHoleId == -1)
        return;

    // BUG: "Go" and "Come" are inverted in warpHoleInfo
    const WorldWarpHoleInfo* warpHoleInfo;
    if (al::isEqualSubString(srcLabel, "Come")) {
        s32 srcId = tryCalcWorldWarpHoleSrcId(warpHoleId);
        if (srcId == -1) {
            for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
                sead::FixedSafeString<128> nmp;
                nmp.format("%s%s%d", "Come", "From", i);

                if (al::isEqualString(nmp.cstr(), srcLabel)) {
                    srcId = i;
                    break;
                }
            }
            if (srcId == -1)
                return;
        }

        s32 warpHoleWorldId = calcWorldIdFromWorldWarpHoleId(srcId);
        if (warpHoleWorldId == -1)
            return;

        // BUG: Correct call findWorldWarpHoleInfo(warpHoleWorldId, warpHoleId, "Go")
        warpHoleInfo = findWorldWarpHoleInfo(warpHoleWorldId, worldId, "Go");
    } else {
        s32 warpHoleWorldId = calcWorldIdFromWorldWarpHoleId(calcWorldWarpHoleDestId(warpHoleId));
        if (warpHoleWorldId == -1)
            return;

        warpHoleInfo =
            findWorldWarpHoleInfo(warpHoleWorldId, calcWorldWarpHoleIdFromWorldId(worldId), "Come");
    }

    label->format("%s", warpHoleInfo->name.cstr());
    stageName->format("%s", warpHoleInfo->stageName.cstr());
    mPlayingFile->setWarpHoleWorldId(warpHoleInfo->worldId);
}

const GameDataHolder::WorldWarpHoleInfo*
GameDataHolder::findWorldWarpHoleInfo(s32 worldId, s32 scenarioNo, const char* name) const {
    for (s32 i = 0; i < mWorldWarpHoleInfoNum; i++) {
        if (mWorldWarpHoleInfos[i].worldId != worldId)
            continue;
        sead::FixedSafeString<128> nmp;
        nmp.format("%s%s%d", name, "From", scenarioNo);

        if (al::isEqualString(name, mWorldWarpHoleInfos[i].name) ||
            al::isEqualString(nmp, mWorldWarpHoleInfos[i].name)) {
            return &mWorldWarpHoleInfos[i];
        }
    }

    return nullptr;
}

bool GameDataHolder::checkIsOpenWorldWarpHoleInScenario(s32 worldId, s32 scenarioNo) const {
    for (s32 i = 0; i < mWorldWarpHoleInfoNum; i++) {
        if (mWorldWarpHoleInfos[i].worldId != worldId)
            continue;

        if (al::isEqualString(mWorldWarpHoleInfos[i].name, "Go"))
            return mWorldWarpHoleInfos[i].scenarioNo <= scenarioNo;
    }

    return false;
}

void GameDataHolder::setLocationName(const al::PlacementInfo* placementInfo) {
    mLocationName->set(GameDataFunction::getCurrentStageName(const_cast<GameDataHolder*>(this)),
                       placementInfo);
}

bool GameDataHolder::isPrevLocation(const al::PlacementInfo* placementInfo) const {
    UniqObjInfo objInfo;

    objInfo.set(GameDataFunction::getCurrentStageName(const_cast<GameDataHolder*>(this)),
                placementInfo);

    return mLocationName->isEqual(objInfo);
}

void GameDataHolder::setCoinTransForDeadPlayer(const sead::PtrArray<Coin>& coins, s32 coinNum) {
    mDeadPlayerCoinIdx = 0;
    for (s32 i = 0; i < coinNum; i++) {
        if (coins[i] == nullptr || al::isDead(coins[i]))
            continue;

        mCoinTransForDeadPlayer[mDeadPlayerCoinIdx].set(al::getTrans(coins[i]));
        mDeadPlayerCoinIdx++;
    }
}

const sead::Vector3f& GameDataHolder::getCoinTransForDeadPlayer(s32 coinIdx) {
    return mCoinTransForDeadPlayer[coinIdx];
}

void GameDataHolder::setSeparatePlay(bool isSeparatePlay) {
    mIsSeparatePlay = isSeparatePlay;
    rs::trySavePrepoSeparatePlayMode(isSeparatePlay, mPlayingFile->getPlayTimeTotal(),
                                     mPlayingFile->getSaveDataIdForPrepo(), mPlayTimeAcrossFile);
}

CapMessageBossData* GameDataHolder::getCapMessageBossData() const {
    return mCapMessageBossData;
}

s32 GameDataHolder::findUseScenarioNo(const char* stageName) const {
    return mWorldList->findUseScenarioNo(stageName);
}
