#include "Library/Scene/SceneMsgCtrl.h"

namespace al {

SceneMsg::SceneMsg() {}

void SceneMsg::clear() {
    mIsValid = false;
}

void SceneMsg::setMsg(const char* label, const char* text) {
    mIsValid = true;

    mLabel.copy(label);

    if (text)
        mText.copy(text);
    else
        mText.clear();
}

SceneMsgCtrl::SceneMsgCtrl() {}

}  // namespace al
