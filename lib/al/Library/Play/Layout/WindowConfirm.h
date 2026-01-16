#pragma once

#include <container/seadPtrArray.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class WindowConfirm : public LayoutActor {
public:
    enum class SelectionType {
        None = -1,
        HardKey = 0,
        List00 = 1,
        List01 = 2,
        List02 = 3,
    };

    enum class Direction {
        None = 0,
        Up = 1,
        Down = 2,
    };

    struct Selection {
        SelectionType selectionType = SelectionType::None;
        SelectionType prevSelectionType = SelectionType::None;
        SelectionType cancelType = SelectionType::None;
    };

    WindowConfirm(const LayoutInitInfo& info, const char* name, const char* actorName);

    void setTxtMessage(const char16* message);
    void setTxtList(s32 index, const char16* message);
    void setListNum(s32 num);
    void setCancelIdx(s32 index);
    void appear() override;
    void appearWithChoicingCancel();
    bool isNerveEnd();
    bool tryEnd();
    bool isEnableInput();
    bool tryUp();
    bool tryDown();
    bool tryDecide();
    bool tryDecideWithoutEnd();
    bool tryCancel();
    void setCursorToPane();
    bool tryCancelWithoutEnd();

    void exeHide();
    void exeAppear();
    void exeWait();
    void exeDecide();
    void exeDecideAfter();
    void exeEnd();

    s32 getSelectionIdx() { return (s32)mSelection.prevSelectionType; }

    s32 getPrevSelectionIdx() { return (s32)mSelection.prevSelectionType; }

    s32 getCancelIdx() { return (s32)mSelection.cancelType; }

    SelectionType getSelectionType() { return mSelection.selectionType; }

    SelectionType getPrevSelectionType() { return mSelection.prevSelectionType; }

    SelectionType getCancelType() { return mSelection.cancelType; }

    SelectionType updateSelectionIdx(Direction dir) {
        if (dir == Direction::Up)
            mSelection.prevSelectionType = (SelectionType)((s32)mSelection.prevSelectionType - 1);
        else if (dir == Direction::Down)
            mSelection.prevSelectionType = (SelectionType)((s32)mSelection.prevSelectionType + 1);
        return mSelection.prevSelectionType;
    }

private:
    Direction mDirection = Direction::None;
    Selection mSelection;
    bool mIsDecided = false;
    s32 mCooldown = -1;
    sead::PtrArray<LayoutActor> mParListArray;
    LayoutActor* mCursorActor;
    LayoutActor* mButtonActor;
};
}  // namespace al
