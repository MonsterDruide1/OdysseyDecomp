#include "Library/Nerve/NerveActionCtrl.h"

#include "Library/Base/StringUtil.h"
#include "Library/Nerve/NerveAction.h"

namespace al {

NerveActionCtrl::NerveActionCtrl(alNerveFunction::NerveActionCollector* collector) {
    mNumActions = collector->getNumActions();
    mActions = new NerveAction*[mNumActions];

    if (mNumActions > 0) {
        NerveAction* current = collector->getHead();
        mActions[0] = current;

        for (s32 i = 1; i < mNumActions; ++i) {
            // requires doing this pattern to calculate mActions[i] first
            NerveAction** next = &mActions[i];
            current = current->getNext();
            *next = current;
        }
    }
}

NerveAction* NerveActionCtrl::findNerve(const char* name) const {
    for (s32 i = 0; i < mNumActions; ++i) {
        NerveAction* action = mActions[i];
        if (isEqualString(action->getActionName(), name))
            return action;
    }

    return nullptr;
}

}  // namespace al
