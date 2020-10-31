#pragma once

#include "al/nerve/Nerve.h"
#include "al/nerve/NerveKeeper.h"

namespace al
{
    class NerveExecutor : public IUseNerve
    {
    public:
        NerveExecutor(const char *);

        virtual NerveKeeper* getNerveKeeper() const;
        virtual ~NerveExecutor();

        void initNerve(const al::Nerve *, int);
        void updateNerve();

        al::NerveKeeper* mKeeper; // _8
    };
};