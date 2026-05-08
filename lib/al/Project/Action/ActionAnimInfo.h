#pragma once

#include <basis/seadTypes.h>

namespace al {

enum class ActionAnimType : s32 { Skl, Mcl, Mtp, Mts, Vis };

struct ActionAnimDataInfo {
    const char* actionName = nullptr;
    f32 _8 = -1.0;
    bool isKeepAnim = false;
    bool isActionAnim = false;
};

struct ActionAnimCtrlInfo {
    ActionAnimCtrlInfo(s32 sklSize);

    const char* actionName;
    u32 sklDataCount;
    ActionAnimDataInfo* sklDatas;
    ActionAnimDataInfo mclData;
    ActionAnimDataInfo mtpData;
    ActionAnimDataInfo mtsData;
    ActionAnimDataInfo visData;
    ActionAnimType actionAnimType;
};
}  // namespace al
