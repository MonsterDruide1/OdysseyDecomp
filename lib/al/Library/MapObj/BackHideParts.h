#pragma once

#include "Library/Anim/DitherAnimator.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
class BackHideDitherAnimator : public DitherAnimator {
public:
    BackHideDitherAnimator(LiveActor* actor);

    void update() override;

private:
    LiveActor* mActor;
};

class BackHideParts : public LiveActor {
public:
    BackHideParts(const char* name);

    void init(const ActorInitInfo& info) override;

private:
    BackHideDitherAnimator* mBackHideDitherAnimator = nullptr;
};
}  // namespace al
