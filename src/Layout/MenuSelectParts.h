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
    MenuSelectParts(const char* name, al::LayoutActor* layoutActor, al::LiveActor* marioActor,
                    const al::LayoutInitInfo& info, s32 menuItemCount);

    void update();
    void appear(s32 menuItemAmount);
    void startActionPartsIllustSelectIndex();
    void appearWait();
    void setSelectMessage(s32 index, const char16* message);

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
    s32 calcPartsIndex(s32 selection) const;

    void exeHide();
    void exeAppear();
    void startActionMarioSelectIndex();
    void exeSelect();
    void startActionMario(al::LiveActor* marioActor, const char* actionName);
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
    s32 field_38 = 0;
    al::LiveActor* mMarioActor = nullptr;
    al::LiveActor* mCapActor = nullptr;
    al::KeyRepeatCtrl* mKeyRepeatCtrl = nullptr;
    bool mIsMainMenu = false;
};
