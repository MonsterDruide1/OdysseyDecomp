#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SimpleAudioUser;
struct ActorInitInfo;
}  // namespace al

class CageShine : public al::LiveActor {
public:
    CageShine(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void addDemoActor();
    void startBreak();
    void exeWait();
    void exeBreak();

private:
    al::LiveActor* mBreakModel = nullptr;
    al::SimpleAudioUser* mSuccessSeObj = nullptr;
};

static_assert(sizeof(CageShine) == 0x118);
