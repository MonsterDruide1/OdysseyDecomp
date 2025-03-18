#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class CoinStateCountUp : public al::ActorStateBase {
public:
    CoinStateCountUp(al::LiveActor* actor);
    void appear() override;
    void exeCountUp();
    void exeGot();

private:
    f32 mTransY = 0.0f;
};
