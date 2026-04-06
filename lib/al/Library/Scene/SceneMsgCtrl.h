#pragma once

#include <prim/seadSafeString.h>

namespace al {
class SceneMsg {
public:
    SceneMsg();

    void clear();
    void setMsg(const char* label, const char* text);

private:
    bool mIsValid = false;
    sead::FixedSafeString<128> mLabel;
    sead::FixedSafeString<128> mText;
};

static_assert(sizeof(SceneMsg) == 0x138);

class SceneMsgCtrl {
public:
    SceneMsgCtrl();

private:
    SceneMsg mMessages[32];
};

static_assert(sizeof(SceneMsgCtrl) == 0x2700);
}  // namespace al
