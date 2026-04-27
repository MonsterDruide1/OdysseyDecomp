#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>

#include "Library/LiveActor/LiveActor.h"

struct FastenerVertex;

class FastenerKnob : public al::LiveActor {
public:
    FastenerKnob(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void exeWait();
    void exeHackEnd();
    void onHackEnd(const sead::Quatf& quat, const FastenerVertex* vertex);
    void reset();
    void disappear();
    void exeReset();
    bool isEnableCapture() const;

private:
    u64 _108 = 0;
    bool mIsUnknown110 = false;
    const FastenerVertex* mStartVertex = nullptr;
    const FastenerVertex* mCurrentVertex = nullptr;
    sead::Quatf mResetQuat = sead::Quatf::unit;
};

static_assert(sizeof(FastenerKnob) == 0x138);
