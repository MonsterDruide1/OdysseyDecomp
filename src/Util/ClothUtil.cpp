#include "Util/ClothUtil.h"

#include "System/GameDataHolder.h"
#include "System/GameDataHolderAccessor.h"

const sead::PtrArray<ShopItem::ItemInfo>& rs::getClothList(GameDataHolderAccessor accessor) {
    return accessor->getClothList();
}

const sead::PtrArray<ShopItem::ItemInfo>& rs::getCapList(GameDataHolderAccessor accessor) {
    return accessor->getCapList();
}

const sead::PtrArray<ShopItem::ItemInfo>& rs::getGiftList(GameDataHolderAccessor accessor) {
    return accessor->getGiftList();
}

const sead::PtrArray<ShopItem::ItemInfo>& rs::getStickerList(GameDataHolderAccessor accessor) {
    return accessor->getStickerList();
}
