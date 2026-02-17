#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class DotCharacterRare : public al::LiveActor {
public:
    DotCharacterRare(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appearPopUp();

    void exePopUp();
};

static_assert(sizeof(DotCharacterRare) == 0x108);
