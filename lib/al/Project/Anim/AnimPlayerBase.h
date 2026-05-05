#pragma once

#include "Library/HostIO/HioNode.h"

namespace al {
class AnimInfoTable;

class AnimPlayerBase : public HioNode {
public:
    AnimPlayerBase();

    virtual void updateLast() { _10 = false; }

    virtual bool calcNeedUpdateAnimNext() = 0;

    AnimInfoTable* getAnimInfoTable() const { return mInfoTable; }

    bool is10() const { return _10; };

    void set10(bool value) { _10 = value; }

    bool is11() const { return _11; };

    void set11(bool value) { _11 = value; }

private:
    AnimInfoTable* mInfoTable = nullptr;
    bool _10 = false;
    bool _11 = false;
};

static_assert(sizeof(AnimPlayerBase) == 0x18);

}  // namespace al
