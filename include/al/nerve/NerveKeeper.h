#pragma once

#include "al/nerve/Nerve.h"

namespace al
{
    class NerveStateCtrl;
    class NerveKeeper
    {
    public:
        NerveKeeper(al::IUseNerve *, const al::Nerve *, int);

        void update();

        void setNerve(const al::Nerve *);
        al::Nerve* getCurrentNerve() const;

        void tryChangeNerve();

        al::IUseNerve* mParent; // _0
        unsigned long* _8;
        al::Nerve* mNerve; // _10
        int mStep; // _18
        int _1C;
        al::NerveStateCtrl* mStateCtrl; // _20
        unsigned long _28;
    };
};