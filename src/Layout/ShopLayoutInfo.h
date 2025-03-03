#pragma once

#include <basis/seadTypes.h>

namespace ShopItem {

enum class ItemType : u32 { Cloth, Cap, Gift, Sticker, UseItem, Moon };

struct ShopItemInfo {
    s32 characterId;
    s32 numberingId;
};

struct ItemInfo {
    s32 index;
    char name[0x80];
    ItemType type;
    u32 itemCount;
    ShopItemInfo* shopItemInfo;
    bool isAOC;
};

}  // namespace ShopItem
