#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class EventFlowChoiceInfo;
class LayoutActor;
class LayoutInitInfo;
}  // namespace al

class CommonSelectParts : public al::NerveExecutor {
public:
    CommonSelectParts(const char* name, al::LayoutActor* layoutActor,
                      const al::LayoutInitInfo& info, s32 maxChoiceNum, bool isUseDeactivate);

    void activateAll();
    void startSelect2(const char16* choiceFirst, const char16* choiceSecond, s32 selectedChoice);
    void startSelectWithChoiceTable(const char16** choiceTable, s32 choiceNum, s32 selectedChoice);
    void startSelectWithChoiceTableWithoutPosAnim(const char16** choiceTable, s32 choiceNum,
                                                  s32 selectedChoice);
    void startSelectWithChoiceInfo(const al::EventFlowChoiceInfo* choiceInfo);
    bool isActive() const;
    bool isDecideEnd() const;
    void setSelectPartsString(const char16* message, s32 index);
    void deactivate(s32 index);
    void kill();
    void reset();

    s32 getSelectedChoiceIndex() const { return mSelectedChoiceIndex; }

    void exeHide();
    void exeAppearBefore();
    void exeAppear();
    void exeAppearAfter();
    void exeAppearCursor();
    void exeSelect();
    void exeDecideParts();
    void exeDecideDeactiveParts();
    void exeDecide();
    void exeDecideAfter();
    void exeDecideEnd();

private:
    al::LayoutActor* mLayoutActor = nullptr;
    s32 mCursorIndex = 0;
    s32 mSelectedChoiceIndex = 0;
    s32 mChoiceNum = 0;
    s32 mMaxChoiceNum = 0;
    al::LayoutActor** mParts = nullptr;
    al::LayoutActor* mCursorActor = nullptr;
    s32 mDecideIndex = -1;
    s32 mDecideDeactiveIndex = 0;
    s32 mLastSelectIndex = -1;
    bool mIsWithoutPosAnim = false;
    bool mIsDecideDeactive = false;
    u8 _46[2] = {};
    s32 mDeactivateNum = 0;
    s32 _4c = 0;
    s32* mDeactivateIndexes = nullptr;
};

static_assert(sizeof(CommonSelectParts) == 0x58);
