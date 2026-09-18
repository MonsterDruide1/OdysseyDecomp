#pragma once

#include <basis/seadTypes.h>
#include <math/seadVectorFwd.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class EchoEmitter : public al::LiveActor {
public:
    EchoEmitter(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void start(const sead::Vector3f& trans, f32 radius, s32 life);
    void startKeep(const sead::Vector3f& trans, f32 radius, s32 life);
    void exeWait();
    void exeKeep();
    void exeStop();
    s32 getLife() const;

private:
    s32 mLife = 0;
    f32 mExpansionRate = 0.0f;
    f32 mRadius = 10.0f;
    f32 mAlpha = 1.0f;
    f32 mKeepRadius = 0.0f;
    f32 mOffsetStart = -300.0f;
    f32 mOffsetEnd = 600.0f;
    f32 mRadiusStart = 300.0f;
    f32 mRadiusEnd = 300.0f;
};

static_assert(sizeof(EchoEmitter) == 0x130);
