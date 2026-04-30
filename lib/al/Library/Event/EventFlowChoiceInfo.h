#pragma once

#include <basis/seadTypes.h>

namespace al {
class EventFlowChoiceInfo {
public:
    EventFlowChoiceInfo(s32 choiceNum);

    void registerChoiceMessage(const char16* message);

private:
    s32 mSelectedIndex;
    s32 mCancelIndex;
    s32 mChoiceMessageNum;
    s32 mChoiceMessageCapacity;
    const char16** mChoiceMessages;
    void* _18;
    bool mIsEnableCancel;
};

static_assert(sizeof(EventFlowChoiceInfo) == 0x28);
}  // namespace al
