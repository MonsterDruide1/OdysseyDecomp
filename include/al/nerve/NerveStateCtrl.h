#pragma once

#include "al/nerve/Nerve.h"
#include "al/nerve/NerveStateBase.h"

namespace al
{
    struct State
    {
        al::NerveStateBase* mStateBase; // _0
        const al::Nerve* mNerve; // _8
        const char* mName; // _10
    };

    class NerveStateCtrl
    {
    public:
        NerveStateCtrl(int);

        void addState(al::NerveStateBase *, const al::Nerve *, const char *);
        void update();

        bool isCurrentStateEnd() const;

        int _0;
        int _4;
        State** mStates; // _8
        int mCurrentState; // _10
        int _14;
    };
};