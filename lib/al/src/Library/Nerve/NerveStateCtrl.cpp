#include "Library/Nerve/NerveStateCtrl.h"

#include "Library/Nerve/NerveKeeper.h"

namespace al {
// todo -- some scheduling problems with mStateCount's incrementation
// adds a state to the list of states in the controller
void NerveStateCtrl::addState(NerveStateBase* state, const Nerve* nerve, const char* name) {
    mStates[mStateCount] = {.state = state, .nerve = nerve, .name = name};
    mStateCount++;
}

// run the state's update function, if there is a current state active
bool NerveStateCtrl::updateCurrentState() {
    if (!mCurrentState) {
        return false;
    }

    return mCurrentState->state->update();
}

// UNUSED FUNCTION
// uses a supplied nerve pointer to compare it with the nerves contained in states
// returns the matching nerve, if any
NerveStateCtrl::State* NerveStateCtrl::findStateInfo(const Nerve* nerve) {
    for (s32 i = 0; i < mStateCount; i++) {
        if (mStates[i].nerve == nerve) {
            return &mStates[i];
        }
    }

    return nullptr;
}

// determines if the current state on the controller has ended
// this can occur if there is no state, or if the base is not considered dead
bool NerveStateCtrl::isCurrentStateEnd() const {
    if (!mCurrentState) {
        return true;
    }

    return mCurrentState->state->isDead() != 0;
}

// attempt to end the currently active state by "killing" the state, then killing the state
// controller contained in the nerve keeper
void NerveStateCtrl::tryEndCurrentState() {
    if (mCurrentState) {
        if (!mCurrentState->state->isDead()) {
            mCurrentState->state->kill();
        }

        NerveKeeper* keeper = mCurrentState->state->getNerveKeeper();

        if (keeper) {
            NerveStateCtrl* ctrl = keeper->getStateCtrl();

            if (ctrl) {
                ctrl->tryEndCurrentState();
            }
        }

        mCurrentState = nullptr;
    }
}
}  // namespace al
