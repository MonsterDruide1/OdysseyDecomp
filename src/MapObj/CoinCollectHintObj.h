#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class CoinCollectHintObj : public al::LiveActor {
public:
    CoinCollectHintObj(const char* name);

    void init(const al::ActorInitInfo& info) override;

private:
    const char* mStageName = nullptr;
    sead::Vector3f mTrans = sead::Vector3f::zero;
};
