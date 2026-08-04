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
    rs::ItemType::ValueType getRandomItemType(const al::IUseSceneObjHolder*);

private:
    char filler[0x10];
};

static_assert(sizeof(RandomItemSelector) == 0x18);

namespace rs {
void createRandomItemSelector(const al::IUseSceneObjHolder*);
}
