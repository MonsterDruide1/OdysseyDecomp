#pragma once

#include "al/nerve/Nerve.h"

namespace al
{
    class NerveKeeper
    {
    public:
        NerveKeeper(al::IUseNerve *, const al::Nerve *, int);

        void update();

        al::IUseNerve* mParent; // _0
        unsigned long* _8;
        al::Nerve* mNerve; // _10
        int mNerveCount; // _18
        int _1C;
        unsigned long* _20; // NerveStateCtrl*
        unsigned long _28;
    };
};