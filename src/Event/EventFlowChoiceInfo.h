#pragma once

#include <basis/seadTypes.h>

namespace al {
class EventFlowChoiceInfo {
public:
    EventFlowChoiceInfo(s32 choiceNum);

    void registerChoiceMessage(const char16* message);

    s32 mSelectedIndex;
    s32 mCancelIndex;
    void* _8;
    void* _10;
    const char16* mNodeMessage;
    bool mIsContinuePreSelectIndex;
};

static_assert(sizeof(EventFlowChoiceInfo) == 0x28);
}  // namespace al
