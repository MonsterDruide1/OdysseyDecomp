#pragma once

#include <container/seadPtrArray.h>

#include "System/ByamlSave.h"

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
    void read(const al::ByamlIter& iter) override;

private:
    void* _padding[0x7];
};

static_assert(sizeof(ShopTalkData) == 0x40);
