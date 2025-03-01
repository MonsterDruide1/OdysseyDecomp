#pragma once

#include <basis/seadTypes.h>

namespace al {
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
    ActionAnimDataInfo** sklDatas;
    ActionAnimDataInfo mclData;
    ActionAnimDataInfo mtpData;
    ActionAnimDataInfo mtsData;
    ActionAnimDataInfo visData;
    s32 partNum;
};
}  // namespace al
