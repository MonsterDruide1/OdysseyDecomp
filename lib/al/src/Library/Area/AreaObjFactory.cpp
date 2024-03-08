#include "Library/Area/AreaObjFactory.h"

#include "Library/Base/String.h"

namespace al {

AreaObjFactory::AreaObjFactory(const char* factoryName)
    : Factory<AreaObjCreatorFunction>(factoryName) {}

s32 AreaObjFactory::tryFindAddBufferSize(const char* bufferName) const {
    if (mAreaGroupInfo == nullptr || mNumBuffers < 1)
        return 0;

    s32 offset = 0;
    while (!isEqualString(bufferName, mAreaGroupInfo[offset].name)) {
        if (++offset >= mNumBuffers)
            return 0;
    }
    return mAreaGroupInfo[offset].size;
}

}  // namespace al
