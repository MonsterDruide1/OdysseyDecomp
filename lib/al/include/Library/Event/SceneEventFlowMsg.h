#pragma once

#include <prim/seadSafeString.h>

namespace al {
class SceneEventFlowMsg {
public:
    SceneEventFlowMsg();

    bool isReceiveCommand(const char* cmd) const;
    void requestCommand(const char* cmd);

private:
    sead::FixedSafeString<64> mCmd;
};

static_assert(sizeof(SceneEventFlowMsg) == 0x58);
}  // namespace al
