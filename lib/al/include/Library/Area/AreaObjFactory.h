#pragma once

#include "Library/Factory/Factory.h"

namespace al {

class AreaObj;

using AreaObjCreatorFunction = AreaObj* (*)(const char*);

struct AreaGroupInfo {
    const char* name;
    s32 size;
};

class AreaObjFactory : public Factory<AreaObjCreatorFunction> {
public:
    AreaObjFactory(const char* factoryName);
    s32 tryFindAddBufferSize(const char* bufferName) const;

    AreaGroupInfo* getAreaGroupInfo() const { return mAreaGroupInfo; };
    s32 getAreaGroupCount() const { return mNumBuffers; };

    template <s32 N>
    void setAreaGroupInfo(AreaGroupInfo (&areaInfo)[N]) {
        mNumBuffers = N;
        mAreaGroupInfo = areaInfo;
    }

private:
    AreaGroupInfo* mAreaGroupInfo = nullptr;
    s32 mNumBuffers = 0;
};

}  // namespace al
