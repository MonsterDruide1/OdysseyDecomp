#include "Library/Event/SceneEventFlowMsg.h"

#include "Library/Base/String.h"

namespace al {
SceneEventFlowMsg::SceneEventFlowMsg() {}

bool SceneEventFlowMsg::isReceiveCommand(const char* cmd) const {
    if (mCmd.isEmpty()) {
        return false;
    }

    return isEqualString(cmd, mCmd.cstr());
}

void SceneEventFlowMsg::requestCommand(const char* cmd) {
    mCmd = cmd;
}
}  // namespace al
