#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class ModelKeeper;

class ModelGroup : public HioNode {
public:
    ModelGroup(s32 maxModels);
    virtual ~ModelGroup();

    void registerModel(ModelKeeper*);

private:
    void* filler[2];
};

static_assert(sizeof(ModelGroup) == 0x18);

}  // namespace al
