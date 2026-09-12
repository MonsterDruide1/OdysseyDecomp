#include "Item/RandomItemSelector.h"

#include "Library/Base/StringUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Yaml/ByamlIter.h"

#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"

RandomItemSelector::RandomItemSelector() {
    const char* itemName;
    al::Resource* resource = al::findOrCreateResource("SystemData/ItemList", nullptr);
    al::ByamlIter iter(al::findResourceYaml(resource, "RandomItemList", nullptr));
    mItemLists = new ItemList[6];
    for (s32 i = 0; i < 6; i++) {
        al::ByamlIter listIter;
        iter.tryGetIterByKey(&listIter, al::StringTmp<64>("Diff%d", i).cstr());
        s32 listSize = listIter.getSize();
        mItemLists[i].itemCount = listSize;
        mItemLists[i].itemTypes = new rs::ItemType::ValueType[listSize];
        rs::ItemType::ValueType* itemTypes = mItemLists[i].itemTypes;
        s32 itemCount = listIter.getSize();
        for (s32 j = 0; j < itemCount; j++) {
            listIter.tryGetStringByIndex(&itemName, j);
            if (al::isEqualString(itemName, "Coin"))
                itemTypes[j] = rs::ItemType::Coin;
            else if (al::isEqualString(itemName, "LifeUpItem"))
                itemTypes[j] = rs::ItemType::LifeUpItem;
        }
    }
    mItemIndex = al::getRandom(100);
}

rs::ItemType::ValueType RandomItemSelector::getRandomItemType(const al::IUseSceneObjHolder* user) {
    s32 hitPoint;
    {
        GameDataHolderAccessor accessor(user);
        hitPoint = GameDataFunction::getPlayerHitPoint(accessor);
    }
    s32 maxHitPoint;
    {
        GameDataHolderAccessor accessor(user);
        maxHitPoint = GameDataFunction::getPlayerHitPointMaxCurrent(accessor);
    }
    s32 missingHitPoint = maxHitPoint - hitPoint;
    if (missingHitPoint == 0)
        return rs::ItemType::Coin;

    const ItemList& list = mItemLists[missingHitPoint];
    s32 itemCount = list.itemCount;
    return list.itemTypes[mItemIndex++ % itemCount];
}

namespace rs {
RandomItemSelector* createRandomItemSelector(const al::IUseSceneObjHolder* user) {
    return al::createSceneObj<RandomItemSelector>(user);
}
}  // namespace rs
