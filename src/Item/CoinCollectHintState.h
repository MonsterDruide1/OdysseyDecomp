#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class CoinCollectHintState : public al::ActorStateBase {
public:
    CoinCollectHintState(al::LiveActor* actor);

    void init() override;
    void appear() override;
    void kill() override;

    void deleteHintEffect();
    void appearHintEffect();

    void exeWait();
};
