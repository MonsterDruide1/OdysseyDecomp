#include "al/Library/Nerve/NerveStateCtrl.h"

namespace al {
// todo -- some scheduling problems with mStateCount's incrementation
// adds a state to the list of states in the controller
void NerveStateCtrl::addState(al::NerveStateBase* pBase, const al::Nerve* pNerve,
                              const char* pName) {
    auto& state = mStates[mStateCount];
    state.mStateBase = pBase;
    state.mNerve = pNerve;
    state.mName = pName;
    mStateCount++;
}

// run the state's update function, if there is a current state active
bool NerveStateCtrl::updateCurrentState() {
    if (!mCurrentState) {
        return false;
    }

    return mCurrentState->mStateBase->update();
}

// UNUSED FUNCTION
// uses a supplied nerve pointer to compare it with the nerves contained in states
// returns the matching nerve, if any
State* al::NerveStateCtrl::findStateInfo(const al::Nerve* pNerve) {
    if (mStateCount < 1) {
        return nullptr;
    }

    int curIdx = 0;

    while (curIdx < mStateCount) {
        if (mStates[curIdx].mNerve == pNerve) {
            return &mStates[curIdx];
        }

        curIdx++;
    }

    return nullptr;
}

// determines if the current state on the controller has ended
// this can occur if there is no state, or if the base is not considered dead
bool NerveStateCtrl::isCurrentStateEnd() const {
    if (!mCurrentState) {
        return true;
    }

    return mCurrentState->mStateBase->mIsDead != 0;
}

// attempt to end the currently active state by "killing" the state, then killing the state
// controller contained in the nerve keeper
void NerveStateCtrl::tryEndCurrentState() {
    if (mCurrentState) {
        if (!mCurrentState->mStateBase->mIsDead) {
            mCurrentState->mStateBase->kill();
        }

        al::NerveKeeper* keeper = mCurrentState->mStateBase->getNerveKeeper();

        if (keeper) {
            al::NerveStateCtrl* ctrl = keeper->mStateCtrl;

            if (ctrl) {
                ctrl->tryEndCurrentState();
            }
        }

        mCurrentState = nullptr;
    }
}
};  // namespace al
