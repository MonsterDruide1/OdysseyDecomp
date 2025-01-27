#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SubActorLodExecutor;

class SubActorLodMapParts : public LiveActor {
public:
    SubActorLodMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void control() override;
    void movement() override;
    void calcAnim() override;

private:
    SubActorLodExecutor* mSubActorLodExecutor = nullptr;
    bool mIsControlled = false;
};

static_assert(sizeof(SubActorLodMapParts) == 0x118);
}  // namespace al
