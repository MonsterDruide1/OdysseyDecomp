#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class CoinBlow : public al::LiveActor {
public:
    CoinBlow(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;

    void listenStart();

private:
    const char* mBlowSize = nullptr;
};
