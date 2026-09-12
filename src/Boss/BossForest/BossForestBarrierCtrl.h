#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LiveActor;
}

class BossForestBarrierCtrl : public al::NerveExecutor {
public:
    BossForestBarrierCtrl(al::LiveActor* actor);

    void appear();
    void doBreak();
    void doWait();
    al::LiveActor* getBarrierModel() const;
    bool isAppeared() const;

    void exeHidden();
    void exeAppear();
    void exeWait();
    void exeBreak();

private:
    al::LiveActor* mActor = nullptr;
};

static_assert(sizeof(BossForestBarrierCtrl) == 0x18);
