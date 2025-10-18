#pragma once

#include <prim/seadSafeString.h>

namespace al {
class SceneMsg {
public:
    SceneMsg();
    void clear();
    void setMsg(const char*, const char*);

private:
    void* field_0;
    sead::FixedSafeString<128> field_8;
    sead::FixedSafeString<128> field_a0;
};
}  // namespace al
