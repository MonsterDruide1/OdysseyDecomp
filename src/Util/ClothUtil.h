#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "System/GameDataHolderAccessor.h"

namespace al {
class IUseSceneObjHolder;
class IUseMessageSystem;
}  // namespace al

namespace ShopItem {
struct ItemInfo;
enum class ItemType : u32;
struct ShopItemInfo;
}  // namespace ShopItem

namespace rs {
void buyItemInShopItemList(const al::IUseSceneObjHolder* user, s32 itemIdx);
bool isBuyItemInShopItemList(const al::IUseSceneObjHolder* user, s32 itemIdx);
void buyItem(const al::IUseSceneObjHolder* user, const ShopItem::ItemInfo* itemInfo);
void buyItemAll(const al::IUseSceneObjHolder* user);
bool isBuyItem(GameDataHolderAccessor accessor, const ShopItem::ItemInfo* itemInfo);
bool isHaveCloth(GameDataHolderAccessor accessor, const char* name);
bool isHaveCap(GameDataHolderAccessor accessor, const char* name);
bool isHaveSticker(GameDataHolderAccessor accessor, const char* name);
bool isHaveGift(GameDataHolderAccessor accessor, const char* name);
const sead::PtrArray<ShopItem::ItemInfo>& getClothList(GameDataHolderAccessor accessor);
s32 getClothListSize(GameDataHolderAccessor accessor);
bool isHaveCloth(GameDataHolderAccessor accessor, s32 clothIdx);
bool isHaveClothByFileId(GameDataHolderAccessor accessor, const char* clothName, s32 fileId);
s32 calcHaveClothNum(GameDataHolderAccessor accessor);
void buyCloth(const al::IUseSceneObjHolder* user, const char* clothName);
const sead::PtrArray<ShopItem::ItemInfo>& getCapList(GameDataHolderAccessor accessor);
s32 getCapListSize(GameDataHolderAccessor accessor);
bool isHaveCap(GameDataHolderAccessor accessor, s32 capIdx);
bool isHaveCapByFileId(GameDataHolderAccessor accessor, const char* capName, s32 fileId);
bool isCompleteClothCap(const al::IUseSceneObjHolder* user);
bool isCompleteCloth(const al::IUseSceneObjHolder* user);
bool isCompleteCap(const al::IUseSceneObjHolder* user);
bool isCompleteCloth(const al::IUseSceneObjHolder* user, s32 fileId);
bool isCompleteCap(const al::IUseSceneObjHolder* user, s32 fileId);
s32 calcHaveCapNum(GameDataHolderAccessor accessor);
void buyCap(const al::IUseSceneObjHolder* user, const char* capName);
const sead::PtrArray<ShopItem::ItemInfo>& getGiftList(GameDataHolderAccessor accessor);
s32 getGiftListSize(GameDataHolderAccessor accessor);
bool isHaveGift(GameDataHolderAccessor accessor, s32 giftIdx);
s32 calcHaveGiftNum(GameDataHolderAccessor accessor);
bool checkCompleteGift(GameDataHolderAccessor accessor, s32 fileId);
const sead::PtrArray<ShopItem::ItemInfo>& getStickerList(GameDataHolderAccessor accessor);
s32 getStickerListSize(GameDataHolderAccessor accessor);
bool isHaveSticker(GameDataHolderAccessor accessor, s32 fileId);
s32 calcHaveStickerNum(GameDataHolderAccessor accessor);
bool checkCompleteSticker(GameDataHolderAccessor accessor, s32 fileId);
const char* getTestClothesName();
const char* getTestCapName();
const char* getTestGiftName();
const char* getTestStickerName();
const char* getTestItemName();
const char* getTestShineName();
const char* getMsTextName(const ShopItem::ItemType& itemType);
const char16* getDisplayName(const al::IUseMessageSystem* user, const ShopItem::ItemInfo& itemInfo);
bool isTypeWear(const ShopItem::ShopItemInfo* itemInfo);
}  // namespace rs
