#pragma once

#include "Library/Scene/ISceneObj.h"

#include "Util/ItemUtil.h"

namespace al {
class IUseSceneObjHolder;
}

class RandomItemSelector : public al::ISceneObj {
public:
    RandomItemSelector();
    rs::ItemType::ValueType getRandomItemType();

private:
    char filler[0x10];
};

static_assert(sizeof(RandomItemSelector) == 0x18);

namespace rs {
void createRandomItemSelector(const al::IUseSceneObjHolder*);
}
