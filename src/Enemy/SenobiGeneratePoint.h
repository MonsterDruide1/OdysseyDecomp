#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class Senobi;

namespace al {
struct ActorInitInfo;
}  // namespace al

class SenobiGeneratePoint : public al::LiveActor {
public:
    SenobiGeneratePoint(const char* actorName) : al::LiveActor(actorName) {}

    void init(const al::ActorInitInfo& info) override;

    void forceKill();
    bool tryGenerate();
    void exeStandby();
    void exeWait();
    void exeEnd();
    void exeGenerate();

private:
    Senobi* mSenobi = nullptr;
    sead::Vector3f mGenerateOffset = sead::Vector3f::zero;
};

static_assert(sizeof(SenobiGeneratePoint) == 0x120);
