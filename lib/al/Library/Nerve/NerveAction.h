#pragma once

#include "Library/Nerve/Nerve.h"
#include "Library/Nerve/NerveUtil.h"

namespace al {
class NerveAction : public Nerve {
public:
    NerveAction();
    virtual const char* getActionName() const = 0;

protected:
    friend class alNerveFunction::NerveActionCollector;

private:
    NerveAction* mNextAction = nullptr;
};
}  // namespace al
