#pragma once

#include "Library/Nerve/Nerve.h"

namespace alNerveFunction {
class NerveActionCollector;
}

namespace al {
class NerveAction : public Nerve {
public:
    NerveAction();
    virtual const char* getActionName() const = 0;

    NerveAction* getNext() const { return mNextAction; }

protected:
    friend class alNerveFunction::NerveActionCollector;

private:
    NerveAction* mNextAction = nullptr;
};
}  // namespace al

namespace alNerveFunction {

class NerveActionCollector {
public:
    NerveActionCollector();

    void addNerve(al::NerveAction* action);

    s32 getNumActions() const { return mActionCount; }

    al::NerveAction* getHead() { return mHead; }

protected:
    friend class al::NerveAction;

private:
    s32 mActionCount = 0;
    al::NerveAction* mHead = nullptr;
    al::NerveAction* mTail = nullptr;

    static NerveActionCollector* sCurrentCollector;
};

}  // namespace alNerveFunction
