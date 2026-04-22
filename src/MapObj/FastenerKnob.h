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
    u64 _108;
    bool mIsUnknown110;
    u8 _111[7];
    const FastenerVertex* mStartVertex;
    const FastenerVertex* mCurrentVertex;
    sead::Quatf mResetQuat;
};

static_assert(sizeof(FastenerKnob) == 0x138);
