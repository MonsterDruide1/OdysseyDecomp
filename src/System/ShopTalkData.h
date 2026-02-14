#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "System/ByamlSave.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

namespace ShopItem {
struct ShopItemInfo;
}

class GameDataHolderAccessor;

class ShopTalkData : public ByamlSave {
public:
    ShopTalkData(s32, const s32*, s32, const sead::PtrArray<ShopItem::ShopItemInfo>&);
    void init();
    bool isEnableExplainShopCoin(s32) const;
    bool checkShowNewItemAndPrepareSave(const GameDataHolderAccessor&, bool, s32);
    bool checkExistNewItem(const GameDataHolderAccessor&, s32, s32, bool) const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

    bool isInNormalCoinShopFirst() const { return mIsInNormalCoinShopFirst; }

    void resetInNormalCoinShopFirst() { mIsInNormalCoinShopFirst = false; }

    void resetIsEnableExplainShopCoin(s32 worldId) { mIsEnableExplainShopCoin[worldId] = false; }

private:
    bool mIsInNormalCoinShopFirst;
    s32 mSize;
    bool* mIsEnableExplainShopCoin;  // Array of bool of size mSize
    bool* _18;
    bool* _20;
    s32* _28;
    s32 _30;
    ShopItem::ShopItemInfo** _38;
};

static_assert(sizeof(ShopTalkData) == 0x40);
