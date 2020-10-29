#include "al/util/NerveUtil.h"
#include "al/nerve/NerveKeeper.h"

namespace al
{
    void setNerve(al::IUseNerve *pKeeper, const al::Nerve *pNerve)
    {
        pKeeper->getNerveKeeper()->setNerve(pNerve);
    }

    void setNerveAtStep(al::IUseNerve *pKeeper, const al::Nerve *pNerve, int step)
    {
        if (pKeeper->getNerveKeeper()->mStep == step)
        {
            pKeeper->getNerveKeeper()->setNerve(pNerve);
        }
    }
    bool isStep(const al::IUseNerve *pKeeper, int step)
    {
        return pKeeper->getNerveKeeper()->mStep == step;
    }

    void setNerveAtGreaterEqualStep(al::IUseNerve *pKeeper, const al::Nerve *pNerve, int step)
    {
        if (pKeeper->getNerveKeeper()->mStep >= step)
        {
            pKeeper->getNerveKeeper()->setNerve(pNerve);
        }
    }

    bool isGreaterEqualStep(const al::IUseNerve *pKeeper, int step)
    {
        return pKeeper->getNerveKeeper()->mStep >= step;
    }

    bool isNerve(const al::IUseNerve *pKeeper, const al::Nerve *pNerve)
    {
        return pKeeper->getNerveKeeper()->getCurrentNerve() == pNerve;
    }

    int getNerveStep(const al::IUseNerve *pKeeper)
    {
        return pKeeper->getNerveKeeper()->mStep;
    }

    al::Nerve* getCurrentNerve(const al::IUseNerve *pKeeper)
    {
        return pKeeper->getNerveKeeper()->getCurrentNerve();
    }

    bool isFirstStep(const al::IUseNerve *pKeeper)
    {
        return pKeeper->getNerveKeeper()->mStep == 0;
    }

    bool isLessStep(const al::IUseNerve *pKeeper, int step)
    {
        return pKeeper->getNerveKeeper()->mStep < step;
    }

    bool isLessEqualStep(const al::IUseNerve *pKeeper, int step)
    {
        return pKeeper->getNerveKeeper()->mStep <= step;
    }

    bool isGreaterStep(const al::IUseNerve *pKeeper, int step)
    {
        return pKeeper->getNerveKeeper()->mStep > step;
    }

    bool isInRangeStep(const al::IUseNerve *pKeeper, int low, int high)
    {
        int step = pKeeper->getNerveKeeper()->mStep;
        return (step <= high) & (step >= low);
    }

    // logic just won't add up for some reason
    int calcNerveInterval(const al::IUseNerve *pKeeper, int start, int end)
    {
        al::NerveKeeper* keeper = pKeeper->getNerveKeeper();

        if (start >= -1)
        {
            int dist = keeper->mStep - end;

            if (dist >= -1)
            {
                return dist / start;
            }
        }

        return 0;
    }

    float calcNerveRate(const al::IUseNerve *pKeeper, int step)
    {
        float ret;

        if (step < 1)
        {
            ret = 1.0f;
        }
        else
        {
            float curStep = pKeeper->getNerveKeeper()->mStep;
            ret = curStep / step;

            // TODO -- technically the same code,
            // but this one uses B.GE and not B.PL
            if (ret >= 0)
            {
                if (ret > 1.0f)
                {
                    ret = 1.0f;
                }
            }
            else
            {
                ret = 0;
            }
        }

        return ret;
    }
}