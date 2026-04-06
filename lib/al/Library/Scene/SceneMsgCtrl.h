#pragma once

#include "Library/Scene/SceneMsg.h"

namespace al {
class SceneMsgCtrl {
public:
    SceneMsgCtrl();

private:
    SceneMsg mMessages[32];
};

static_assert(sizeof(SceneMsgCtrl) == 0x2700);
}  // namespace al
