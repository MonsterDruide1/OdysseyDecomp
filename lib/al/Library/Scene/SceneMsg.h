#pragma once

#include <prim/seadSafeString.h>

namespace al {
class SceneMsg {
public:
    __attribute__((used)) SceneMsg() = default;

    void clear();
    void setMsg(const char* label, const char* text);

private:
    bool mIsValid = false;
    sead::FixedSafeString<128> mLabel;
    sead::FixedSafeString<128> mText;
};

static_assert(sizeof(SceneMsg) == 0x138);
}  // namespace al
