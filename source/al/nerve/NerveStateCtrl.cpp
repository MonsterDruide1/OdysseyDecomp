#include "al/nerve/NerveStateCtrl.h"

namespace al
{
    // todo -- some scheduling problems with mStateCount's incrementation
    void NerveStateCtrl::addState(al::NerveStateBase *pBase, const al::Nerve *pNerve, const char *pName)
    {
        State* state = &mStates[mStateCount];
        state->mStateBase = pBase;
        state->mNerve = pNerve;
        state->mName = pName;
        mStateCount++;
    }

    bool NerveStateCtrl::updateCurrentState()
    {
        if (!mCurrentState)
        {
            return false;
        }

        return mCurrentState->mStateBase->update();
    }

    // UNUSED FUNCTION
    State* al::NerveStateCtrl::findStateInfo(const al::Nerve *pNerve)
    {
        if (mStateCount < 1)
        {
            return nullptr;
        }

        int curIdx = 0;

        while (curIdx < mStateCount)
        {
            if (mStates[curIdx].mNerve == pNerve)
            {
                return &mStates[curIdx];
            }

            curIdx++;
        }

        return nullptr;
    }

    bool NerveStateCtrl::isCurrentStateEnd() const
    {
        if (!mCurrentState)
        {
            return true;
        }

        return mCurrentState->mStateBase->mIsDead != 0;
    }

    void NerveStateCtrl::tryEndCurrentState()
    {
        if (mCurrentState)
        {
            if (!mCurrentState->mStateBase->mIsDead)
            {
                mCurrentState->mStateBase->kill();
            }

            al::NerveKeeper* keeper = mCurrentState->mStateBase->getNerveKeeper();

            if (keeper)
            {
                al::NerveStateCtrl* ctrl = keeper->mStateCtrl;
                            
                if (ctrl)
                {
                    ctrl->tryEndCurrentState();
                }
            }

            mCurrentState = nullptr;
        }
    }
};