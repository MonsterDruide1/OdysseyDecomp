#pragma once

#include "Library/Factory/Factory.h"

namespace al {
class AreaObj;

using AreaCreatorFunction = AreaObj* (*)(const char* areaName);

struct AreaGroupInfo {
    const char* name;
    s32 size;
};

class AreaObjFactory : public Factory<AreaCreatorFunction> {
public:
    AreaObjFactory(const char* factoryName);

    s32 tryFindAddBufferSize(const char*) const;

    template <s32 N>
    inline void setAreaGroupInfos(const AreaGroupInfo (&areaInfos)[N]) {
        mAreaGroupInfos = areaInfos;
        mAreaGroupInfoCount = N;
    }

private:
    const AreaGroupInfo* mAreaGroupInfos = nullptr;
    s32 mAreaGroupInfoCount = 0;
};
}  // namespace al
