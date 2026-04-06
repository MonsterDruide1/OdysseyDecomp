#include "Library/Scene/SceneMsg.h"

namespace al {

void SceneMsg::clear() {
    mIsValid = false;
}

void SceneMsg::setMsg(const char* label, const char* text) {
    mIsValid = true;

    if (mLabel.getStringTop() != label)
        mLabel.copy(label);

    if (text) {
        if (mText.getStringTop() != text)
            mText.copy(text);
    } else {
        mText.clear();
    }
}

}  // namespace al
