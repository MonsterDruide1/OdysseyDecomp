#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class ElectricWirePole : public al::LiveActor {
public:
    ElectricWirePole(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool isExistSelfCollisionBeneath(sead::Vector3f* out, const al::LiveActor* actor,
                                     const sead::Vector3f& pos, f32 startOffset, f32 length);
    void initAfterPlacement() override;
    void kill() override;
    void appear() override;

private:
    al::LiveActor* mBaseActor = nullptr;
};

static_assert(sizeof(ElectricWirePole) == 0x110);
