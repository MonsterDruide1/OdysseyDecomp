#pragma once

#include "Library/Scene/SceneMsg.h"

namespace al {
class SceneMsgCtrl {
public:
    SceneMsgCtrl();

private:
    SceneMsg mMessages[32];
};
}  // namespace al
