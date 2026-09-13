#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class KoopaCapStateSpinThrow : public al::ActorStateBase {
public:
    KoopaCapStateSpinThrow(al::LiveActor* actor);
    ~KoopaCapStateSpinThrow() override;

    void startSpinThrowChase(const sead::Vector3f* targetTrans, f32 degree, bool isStartSpin);
    void exeSpinStart();
    void exeSpin();

private:
    sead::Vector3f mSpinDir = sead::Vector3f::zero;
    f32 mSpinDegree = 0.0f;
    const sead::Vector3f* mTargetTrans = nullptr;
    s32 mSpinStep = 0;
};

static_assert(sizeof(KoopaCapStateSpinThrow) == 0x40);
