#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActionAnimDataInfo {
    const char* mActionName = nullptr;
    f32 _8 = -1.0;
    bool mIsKeepAnim = false;
    bool mIsActionAnim = false;
};

struct ActionAnimCtrlInfo {
    ActionAnimCtrlInfo(s32 sklSize);

    const char* mActionName;
    u32 mSklDataCount;
    ActionAnimDataInfo** mSklDatas;
    ActionAnimDataInfo mMclData;
    ActionAnimDataInfo mMtpData;
    ActionAnimDataInfo mMtsData;
    ActionAnimDataInfo mVisData;
    s32 mPartNum;
};
}  // namespace al
