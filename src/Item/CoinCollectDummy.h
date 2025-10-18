#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class CoinCollectHintState;

class CoinCollectDummy : public al::LiveActor {
public:
    CoinCollectDummy(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;

    void appearHint(const sead::Vector3f& position);
    void reappearHint();
    void deleteHint();
    void exeHint();

private:
    CoinCollectHintState* mHintState = nullptr;
};
