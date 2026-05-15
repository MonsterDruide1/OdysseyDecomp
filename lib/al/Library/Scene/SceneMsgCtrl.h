#pragma once

#include <prim/seadSafeString.h>

namespace al {
class SceneMsg {
public:
    SceneMsg();

    void clear();
    // TODO: add parameter names
    void setMsg(const char* param1, const char* param2);

private:
    bool mIsValid = false;
    sead::FixedSafeString<128> _8;
    sead::FixedSafeString<128> _a0;
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
