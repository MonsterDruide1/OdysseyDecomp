#include "Layout/MenuSelectParts.h"

#include "Library/Base/StringUtil.h"
#include "Library/Controller/KeyRepeatCtrl.h"
#include "Library/Controller/PadRumbleDirector.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"
#include "Sequence/GameSequenceInfo.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/StageInputFunction.h"

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

const int mainMenuParts[5] = {0, 1, 2, 3, 5};
const int pauseMenuParts[8] = {0, 1, 3, 4, 5, 0, 0, 0};

const int* getArray(bool isPauseMenu, s32 selection) {
    return isPauseMenu ? pauseMenuParts : mainMenuParts;
}

void setCursorPaneTrans(al::LayoutActor* cursorActor, al::LayoutActor* actor) {
    sead::Vector2f cursorTrans = {0.0f, 0.0f};
    al::calcPaneTrans(&cursorTrans, actor, "Cursor");
    al::setLocalTrans(cursorActor, cursorTrans);
}

MenuSelectParts::MenuSelectParts(const char* name, al::LayoutActor* layoutActor,
                                 al::LiveActor* marioActor, const al::LayoutInitInfo& info,
                                 s32 menuItemCount)
    : al::NerveExecutor(name), mLayoutActor(layoutActor), mMax(menuItemCount),
      mMarioActor(marioActor), mIsMainMenu(false) {
    al::LiveActor* subActor = al::getSubActor(marioActor, "頭");
    mCapActor = al::getSubActor(subActor, "メニュー用キャップ目");
    mKeyRepeatCtrl = new al::KeyRepeatCtrl();
    mKeyRepeatCtrl->init(30, 5);
    mLayoutArray = new al::LayoutActor*[mMax];
    for (s32 i = 0; i < mMax; i++) {
        mLayoutArray[i] = new al::LayoutActor("選択肢パーツ");
        al::StringTmp<32> name("%s%02d", "ParList", i);
        al::initLayoutPartsActor(mLayoutArray[i], layoutActor, info, name.cstr(), nullptr);
        al::startAction(mLayoutArray[i], "Active", "State");
    }
    mCursorActor = new al::LayoutActor("カーソルパーツ");
    al::initLayoutPartsActor(mCursorActor, layoutActor, info, "ParCursor", nullptr);

    al::startAction(mCursorActor, "Hide", nullptr);
    initNerve(&Hide, 0);
}

void MenuSelectParts::update() {
    updateNerve();
}

void MenuSelectParts::appear(s32 menuItemAmount) {
    mIsMainMenu = false;
    mCursorItemIndex = 0;
    field_38 = false;

    mMenuItemAmount = menuItemAmount;
    for (s32 i = 0; i < mMenuItemAmount; i++) {
        if (i == mCursorItemIndex)
            al::startFreezeActionEnd(mLayoutArray[calcPartsIndex(i)], "Select", nullptr);
        else
            al::startFreezeActionEnd(mLayoutArray[calcPartsIndex(i)], "Wait", nullptr);
        al::startAction(mLayoutArray[calcPartsIndex(i)], "Active", "State");
    }

    if (rs::isSceneStatusInvalidSave(mLayoutActor)) {
        al::startAction(mLayoutArray[calcPartsIndex(3)], "Deactive", "State");
        al::startAction(mLayoutArray[calcPartsIndex(4)], "Deactive", "State");

        al::setSeKeeperPlayNamePrefix(mLayoutArray[calcPartsIndex(3)], "Deactive");
        al::setSeKeeperPlayNamePrefix(mLayoutArray[calcPartsIndex(4)], "Deactive");
    }

    al::setNerve(this, &Appear);
}

void MenuSelectParts::startActionPartsIllustSelectIndex() {}

void MenuSelectParts::appearWait() {
    al::startFreezeActionEnd(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Select", nullptr);
    al::setNerve(this, &NrvMenuSelectParts.Select);
}

void MenuSelectParts::setSelectMessage(s32 selection, const char16* message) {
    al::setPaneString(mLayoutArray[calcPartsIndex(selection)], "TxtContent", message, 0);
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
    bool a = !mIsMainMenu;
    return getArray(a, selection)[selection];
}

void MenuSelectParts::exeHide() {}

void MenuSelectParts::exeAppear() {
    if (al::isFirstStep(this)) {
        setCursorPaneTrans(mCursorActor, mLayoutArray[calcPartsIndex(mCursorItemIndex)]);
        al::startAction(mCursorActor, "Appear", nullptr);
        startActionMarioSelectIndex();
        for (s32 i = 0; i < mMenuItemAmount; i++) {
            if (i == mCursorItemIndex)
                continue;
            al::startAction(mLayoutArray[calcPartsIndex(i)], "Wait", nullptr);
        }
    }
    setCursorPaneTrans(mCursorActor, mLayoutArray[calcPartsIndex(mCursorItemIndex)]);

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

void MenuSelectParts::exeSelect() {
    if (al::isFirstStep(this)) {
        if (al::isNerve(this, &NrvMenuSelectParts.Select))
            al::startAction(mCursorActor, "Wait", nullptr);
        mKeyRepeatCtrl->reset();
    }
    mKeyRepeatCtrl->update(rs::isHoldUiUp(mLayoutActor), rs::isHoldUiDown(mLayoutActor));
    if (mKeyRepeatCtrl->isUp() || mKeyRepeatCtrl->isDown()) {
        s32 direction = mKeyRepeatCtrl->isUp() ? -1 : 1;
        al::startAction(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Wait", nullptr);
        mCursorItemIndex =
            al::modi(mCursorItemIndex + direction + mMenuItemAmount, mMenuItemAmount);

        f32 v19 = ((1.0f - (f32)mCursorItemIndex / (mMenuItemAmount - 1)) * 0.375f) + 1.0f;
        al::PadRumbleParam param = {0.0f, 3000.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, 0, 0};
        alPadRumbleFunction::makePadRumbleParamNearFarVolumePitch(&param, 0.0f, 500.0f, v19 * 0.05f,
                                                                  v19);
        param.field_1C = 1;
        alPadRumbleFunction::startPadRumbleNo3DWithParam(
            alPadRumbleFunction::getPadRumbleDirector(mLayoutActor), "240Hz", param, -1);
        al::startAction(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Select", nullptr);
        startActionMarioSelectIndex();
    }
    if (rs::isTriggerUiCancel(mLayoutActor) || rs::isTriggerUiPause(mLayoutActor)) {
        if (mIsMainMenu)
            return;
        al::startAction(mLayoutArray[pauseMenuParts[mCursorItemIndex]], "Wait", nullptr);
        startActionMario(mMarioActor, "PauseMenuContinue");
        if (mCursorItemIndex != field_38)
            mCursorItemIndex = field_38;
        al::startAction(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Select", nullptr);
        setCursorPaneTrans(mCursorActor, mLayoutArray[calcPartsIndex(mCursorItemIndex)]);
        al::startHitReaction(mLayoutActor, "キャンセル", nullptr);
        al::setNerve(this, &NrvMenuSelectParts.DecideParts);
    } else {
        setCursorPaneTrans(mCursorActor, mLayoutArray[calcPartsIndex(mCursorItemIndex)]);
        if (rs::isTriggerUiDecide(mLayoutActor)) {
            al::startHitReaction(mLayoutActor, "決定", nullptr);
            al::setNerve(this, &NrvMenuSelectParts.DecideParts);
        }
    }
}

void MenuSelectParts::startActionMario(al::LiveActor* marioActor, const char* actionName) {
    al::tryStartActionIfNotPlaying(marioActor, actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "目"), actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "頭"), actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "顔"), actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "右手"), actionName);
    al::tryStartActionIfNotPlaying(al::getSubActor(marioActor, "左手"), actionName);
    al::tryStartActionIfNotPlaying(mCapActor, actionName);
}

void MenuSelectParts::exeDecideParts() {
    if (al::isFirstStep(this)) {
        al::startAction(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Decide", nullptr);

        if (calcPartsIndex(mCursorItemIndex) == 0)
            startActionMario(mMarioActor, "PauseMenuContinueEnd");
        if (isInvalidSelect()) {
            al::setNerve(this, &NrvMenuSelectParts.SelectSecond);
            return;
        }
        al::startAction(mCursorActor, "End", nullptr);
    }

    if (al::isActionEnd(mLayoutArray[calcPartsIndex(mCursorItemIndex)], nullptr) &&
        al::isActionEnd(mCursorActor, nullptr)) {
        al::startAction(mCursorActor, "Hide", nullptr);
        al::setNerve(this, &NrvMenuSelectParts.DecideInterval);
    }
}

bool MenuSelectParts::isInvalidSelect() const {
    GameDataHolderAccessor accessor(mLayoutActor);

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
