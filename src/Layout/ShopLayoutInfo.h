#pragma once

#include <basis/seadTypes.h>
#include <container/seadBuffer.h>

namespace ShopItem {

enum class ItemType : s32 {
    Invalid = -1,
    Cloth,
    Cap,
    Gift,
    Sticker,
    UseItem,
    Moon,
};

enum class CoinType : s32 {
    Invalid = -1,
    Coin,
    Collect,
};

struct ShopAmiiboInfo {
    s32 characterId = -1;
    s32 numberingId = -1;
};

static_assert(sizeof(ShopAmiiboInfo) == 0x8);

struct ItemInfo {
    ItemInfo() { name[0] = '\0'; }

    s32 index = -1;
    char name[0x80];
    ItemType type = ItemType::Invalid;
    sead::Buffer<ShopAmiiboInfo> amiiboInfoList;
    bool isAOC = false;
};

static_assert(sizeof(ItemInfo) == 0xa0);

struct ShopItemInfo {
    ShopItemInfo() {
        storeName[0] = '\0';
        clearWorld[0] = '\0';
    }

    ItemInfo info = {};
    s32 price = -1;
    CoinType coinType = CoinType::Invalid;
    char storeName[0x80];
    char clearWorld[0x80];
    s32 moonNum = -1;
};

static_assert(sizeof(ShopItemInfo) == 0x1b0);

}  // namespace ShopItem
