#include "Util/ClothUtil.h"

#include "System/GameDataHolder.h"

const sead::PtrArray<ItemListInfo>& rs::getClothList(GameDataHolderAccessor accessor) {
    return accessor->getClothList();
}

const sead::PtrArray<ItemListInfo>& rs::getCapList(GameDataHolderAccessor accessor) {
    return accessor->getCapList();
}

const sead::PtrArray<ItemListInfo>& rs::getGiftList(GameDataHolderAccessor accessor) {
    return accessor->getGiftList();
}

const sead::PtrArray<ItemListInfo>& rs::getStickerList(GameDataHolderAccessor accessor) {
    return accessor->getStickerList();
}
