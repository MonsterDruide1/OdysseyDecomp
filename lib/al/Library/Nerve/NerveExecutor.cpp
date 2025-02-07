#include "Library/Nerve/NerveExecutor.h"

#include "Library/Nerve/NerveKeeper.h"

namespace al {

NerveExecutor::NerveExecutor(const char* name) {}

NerveExecutor::~NerveExecutor() {
    delete mKeeper;
}

NerveKeeper* NerveExecutor::getNerveKeeper() const {
    return mKeeper;
}

void NerveExecutor::initNerve(const Nerve* nerve, s32 stateCount) {
    mKeeper = new NerveKeeper(this, nerve, stateCount);
}

void NerveExecutor::updateNerve() {
    if (mKeeper)
        mKeeper->update();
}
}  // namespace al
