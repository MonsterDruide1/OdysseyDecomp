#pragma once

#include <basis/seadTypes.h>

namespace alNerveFunction {
class NerveActionCollector;
}  // namespace alNerveFunction

namespace al {
class NerveAction;

class NerveActionCtrl {
public:
    NerveActionCtrl(alNerveFunction::NerveActionCollector* collector);

    NerveAction* findNerve(const char* name) const;

    s32 getNumActions() const { return mNumActions; }

    NerveAction* getAction(s32 idx) const { return mActions[idx]; }

private:
    s32 mNumActions = 0;
    NerveAction** mActions = nullptr;
};

static_assert(sizeof(NerveActionCtrl) == 0x10);

}  // namespace al
