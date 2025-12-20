#include "Util/ClothUtil.h"

#include "Library/Base/StringUtil.h"
#include "Library/Message/MessageHolder.h"

#include "Layout/ShopLayoutInfo.h"
#include "System/GameDataFile.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolder.h"

namespace rs {

void buyItemInShopItemList(const al::IUseSceneObjHolder* user, s32 itemIdx) {
    const sead::PtrArray<ShopItem::ShopItemInfo>& itemList =
        GameDataFunction::getShopItemInfoList(user);
    GameDataHolderAccessor(user)->getGameDataFile()->buyItem(&itemList[itemIdx]->info, true);
}

bool isBuyItemInShopItemList(const al::IUseSceneObjHolder* user, s32 itemIdx) {
    const sead::PtrArray<ShopItem::ShopItemInfo>& itemList =
        GameDataFunction::getShopItemInfoList(user);

    return GameDataHolderAccessor(user)->getGameDataFile()->isBuyItem(&itemList[itemIdx]->info);
}

void buyItem(const al::IUseSceneObjHolder* user, const ShopItem::ItemInfo* itemInfo) {
    GameDataHolderAccessor(user)->getGameDataFile()->buyItem(itemInfo, true);
}

void buyItemAll(const al::IUseSceneObjHolder* user) {
    GameDataHolderAccessor(user)->getGameDataFile()->buyItemAll();
}

bool isBuyItem(GameDataHolderAccessor accessor, const ShopItem::ItemInfo* itemInfo) {
    switch (itemInfo->type) {
    case ShopItem::ItemType::Cloth:
        return isHaveCloth(accessor, itemInfo->name);
    case ShopItem::ItemType::Cap:
        return isHaveCap(accessor, itemInfo->name);
    case ShopItem::ItemType::Gift:
        return isHaveGift(accessor, itemInfo->name);
    case ShopItem::ItemType::Sticker:
        return isHaveSticker(accessor, itemInfo->name);
    default:
        return false;
    }
}

bool isHaveCloth(GameDataHolderAccessor accessor, const char* name) {
    return accessor->getGameDataFile()->isBuyItem(name,
                                                  accessor->getGameDataFile()->getClothList());
}

bool isHaveCap(GameDataHolderAccessor accessor, const char* name) {
    return accessor->getGameDataFile()->isBuyItem(name, accessor->getGameDataFile()->getCapList());
}

bool isHaveSticker(GameDataHolderAccessor accessor, const char* name) {
    return accessor->getGameDataFile()->isBuyItem(name,
                                                  accessor->getGameDataFile()->getStickerList());
}

bool isHaveGift(GameDataHolderAccessor accessor, const char* name) {
    return accessor->getGameDataFile()->isBuyItem(name, accessor->getGameDataFile()->getGiftList());
}

const sead::PtrArray<ShopItem::ItemInfo>& getClothList(GameDataHolderAccessor accessor) {
    return accessor->getClothList();
}

s32 getClothListSize(GameDataHolderAccessor accessor) {
    return getClothList(accessor).size();
}

bool isHaveCloth(GameDataHolderAccessor accessor, s32 clothIdx) {
    return accessor->getGameDataFile()->isBuyItem(getClothList(accessor)[clothIdx]->name,
                                                  accessor->getGameDataFile()->getClothList());
}

bool isHaveClothByFileId(GameDataHolderAccessor accessor, const char* clothName, s32 fileId) {
    GameDataFile* gameDataFile = accessor->getGameDataFile(fileId);
    return gameDataFile->isBuyItem(clothName, gameDataFile->getClothList());
}

s32 calcHaveClothNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcHaveClothNum();
}

void buyCloth(const al::IUseSceneObjHolder* user, const char* clothName) {
    const sead::PtrArray<ShopItem::ItemInfo>& list = getClothList(user);
    for (s32 i = 0; i < list.size(); i++) {
        if (al::isEqualString(clothName, list[i]->name)) {
            buyItem(user, list[i]);
            return;
        }
    }
}

const sead::PtrArray<ShopItem::ItemInfo>& getCapList(GameDataHolderAccessor accessor) {
    return accessor->getCapList();
}

s32 getCapListSize(GameDataHolderAccessor accessor) {
    return getCapList(accessor).size();
}

bool isHaveCap(GameDataHolderAccessor accessor, s32 capIdx) {
    return accessor->getGameDataFile()->isBuyItem(getCapList(accessor)[capIdx]->name,
                                                  accessor->getGameDataFile()->getCapList());
}

bool isHaveCapByFileId(GameDataHolderAccessor accessor, const char* capName, s32 fileId) {
    GameDataFile* gameDataFile = accessor->getGameDataFile(fileId);
    return gameDataFile->isBuyItem(capName, gameDataFile->getCapList());
}

bool isCompleteClothCap(const al::IUseSceneObjHolder* user) {
    return isCompleteCloth(user) && isCompleteCap(user);
}

bool isCompleteCloth(const al::IUseSceneObjHolder* user) {
    const sead::PtrArray<ShopItem::ItemInfo>& list = getClothList(user);
    for (s32 i = 0; i < list.size(); i++)
        if (!list[i]->isAOC && !isHaveCloth(user, list[i]->name))
            return false;
    return true;
}

bool isCompleteCap(const al::IUseSceneObjHolder* user) {
    const sead::PtrArray<ShopItem::ItemInfo>& list = getCapList(user);
    for (s32 i = 0; i < list.size(); i++)
        if (!list[i]->isAOC && !isHaveCap(user, list[i]->name))
            return false;
    return true;
}

bool isCompleteCloth(const al::IUseSceneObjHolder* user, s32 fileId) {
    const sead::PtrArray<ShopItem::ItemInfo>& list = getClothList(user);
    for (s32 i = 0; i < list.size(); i++)
        if (!list[i]->isAOC && !isHaveClothByFileId(user, list[i]->name, fileId))
            return false;
    return true;
}

bool isCompleteCap(const al::IUseSceneObjHolder* user, s32 fileId) {
    const sead::PtrArray<ShopItem::ItemInfo>& list = getCapList(user);
    for (s32 i = 0; i < list.size(); i++)
        if (!list[i]->isAOC && !isHaveCapByFileId(user, list[i]->name, fileId))
            return false;
    return true;
}

s32 calcHaveCapNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcHaveCapNum();
}

void buyCap(const al::IUseSceneObjHolder* user, const char* capName) {
    const sead::PtrArray<ShopItem::ItemInfo>& list = getCapList(user);
    for (s32 i = 0; i < list.size(); i++) {
        if (al::isEqualString(capName, list[i]->name)) {
            buyItem(user, list[i]);
            return;
        }
    }
}

const sead::PtrArray<ShopItem::ItemInfo>& getGiftList(GameDataHolderAccessor accessor) {
    return accessor->getGiftList();
}

s32 getGiftListSize(GameDataHolderAccessor accessor) {
    return getGiftList(accessor).size();
}

bool isHaveGift(GameDataHolderAccessor accessor, s32 giftIdx) {
    return accessor->getGameDataFile()->isBuyItem(getGiftList(accessor)[giftIdx]->name,
                                                  accessor->getGameDataFile()->getGiftList());
}

s32 calcHaveGiftNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcHaveGiftNum();
}

bool checkCompleteGift(GameDataHolderAccessor accessor, s32 fileId) {
    return accessor->getGameDataFile(fileId)->calcHaveGiftNum() == getGiftListSize(accessor);
}

const sead::PtrArray<ShopItem::ItemInfo>& getStickerList(GameDataHolderAccessor accessor) {
    return accessor->getStickerList();
}

s32 getStickerListSize(GameDataHolderAccessor accessor) {
    return getStickerList(accessor).size();
}

bool isHaveSticker(GameDataHolderAccessor accessor, s32 stickerIdx) {
    return accessor->getGameDataFile()->isBuyItem(getStickerList(accessor)[stickerIdx]->name,
                                                  accessor->getGameDataFile()->getStickerList());
}

s32 calcHaveStickerNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcHaveStickerNum();
}

bool checkCompleteSticker(GameDataHolderAccessor accessor, s32 fileId) {
    return accessor->getGameDataFile(fileId)->calcHaveStickerNum() == getStickerListSize(accessor);
}

const char* getTestClothesName() {
    return "Mario";
}

const char* getTestCapName() {
    return "MarioCap";
}

const char* getTestGiftName() {
    return "GiftDharma";
}

const char* getTestStickerName() {
    return "TestSticker";
}

const char* getTestItemName() {
    return "TestItem";
}

const char* getTestShineName() {
    return "MoonTutorial";
}

const char* getMsTextName(const ShopItem::ItemType& itemType) {
    switch (itemType) {
    case ShopItem::ItemType::Cloth:
        return "ItemCloth";
    case ShopItem::ItemType::Cap:
        return "ItemCap";
    case ShopItem::ItemType::Gift:
        return "ItemGift";
    case ShopItem::ItemType::Sticker:
        return "ItemSticker";
    case ShopItem::ItemType::UseItem:
        return "ItemUseItem";
    case ShopItem::ItemType::Moon:
        return "ItemMoon";
    default:
        return nullptr;
    }
}

const char16* getDisplayName(const al::IUseMessageSystem* user,
                             const ShopItem::ItemInfo& itemInfo) {
    return al::getSystemMessageString(user, getMsTextName(itemInfo.type), itemInfo.name);
}

bool isTypeWear(const ShopItem::ShopItemInfo* itemInfo) {
    return itemInfo->info.type == ShopItem::ItemType::Cloth ||
           itemInfo->info.type == ShopItem::ItemType::Cap;
}

}  // namespace rs
