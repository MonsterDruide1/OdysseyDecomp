#include "al/nerve/NerveStateBase.h"

namespace al
{
    NerveStateBase::NerveStateBase(const char *pName) : NerveExecutor(pName)
    {
        mIsDead = true;
    }

    NerveStateBase::~NerveStateBase()
    {
        delete this;
    }

    void NerveStateBase::init()
    {

    }

    void NerveStateBase::appear()
    {
        mIsDead = false;
    }

    void NerveStateBase::kill()
    {
        mIsDead = true;
    }

    bool NerveStateBase::update()
    {
        updateNerve();

        if (mIsDead)
        {
            return true;
        }

        control();

        return mIsDead;
    }

    void NerveStateBase::control()
    {
        
    }
};