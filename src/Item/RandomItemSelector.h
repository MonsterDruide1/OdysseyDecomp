#pragma once

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"
#include "Util/ItemUtil.h"

namespace al {
class IUseSceneObjHolder;
}

class RandomItemSelector : public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_RandomItemSelector;

    RandomItemSelector();
    rs::ItemType::ValueType getRandomItemType(const al::IUseSceneObjHolder* user);

private:
    struct ItemList {
        s32 itemCount = 0;
        rs::ItemType::ValueType* itemTypes = nullptr;
    };

    ItemList* mItemLists = nullptr;
    s32 mItemIndex = 0;
};

static_assert(sizeof(RandomItemSelector) == 0x18);

namespace rs {
RandomItemSelector* createRandomItemSelector(const al::IUseSceneObjHolder* user);
}  // namespace rs
