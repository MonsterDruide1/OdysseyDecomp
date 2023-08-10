#include "al/Library/Nerve/NerveAction.h"

#include "al/Library/Nerve/NerveUtil.h"

namespace al {
NerveAction::NerveAction() {
    alNerveFunction::NerveActionCollector* collector =
        alNerveFunction::NerveActionCollector::sCurrentCollector;
    if (!collector->mHead)
        collector->mHead = this;
    else
        collector->mTail->mNextAction = this;
    collector->mTail = this;
    collector->mActionCount++;
}
}  // namespace al

namespace alNerveFunction {

NerveActionCollector::NerveActionCollector() {
    sCurrentCollector = this;
}
void NerveActionCollector::addNerve(al::NerveAction* action) {
    if (!mHead)
        mHead = action;
    else
        mTail->mNextAction = action;
    mTail = action;
    mActionCount++;
}

}  // namespace alNerveFunction
