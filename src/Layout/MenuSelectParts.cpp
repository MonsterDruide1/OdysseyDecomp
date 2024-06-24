#include "Layout/MenuSelectParts.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Sequence/GameSequenceInfo.h"
#include "System/GameDataHolderAccessor.h"

namespace {
NERVE_IMPL(MenuSelectParts, Hide);
NERVE_IMPL(MenuSelectParts, Appear);
NERVE_IMPL(MenuSelectParts, Select);
NERVE_IMPL(MenuSelectParts, DecideEnd);
NERVE_IMPL(MenuSelectParts, DecideParts);
NERVE_IMPL_(MenuSelectParts, SelectSecond, Select);
NERVE_IMPL(MenuSelectParts, DecideInterval);

NERVES_MAKE_NOSTRUCT(MenuSelectParts, Hide, Appear, DecideEnd);
NERVES_MAKE_STRUCT(MenuSelectParts, Select, DecideParts, SelectSecond, DecideInterval);
}  // namespace

const int dword_71018AF55C[5] = {0, 1, 2, 3, 5};
const int dword_71018AF570[8] = {0, 1, 3, 4, 5, 0, 0, 0};

const int* getArray(bool a1, s32 selection) {
    return a1 ? dword_71018AF55C : dword_71018AF570;
}

MenuSelectParts::MenuSelectParts(const char* name, al::LayoutActor* layoutActor,
                                 al::LiveActor* marioActor, const al::LayoutInitInfo& info,
                                 s32 menuItemCount)
    : al::NerveExecutor(name), mLayoutActor(layoutActor) {}

void MenuSelectParts::update() {
    updateNerve();
}

void MenuSelectParts::appear(s32 menuItemAmount) {
    field_58 = false;
    mMenuItemAmount = menuItemAmount;
    mCursorItemIndex = 0;
    field_38 = false;
}

void MenuSelectParts::startActionPartsIllustSelectIndex() {}

void MenuSelectParts::appearWait() {
    al::LayoutActor* actor = mLayoutArray[calcPartsIndex(mCursorItemIndex)];
    al::startFreezeActionEnd(actor, "Select", nullptr);
    al::setNerve(this, &NrvMenuSelectParts.Select);
}

void MenuSelectParts::setSelectMessage(s32 selection, const char16* message) {
    al::LayoutActor* actor = mLayoutArray[calcPartsIndex(mCursorItemIndex)];
    al::setPaneString(actor, "TxtContent", message, 0);
}

bool MenuSelectParts::isDecideContinue() const {
    if (!isDecideEnd())
        return false;
    return isSelectContinue();
}

bool MenuSelectParts::isDecideEnd() const {
    return al::isNerve(this, &DecideEnd);
}

bool MenuSelectParts::isSelectContinue() const {
    return calcPartsIndex(mCursorItemIndex) == 0;
}

bool MenuSelectParts::isDecideSetting() const {
    if (!isDecideEnd())
        return false;
    return isSelectSetting();
}

bool MenuSelectParts::isSelectSetting() const {
    return calcPartsIndex(mCursorItemIndex) == 5;
}

bool MenuSelectParts::isDecideSave() const {
    if (!isDecideEnd())
        return false;
    return isSelectSave();
}

bool MenuSelectParts::isSelectSave() const {
    return calcPartsIndex(mCursorItemIndex) == 4;
}

bool MenuSelectParts::isDecideSeparatePlay() const {
    if (!isDecideEnd())
        return false;
    return isSelectSeparatePlay();
}

bool MenuSelectParts::isSelectSeparatePlay() const {
    return calcPartsIndex(mCursorItemIndex) == 1;
}

bool MenuSelectParts::isDecideHelp() const {
    if (!isDecideEnd())
        return false;
    return isSelectHelp();
}

bool MenuSelectParts::isSelectHelp() const {
    return calcPartsIndex(mCursorItemIndex) == 3;
}

bool MenuSelectParts::isDecideNewGame() const {
    if (!isDecideEnd())
        return false;
    return isSelectNewGame();
}

bool MenuSelectParts::isSelectNewGame() const {
    return calcPartsIndex(mCursorItemIndex) == 2;
}

s32 MenuSelectParts::calcPartsIndex(s32 selection) const {
    bool a = !field_58;
    return getArray(a, selection)[selection];
}

void MenuSelectParts::exeHide() {}

void MenuSelectParts::exeAppear() {
    if (al::isFirstStep(this)) {
        al::LayoutActor* actor = mLayoutArray[calcPartsIndex(mCursorItemIndex)];
        sead::Vector2f cursorTrans;
        al::calcPaneTrans(&cursorTrans, actor, "Cursor");
        al::setLocalTrans(mCursorActor, cursorTrans);
        al::startAction(mCursorActor, "Appear", nullptr);
        startActionMarioSelectIndex();
        if (mMenuItemAmount >= 1) {
            for (s32 i = 0; i < mMenuItemAmount; i++) {
                al::LayoutActor* actor = mLayoutArray[calcPartsIndex(i)];
                al::startAction(actor, "Wait", nullptr);
            }
        }
    }
    al::LayoutActor* actor = mLayoutArray[calcPartsIndex(mCursorItemIndex)];
    sead::Vector2f cursorTrans;
    al::calcPaneTrans(&cursorTrans, actor, "Cursor");
    al::setLocalTrans(mCursorActor, cursorTrans);

    if (al::isActionEnd(mCursorActor, nullptr))
        al::setNerve(this, &NrvMenuSelectParts.Select);
}

void MenuSelectParts::startActionMarioSelectIndex() {
    switch (calcPartsIndex(mCursorItemIndex)) {
    case 0:
        startActionMario(mMarioActor, "PauseMenuContinue");
        break;
    case 1:
        startActionMario(mMarioActor, "PauseMenu2Player");
        break;
    case 2:
        startActionMario(mMarioActor, "PauseMenuNewGame");
        break;
    case 3:
        startActionMario(mMarioActor, "PauseMenuHelp");
        break;
    case 4:
        startActionMario(mMarioActor, "PauseMenuSave");
        break;
    case 5:
        startActionMario(mMarioActor, "PauseMenuData");
        break;
    }
}

void MenuSelectParts::exeSelect() {}

void MenuSelectParts::startActionMario(al::LiveActor* marioActor, const char* actionName) {
    al::tryStartActionIfNotPlaying(marioActor, actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "目"), actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "頭"), actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "顔"), actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "右手"), actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "左手"), actionName);
    al::tryStartActionIfNotPlaying(mCapActor, actionName);
}

void MenuSelectParts::exeDecideParts() {}

bool MenuSelectParts::isInvalidSelect() const {
    GameDataHolderAccessor accessor(this->mLayoutActor);

    if (rs::isSceneStatusInvalidSave(accessor)) {
        if (calcPartsIndex(mCursorItemIndex) == 4 || calcPartsIndex(mCursorItemIndex) == 5)
            return true;
    }

    return false;
}

void MenuSelectParts::exeDecideInterval() {
    al::setNerveAtGreaterEqualStep(this, &DecideEnd, 0);
}

void MenuSelectParts::exeDecideEnd() {}
