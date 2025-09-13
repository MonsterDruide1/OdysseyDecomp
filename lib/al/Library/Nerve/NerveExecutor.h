#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Nerve/IUseNerve.h"

namespace al {
class Nerve;
class NerveKeeper;

class NerveExecutor : public IUseNerve, public HioNode {
public:
    NerveExecutor(const char* name);

    NerveKeeper* getNerveKeeper() const override { return mKeeper; }

    virtual ~NerveExecutor();

    void initNerve(const Nerve* nerve, s32 stateCount);
    void updateNerve();

private:
    NerveKeeper* mKeeper = nullptr;
};
}  // namespace al
