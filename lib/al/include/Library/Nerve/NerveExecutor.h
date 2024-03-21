#pragma once

#include <basis/seadTypes.h>

#include "Library/Action/ActorActionKeeper.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Nerve/IUseNerve.h"

namespace alNerveFunction {
class NerveActionCollector;
}  // namespace alNerveFunction

namespace al {
class Nerve;
class NerveKeeper;

class NerveActionCtrl {
    NerveActionCtrl(alNerveFunction::NerveActionCollector*);

    NerveKeeper* findNerve(const char*) const;
};

class NerveExecutor : public IUseNerve, public HioNode {
public:
    NerveExecutor(const char* name);

    virtual NerveKeeper* getNerveKeeper() const;
    virtual ~NerveExecutor();

    void initNerve(const Nerve* nerve, s32 stateCount);
    void updateNerve();

private:
    NerveKeeper* mKeeper = nullptr;
};
}  // namespace al
