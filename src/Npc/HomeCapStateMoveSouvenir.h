#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Nerve/NerveStateBase.h"

class HomeCapMovePoint;

namespace al {
class LiveActor;
}  // namespace al

// Placeholder header: HomeCapStateMoveSouvenir.o is not decompiled yet.
// Only the interface used by HomeCap is declared here; the member layout
// is a placeholder sized to match the 0x60-byte allocation in HomeCap::init.
class HomeCapStateMoveSouvenir : public al::ActorStateBase {
public:
    HomeCapStateMoveSouvenir(al::LiveActor* actor,
                             const sead::PtrArray<HomeCapMovePoint>& points);

    bool tryMoveSouvenir();
    void setReturnChair(al::LiveActor* actor);

private:
    u8 _20[0x40];
};

static_assert(sizeof(HomeCapStateMoveSouvenir) == 0x60);
