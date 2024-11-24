#include "Util/ClothUtil.h"

#include "System/GameDataHolder.h"

const sead::PtrArray<ItemListInfo>& rs::getClothList(GameDataHolderAccessor accessor) {
    return accessor.mData->getClothList();
}

const sead::PtrArray<ItemListInfo>& rs::getCapList(GameDataHolderAccessor accessor) {
    return accessor.mData->getCapList();
}

const sead::PtrArray<ItemListInfo>& rs::getGiftList(GameDataHolderAccessor accessor) {
    return accessor.mData->getGiftList();
}

const sead::PtrArray<ItemListInfo>& rs::getStickerList(GameDataHolderAccessor accessor) {
    return accessor.mData->getStickerList();
}
