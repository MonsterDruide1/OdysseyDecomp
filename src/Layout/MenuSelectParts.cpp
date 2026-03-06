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
#include "Library/LiveActor/LiveActorFunction.h"
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

const MenuSelectParts::Selection sMainMenuParts[5] = {
    MenuSelectParts::Selection::Continue, MenuSelectParts::Selection::SeparatePlay,
    MenuSelectParts::Selection::NewGame, MenuSelectParts::Selection::Help,
    MenuSelectParts::Selection::Setting};

const MenuSelectParts::Selection sPauseMenuParts[5] = {
    MenuSelectParts::Selection::Continue, MenuSelectParts::Selection::SeparatePlay,
    MenuSelectParts::Selection::Help, MenuSelectParts::Selection::Save,
    MenuSelectParts::Selection::Setting};

inline const MenuSelectParts::Selection* getPartsArray(bool isPauseMenu) {
    return isPauseMenu ? sPauseMenuParts : sMainMenuParts;
}

inline void setCursorPaneTrans(al::LayoutActor* cursorActor, al::LayoutActor* actor) {
    sead::Vector2f cursorTrans = {0.0f, 0.0f};
    al::calcPaneTrans(&cursorTrans, actor, "Cursor");
    al::setLocalTrans(cursorActor, cursorTrans);
}

MenuSelectParts::MenuSelectParts(const char* name, al::LayoutActor* layoutActor,
                                 al::LiveActor* marioActor, const al::LayoutInitInfo& info,
                                 s32 menuItemCount)
    : al::NerveExecutor(name), mLayoutActor(layoutActor), mMax(menuItemCount),
      mMarioActor(marioActor) {
    mCapActor = al::getSubActor(al::getSubActor(marioActor, "頭"), "メニュー用キャップ目");

    mKeyRepeatCtrl = new al::KeyRepeatCtrl();
    mKeyRepeatCtrl->init(30, 5);

    mLayoutArray = new al::LayoutActor*[mMax];
    for (s32 i = 0; i < mMax; i++) {
        mLayoutArray[i] = new al::LayoutActor("選択肢パーツ");

        al::StringTmp<32> partListName("%s%02d", "ParList", i);

        al::initLayoutPartsActor(mLayoutArray[i], layoutActor, info, partListName.cstr());
        al::startAction(mLayoutArray[i], "Active", "State");
    }

    mCursorActor = new al::LayoutActor("カーソルパーツ");
    al::initLayoutPartsActor(mCursorActor, layoutActor, info, "ParCursor");

    al::startAction(mCursorActor, "Hide");
    initNerve(&Hide);
}

void MenuSelectParts::update() {
    updateNerve();
}

void MenuSelectParts::appear(s32 menuItemAmount) {
    mIsMainMenu = false;
    mCursorItemIndex = 0;
    mDefaultIndex = 0;
    mMenuItemAmount = menuItemAmount;

    for (s32 i = 0; i < mMenuItemAmount; i++) {
        if (i == mCursorItemIndex)
            al::startFreezeActionEnd(mLayoutArray[calcPartsIndex(i)], "Select");
        else
            al::startFreezeActionEnd(mLayoutArray[calcPartsIndex(i)], "Wait");
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
    al::startFreezeActionEnd(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Select");
    al::setNerve(this, &NrvMenuSelectParts.Select);
}

void MenuSelectParts::setSelectMessage(s32 index, const char16* message) {
    al::setPaneString(mLayoutArray[calcPartsIndex(index)], "TxtContent", message);
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
    return calcPartsIndex(mCursorItemIndex) == Selection::Continue;
}

bool MenuSelectParts::isDecideSetting() const {
    if (!isDecideEnd())
        return false;
    return isSelectSetting();
}

bool MenuSelectParts::isSelectSetting() const {
    return calcPartsIndex(mCursorItemIndex) == Selection::Setting;
}

bool MenuSelectParts::isDecideSave() const {
    if (!isDecideEnd())
        return false;
    return isSelectSave();
}

bool MenuSelectParts::isSelectSave() const {
    return calcPartsIndex(mCursorItemIndex) == Selection::Save;
}

bool MenuSelectParts::isDecideSeparatePlay() const {
    if (!isDecideEnd())
        return false;
    return isSelectSeparatePlay();
}

bool MenuSelectParts::isSelectSeparatePlay() const {
    return calcPartsIndex(mCursorItemIndex) == Selection::SeparatePlay;
}

bool MenuSelectParts::isDecideHelp() const {
    if (!isDecideEnd())
        return false;
    return isSelectHelp();
}

bool MenuSelectParts::isSelectHelp() const {
    return calcPartsIndex(mCursorItemIndex) == Selection::Help;
}

bool MenuSelectParts::isDecideNewGame() const {
    if (!isDecideEnd())
        return false;
    return isSelectNewGame();
}

bool MenuSelectParts::isSelectNewGame() const {
    return calcPartsIndex(mCursorItemIndex) == Selection::NewGame;
}

s32 MenuSelectParts::calcPartsIndex(s32 selection) const {
    bool isPauseMenu = !mIsMainMenu;
    return getPartsArray(isPauseMenu)[selection];
}

void MenuSelectParts::exeHide() {}

void MenuSelectParts::exeAppear() {
    if (al::isFirstStep(this)) {
        setCursorPaneTrans(mCursorActor, mLayoutArray[calcPartsIndex(mCursorItemIndex)]);
        al::startAction(mCursorActor, "Appear");
        startActionMarioSelectIndex();
        for (s32 i = 0; i < mMenuItemAmount; i++) {
            if (i == mCursorItemIndex)
                continue;
            al::startAction(mLayoutArray[calcPartsIndex(i)], "Wait");
        }
    }
    setCursorPaneTrans(mCursorActor, mLayoutArray[calcPartsIndex(mCursorItemIndex)]);

    if (al::isActionEnd(mCursorActor))
        al::setNerve(this, &NrvMenuSelectParts.Select);
}

void MenuSelectParts::startActionMarioSelectIndex() {
    switch (calcPartsIndex(mCursorItemIndex)) {
    case Selection::Continue:
        startActionMario(mMarioActor, "PauseMenuContinue");
        break;
    case Selection::SeparatePlay:
        startActionMario(mMarioActor, "PauseMenu2Player");
        break;
    case Selection::NewGame:
        startActionMario(mMarioActor, "PauseMenuNewGame");
        break;
    case Selection::Help:
        startActionMario(mMarioActor, "PauseMenuHelp");
        break;
    case Selection::Save:
        startActionMario(mMarioActor, "PauseMenuSave");
        break;
    case Selection::Setting:
        startActionMario(mMarioActor, "PauseMenuData");
        break;
    }
}

void MenuSelectParts::exeSelect() {
    if (al::isFirstStep(this)) {
        if (al::isNerve(this, &NrvMenuSelectParts.Select))
            al::startAction(mCursorActor, "Wait");
        mKeyRepeatCtrl->reset();
    }

    mKeyRepeatCtrl->update(rs::isHoldUiUp(mLayoutActor), rs::isHoldUiDown(mLayoutActor));

    if (mKeyRepeatCtrl->isUp() || mKeyRepeatCtrl->isDown()) {
        s32 direction = mKeyRepeatCtrl->isUp() ? -1 : 1;

        al::startAction(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Wait");
        mCursorItemIndex =
            al::modi(mCursorItemIndex + direction + mMenuItemAmount, mMenuItemAmount);

        f32 pitch = ((1.0f - (f32)mCursorItemIndex / (mMenuItemAmount - 1)) * 0.375f) + 1.0f;
        al::PadRumbleParam param;
        alPadRumbleFunction::makePadRumbleParamNearFarVolumePitch(&param, 0.0f, 500.0f,
                                                                  pitch * 0.05f, pitch);

        param.isUseController = 1;
        alPadRumbleFunction::startPadRumbleNo3DWithParam(
            alPadRumbleFunction::getPadRumbleDirector(mLayoutActor), "240Hz単発", param, -1);

        al::startAction(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Select");
        startActionMarioSelectIndex();
    }

    if (rs::isTriggerUiCancel(mLayoutActor) || rs::isTriggerUiPause(mLayoutActor)) {
        if (mIsMainMenu)
            return;
        al::startAction(mLayoutArray[sPauseMenuParts[mCursorItemIndex]], "Wait");
        startActionMario(mMarioActor, "PauseMenuContinue");
        if (mCursorItemIndex != mDefaultIndex)
            mCursorItemIndex = mDefaultIndex;
        al::startAction(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Select");
        setCursorPaneTrans(mCursorActor, mLayoutArray[calcPartsIndex(mCursorItemIndex)]);
        al::startHitReaction(mLayoutActor, "キャンセル");
        al::setNerve(this, &NrvMenuSelectParts.DecideParts);
    } else {
        setCursorPaneTrans(mCursorActor, mLayoutArray[calcPartsIndex(mCursorItemIndex)]);
        if (rs::isTriggerUiDecide(mLayoutActor)) {
            al::startHitReaction(mLayoutActor, "決定");
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
        al::startAction(mLayoutArray[calcPartsIndex(mCursorItemIndex)], "Decide");

        if (calcPartsIndex(mCursorItemIndex) == Selection::Continue)
            startActionMario(mMarioActor, "PauseMenuContinueEnd");
        if (isInvalidSelect()) {
            al::setNerve(this, &NrvMenuSelectParts.SelectSecond);
            return;
        }
        al::startAction(mCursorActor, "End");
    }

    if (al::isActionEnd(mLayoutArray[calcPartsIndex(mCursorItemIndex)]) &&
        al::isActionEnd(mCursorActor)) {
        al::startAction(mCursorActor, "Hide");
        al::setNerve(this, &NrvMenuSelectParts.DecideInterval);
    }
}

bool MenuSelectParts::isInvalidSelect() const {
    if (rs::isSceneStatusInvalidSave(mLayoutActor) &&
        (calcPartsIndex(mCursorItemIndex) == Selection::Save ||
         calcPartsIndex(mCursorItemIndex) == Selection::Setting))
        return true;
    return false;
}

void MenuSelectParts::exeDecideInterval() {
    al::setNerveAtGreaterEqualStep(this, &DecideEnd, 0);
}

void MenuSelectParts::exeDecideEnd() {}
