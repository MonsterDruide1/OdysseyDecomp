#pragma once

#include <container/seadPtrArray.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class WindowConfirm : public LayoutActor {
public:
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

    s32 getField134() { return field_134; }

private:
    s32 field_12c = 0;
    s32 field_130 = -1;
    s32 field_134 = -1;
    s32 field_138 = -1;
    bool field_13c = false;
    s32 field_140 = -1;
    sead::PtrArray<LayoutActor> mParListArray;
    LayoutActor* mCursorActor;
    LayoutActor* mButtonActor;
};
}  // namespace al
