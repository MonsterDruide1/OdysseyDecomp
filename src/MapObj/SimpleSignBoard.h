#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class SimpleSignBoard : public al::LiveActor {
public:
    SimpleSignBoard(const char* name);
    void init(const al::ActorInitInfo& info) override;
};

static_assert(sizeof(SimpleSignBoard) == 0x108);

namespace SimpleSignBoardFunction {
void startSignAimVisAnimFromModelName(al::LiveActor* actor, const al::ActorInitInfo& info);
}  // namespace SimpleSignBoardFunction
