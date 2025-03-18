#pragma once

#include <basis/seadTypes.h>

namespace al {
class SeDirectorInitInfo {
public:
    SeDirectorInitInfo() {}

private:
    s32 field_0 = -1;
    s32 field_4 = -1;
    s32 field_8 = 1;
    s32 field_c;
    const char* field_10 = "注視点";
    void* field_18 = nullptr;
    bool field_20 = true;
    bool field_21 = false;
    s32 field_24;
    s32 field_28 = 0;
    s32 field_2c = 0;
    s32 field_30 = -1;
    s32 field_34;
    void* field_38 = nullptr;
    void* field_40 = nullptr;
    void* field_48 = nullptr;
    void* field_50 = nullptr;
    void* field_58 = nullptr;
    f32 field_60 = -1.0f;
    f32 field_64 = 1.0f;
};
}  // namespace al
