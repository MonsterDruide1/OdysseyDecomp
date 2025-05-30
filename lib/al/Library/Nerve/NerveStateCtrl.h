#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/Nerve.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {

class NerveStateCtrl {
public:
    struct State {
        NerveStateBase* state;
        const Nerve* nerve;
        const char* name;
    };

    NerveStateCtrl(s32 maxStates);

    void addState(NerveStateBase* state, const Nerve* nerve, const char* name);
    bool updateCurrentState();
    void startState(const Nerve* nerve);
    void update();

    State* findStateInfo(const Nerve* nerve);
    bool isCurrentStateEnd() const;
    void tryEndCurrentState();

private:
    s32 mMaxStates;
    s32 mStateCount = 0;
    State* mStates = nullptr;
    State* mCurrentState = nullptr;
};
}  // namespace al
