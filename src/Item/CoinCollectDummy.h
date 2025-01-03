#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class CoinCollectHintState;

class CoinCollectDummy : public al::LiveActor {
public:
    CoinCollectDummy(const char*);

    void init(const al::ActorInitInfo&) override;
    void appear() override;

    void appearHint(const sead::Vector3f&);
    void reappearHint();
    void deleteHint();
    void exeHint();

private:
    CoinCollectHintState* mHintState = nullptr;
};
