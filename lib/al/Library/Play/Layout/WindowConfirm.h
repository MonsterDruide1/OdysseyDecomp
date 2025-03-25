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
        SelectionType selectionType;
        SelectionType prevSelectionType;
    };

    WindowConfirm(const LayoutInitInfo&, const char*, const char*);

    void setTxtMessage(const char16*);
    void setTxtList(s32, const char16*);
    void setListNum(s32);
    void setCancelIdx(s32);
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

    SelectionType getPrevSelectionType() { return mSelection.prevSelectionType; }

private:
    Direction mDirection;
    Selection mSelection;
    s32 mCancelIdx;
    bool mIsDecided;
    s32 mCooldown;
    sead::PtrArray<LayoutActor> mParListArray;
    LayoutActor* mCursorActor;
    LayoutActor* mButtonActor;
};
}  // namespace al
