#include "Library/Area/AreaObjFactory.h"

#include "Library/Base/StringUtil.h"

namespace al {

AreaObjFactory::AreaObjFactory(const char* factoryName)
    : Factory<AreaCreatorFunction>(factoryName) {}

s32 AreaObjFactory::tryFindAddBufferSize(const char* bufferName) const {
    if (!mAreaGroupInfos || mAreaGroupInfoCount < 1)
        return 0;

    for (s32 i = 0; i < mAreaGroupInfoCount; i++)
        if (isEqualString(bufferName, mAreaGroupInfos[i].name))
            return mAreaGroupInfos[i].size;
    return 0;
}

}  // namespace al
