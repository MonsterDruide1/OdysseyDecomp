#include "Library/Scene/SceneMsgCtrl.h"

namespace al {

SceneMsg::SceneMsg() {}

void SceneMsg::clear() {
    mIsValid = false;
}

void SceneMsg::setMsg(const char* param1, const char* param2) {
    mIsValid = true;

    _8.copy(param1);

    if (param2)
        _a0.copy(param2);
    else
        _a0.clear();
}

SceneMsgCtrl::SceneMsgCtrl() {}

}  // namespace al
