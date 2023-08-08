#include "al/nerve/NerveKeeper.h"
#include "al/nerve/NerveStateCtrl.h"

namespace al {
void NerveKeeper::tryChangeNerve() {
    if (mNerve) {
        if (mStateCtrl) {
            mStateCtrl->tryEndCurrentState();
            mStateCtrl->startState(mNerve);
        }

        _8 = mNerve;
        mNerve = nullptr;
        mStep = 0;
    }
}

void NerveKeeper::setNerve(const al::Nerve* pNerve) {
    if (!(mStep & 0x80000000)) {
        if (_8) {
            _8->executeOnEnd(this);
        }
    }

    mNerve = pNerve;
    mStep = 0xFFFFFFFF;
}

const al::Nerve* NerveKeeper::getCurrentNerve() const {
    const al::Nerve* ret = mNerve;

    if (!mNerve) {
        ret = _8;
    }

    return ret;
}
};  // namespace al