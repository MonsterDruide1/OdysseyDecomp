#include "Library/Area/AreaObjFactory.h"

#include "Library/Base/String.h"

namespace al {

AreaObjFactory::AreaObjFactory(const char* factoryName)
    : Factory<AreaObjCreatorFunction>(factoryName) {}

s32 AreaObjFactory::tryFindAddBufferSize(const char* bufferName) const {
    if (mAreaGroupInfo == nullptr || mNumBuffers < 1)
        return 0;

    for (s32 i = 0; i < mNumBuffers; i++) {
        if (isEqualString(bufferName, mAreaGroupInfo[i].name))
            return mAreaGroupInfo[i].size;
    }
    return 0;
}

}  // namespace al
