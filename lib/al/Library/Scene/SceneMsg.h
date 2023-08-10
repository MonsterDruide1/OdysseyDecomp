#pragma once

#include <prim/seadSafeString.h>

namespace al {
class SceneMsg {
public:
    SceneMsg();
    void clear();
    void setMsg(const char*, const char*);

private:
    void* field_00;
    sead::FixedSafeString<128> field_08;
    sead::FixedSafeString<128> field_a0;
};
}  // namespace al
