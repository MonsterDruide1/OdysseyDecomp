#pragma once

#include <container/seadPtrArray.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class WindowConfirm : public LayoutActor {
public:
    enum SelectionType : s32 {
        SelectionType_None = -1,
        SelectionType_HardKey = 0,
        SelectionType_List00 = 1,
        SelectionType_List01 = 2,
        SelectionType_List02 = 3,
    };

    enum Direction : s32 { Direction_None = 0, Direction_Up = 1, Direction_Down = 2 };

    WindowConfirm(const LayoutInitInfo&, const char*, const char*);

    void setTxtMessage(const char16*);
    void setTxtList(s32, const char16*);
    void setListNum(s32);
    void setCancelIdx(s32);
    void appear();
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

    // s32 getField134() { return mPrevSelectionIndex; }

private:
    Direction mDirection = Direction_None;
    SelectionType mSelectionType = SelectionType_None;
    s32 mSelectionIndex = -1;
    s32 mCancelIdx = -1;
    bool mIsDecided = false;
    s32 mCooldown = -1;
    sead::PtrArray<LayoutActor> mParListArray;
    LayoutActor* mCursorActor;
    LayoutActor* mButtonActor;
};
}  // namespace al
