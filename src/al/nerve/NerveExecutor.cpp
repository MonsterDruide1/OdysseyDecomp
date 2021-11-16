#include "al/nerve/NerveExecutor.h"

namespace al {

NerveExecutor::NerveExecutor(const char* pName) : IUseNerve() {
    mKeeper = 0;
}

NerveExecutor::~NerveExecutor() {
    delete mKeeper;
}

NerveKeeper* NerveExecutor::getNerveKeeper() const {
    return mKeeper;
}

void NerveExecutor::initNerve(const al::Nerve* pNerve, int nerveCount) {
    mKeeper = new NerveKeeper(this, pNerve, nerveCount);
}

void NerveExecutor::updateNerve() {
    if (mKeeper)
        mKeeper->update();
}
};  // namespace al