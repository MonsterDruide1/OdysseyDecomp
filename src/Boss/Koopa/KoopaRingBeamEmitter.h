#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActorGroup;
struct ActorInitInfo;
}  // namespace al

class KoopaRingBeamEmitter {
public:
    KoopaRingBeamEmitter(const al::ActorInitInfo& initInfo, bool isLineLight);

    void emit(const sead::Vector3f& trans);
    void emitHipDrop(const sead::Vector3f& trans);
    void emitAttackTail(const sead::Vector3f& trans);
    void cancelAll();
    void killAll();
    void killForDemoAll();

private:
    al::LiveActorGroup* mRingBeamGroup = nullptr;
    sead::Vector3f mTrans = sead::Vector3f::zero;
};

static_assert(sizeof(KoopaRingBeamEmitter) == 0x18);
