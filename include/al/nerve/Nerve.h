#pragma once

namespace al
{
    class NerveKeeper;

    class IUseNerve
    {
    public:

        inline IUseNerve()
        {
            mKeeper = nullptr;
        }

        virtual NerveKeeper* getNerveKeeper() const = 0;

        al::NerveKeeper* mKeeper; // _8
    };

    class Nerve
    {
    public:
        virtual void execute(NerveKeeper *) = 0;
        virtual void executeOnEnd(NerveKeeper *);
    };
};