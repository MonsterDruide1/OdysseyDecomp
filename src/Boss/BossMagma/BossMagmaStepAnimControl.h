#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LiveActor;
}  // namespace al

class BossMagmaStepAnimControl : public al::NerveExecutor {
public:
    BossMagmaStepAnimControl(al::LiveActor* actor);

    void requestUp();
    void requestDown();
    void requestDownImm();
    void exeWait();
    void exeUp();
    void exeDownSign();
    void exeDown();

private:
    al::LiveActor* mActor;
};

static_assert(sizeof(BossMagmaStepAnimControl) == 0x18);
