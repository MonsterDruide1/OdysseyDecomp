#pragma once

#include <container/seadPtrArray.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LayoutActor;
class LayoutInitInfo;
class LiveActor;
class KeyRepeatCtrl;
}  // namespace al

class MenuSelectParts : public al::NerveExecutor {
public:
    MenuSelectParts(const char*, al::LayoutActor*, al::LiveActor*, const al::LayoutInitInfo&, s32);

    void update();
    void appear(s32);
    void startActionPartsIllustSelectIndex();
    void appearWait();
    void setSelectMessage(s32, const char16*);

    bool isDecideContinue() const;
    bool isDecideEnd() const;
    bool isSelectContinue() const;
    bool isDecideSetting() const;
    bool isSelectSetting() const;
    bool isDecideSave() const;
    bool isSelectSave() const;
    bool isDecideSeparatePlay() const;
    bool isSelectSeparatePlay() const;
    bool isDecideHelp() const;
    bool isSelectHelp() const;
    bool isDecideNewGame() const;
    bool isSelectNewGame() const;
    s32 calcPartsIndex(s32) const;

    void exeHide();
    void exeAppear();
    void startActionMarioSelectIndex();
    void exeSelect();
    void startActionMario(al::LiveActor*, const char*);
    void exeDecideParts();
    bool isInvalidSelect() const;
    void exeDecideInterval();
    void exeDecideEnd();

private:
    al::LayoutActor* mLayoutActor;
    s32 mMenuItemAmount = 0;
    s32 mCursorItemIndex = 0;
    s32 mMax;
    s32 mCount;
    al::LayoutActor** mLayoutArray = nullptr;
    al::LayoutActor* mCursorActor = nullptr;
    s32 field_38 = false;
    al::LiveActor* mMarioActor = nullptr;
    al::LiveActor* mCapActor = nullptr;
    al::KeyRepeatCtrl* mKeyRepeatCtrl = nullptr;
    bool field_58;
};
