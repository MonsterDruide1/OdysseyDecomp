#pragma once

#include <basis/seadTypes.h>

namespace ShopItem {

enum class ItemType : u32 { Cloth, Cap, Gift, Sticker, UseItem, Moon };

struct ShopItemInfo {
    s32 mCharacterId;
    s32 mNumberingId;
};

struct ItemInfo {
    s32 mIndex;
    char mName[0x80];
    ItemType mType;
    u32 mItemCount;
    ShopItemInfo* mShopItemInfo;
    bool mIsAOC;
};

}  // namespace ShopItem
