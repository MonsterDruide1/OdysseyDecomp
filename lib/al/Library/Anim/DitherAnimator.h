#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LiveActor;

class DitherAnimator : public NerveExecutor {
public:
    DitherAnimator(const char* name) : NerveExecutor(name) {}

    virtual void update() = 0;

    virtual void show() {}

    virtual bool isValidNearClip() const { return false; }
};
}  // namespace al
