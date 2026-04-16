#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

class WaterfallWorldWaterfall : public al::LiveActor {
public:
    WaterfallWorldWaterfall(const char* name);

    void init(const al::ActorInitInfo& info) override;
    const sead::Matrix34f* getBaseMtx() const override;

    void changeToMin();
    void changeToMax();
    void exeWaitMax();
    void exeChangeMaxToMin();
    void exeWaitMin();
    void exeChangeMinToMax();

private:
    sead::Matrix34f mBaseMtx = sead::Matrix34f::ident;
};

static_assert(sizeof(WaterfallWorldWaterfall) == 0x138);
