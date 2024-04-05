#pragma once

#include "Library/HostIO/HioNode.h"

namespace al {
class AnimInfoTable;

class AnimPlayerBase : public HioNode {
public:
    AnimPlayerBase();

    virtual void updateLast();

    AnimInfoTable* getAnimInfoTable() { return mInfoTable; }

private:
    AnimInfoTable* mInfoTable = nullptr;
    bool _10 = false;
    bool _11 = false;
};
}  // namespace al
