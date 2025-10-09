#pragma once

#include "Library/HostIO/HioNode.h"

namespace al {
class AnimInfoTable;

class AnimPlayerBase : public HioNode {
public:
    AnimPlayerBase();

    virtual void updateLast() { _10 = false; }

    virtual bool calcNeedUpdateAnimNext() = 0;

    AnimInfoTable* getAnimInfoTable() { return mInfoTable; }

private:
    AnimInfoTable* mInfoTable = nullptr;
    bool _10 = false;
    bool _11 = false;
};

static_assert(sizeof(AnimPlayerBase) == 0x18);

}  // namespace al
