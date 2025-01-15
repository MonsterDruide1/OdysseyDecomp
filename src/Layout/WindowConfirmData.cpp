#include "Layout/WindowConfirmData.h"

#include <time/seadDateTime.h>

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

#include "Util/StageInputFunction.h"

namespace {
NERVE_IMPL(WindowConfirmData, Disable);
NERVE_IMPL(WindowConfirmData, Appear);
NERVE_IMPL(WindowConfirmData, Vanish);
NERVE_IMPL(WindowConfirmData, Select);
NERVE_IMPL(WindowConfirmData, Wait);

NERVES_MAKE_NOSTRUCT(WindowConfirmData, Disable, Appear, Vanish, Select, Wait);
}  // namespace

WindowConfirmData::WindowConfirmData(const al::LayoutInitInfo& info, const char* layoutName,
                                     const char* name, bool createDataParts)
    : al::NerveExecutor(name) {
    mWindowConfirmLayout = new al::SimpleLayoutAppearWaitEnd("セーブデータ確認ウィンドウ",
                                                             layoutName, info, nullptr, 0);
    mParCursor = new al::LayoutActor("カーソル");
    al::initLayoutPartsActor(mParCursor, mWindowConfirmLayout, info, "ParCursor", nullptr);

    mParOptions[PaneType_Confirm] = new al::LayoutActor("決定");
    al::initLayoutPartsActor(mParOptions[PaneType_Confirm], mWindowConfirmLayout, info, "ParList00",
                             nullptr);

    mParOptions[PaneType_Cancel] = new al::LayoutActor("キャンセル");
    al::initLayoutPartsActor(mParOptions[PaneType_Cancel], mWindowConfirmLayout, info, "ParList01",
                             nullptr);

    if (createDataParts) {
        mParData = new al::LayoutActor("対象データ");
        al::initLayoutPartsActor(mParData, mWindowConfirmLayout, info, "ParData", nullptr);
    }

    initNerve(&Disable, 0);
}

void WindowConfirmData::setConfirmMessage(const char16* message, const char16* confirmMessage,
                                          const char16* cancelMessage) {
    al::setPaneString(mWindowConfirmLayout, "TxtMessage", message, 0);
    al::setPaneString(mParOptions[PaneType_Confirm], "TxtContent", confirmMessage, 0);
    al::setPaneString(mParOptions[PaneType_Cancel], "TxtContent", cancelMessage, 0);
}

void WindowConfirmData::setConfirmData(al::LayoutActor* actor, nn::ui2d::TextureInfo* texture) {
    al::setPaneString(mParData, "TxtNumber", al::getPaneStringBuffer(actor, "TxtNumber"), 0);
    al::setPaneString(mParData, "TxtWorld", al::getPaneStringBuffer(actor, "TxtWorld"), 0);
    al::setPaneString(mParData, "TxtShine", al::getPaneStringBuffer(actor, "TxtShine"), 0);
    al::setPaneString(mParData, "TxtDay", al::getPaneStringBuffer(actor, "TxtDay"), 0);
    al::setPaneString(mParData, "TxtPlay", al::getPaneStringBuffer(actor, "TxtPlay"), 0);
    al::setPaneTexture(mParData, "PicDummy", texture);

    al::startAction(mParData, al::getActionName(actor, "State"), "State");
    al::startAction(mParData, al::getActionName(actor, "OnOff"), "OnOff");
}

void WindowConfirmData::updateConfirmDataDate() {
    sead::DateTime dateTime(0);
    dateTime.setNow();

    al::ReplaceTimeInfo replaceTimeInfo;
    al::createReplaceTimeInfoForDateTime(&replaceTimeInfo, dateTime.getUnixTime());

    al::WStringTmp<128> dateStr(u"---");
    al::replaceMessageTagTimeDirectDateDetail(&dateStr, mWindowConfirmLayout, replaceTimeInfo);
    al::setPaneString(mParData, "TxtDay", dateStr.cstr(), 0);
}

void WindowConfirmData::appear() {
    al::setNerve(this, &Appear);
    mPaneIndex = PaneType_Confirm;
}

void WindowConfirmData::appearWithChoicingCancel() {
    al::setNerve(this, &Appear);
    mPaneIndex = PaneType_Cancel;
}

void WindowConfirmData::end() {
    al::setNerve(this, &Vanish);
}

void WindowConfirmData::kill() {
    mWindowConfirmLayout->kill();
    al::setNerve(this, &Disable);
}

bool WindowConfirmData::isEndSelect() {
    if (!al::isNerve(this, &Select))
        return false;

    if (!al::isActionPlaying(mParCursor, "End", nullptr))
        return false;

    if (!al::isActionEnd(mParCursor, nullptr))
        return false;

    if (!al::isActionPlaying(mParOptions[mPaneIndex], "Decide", nullptr))
        return false;

    if (!al::isActionEnd(mParOptions[mPaneIndex], nullptr))
        return false;

    return true;
}

bool WindowConfirmData::isDecided() {
    return isEndSelect() && mPaneIndex == PaneType_Confirm;
}

bool WindowConfirmData::isCanceled() {
    return !isEndSelect() || mPaneIndex == PaneType_Cancel;
}

bool WindowConfirmData::isDisable() {
    return al::isNerve(this, &Disable);
}

void WindowConfirmData::exeAppear() {
    if (al::isFirstStep(this)) {
        mWindowConfirmLayout->appear();
        al::startAction(mParCursor, "Hide", nullptr);
        changeSelectingIdx(mPaneIndex);
    }
    if (mWindowConfirmLayout->isWait())
        al::setNerve(this, &Wait);
}

void WindowConfirmData::changeSelectingIdx(s32 index) {
    al::startAction(mParOptions[PaneType_Confirm], index == PaneType_Confirm ? "Wait" : "Select",
                    nullptr);
    al::startAction(mParOptions[PaneType_Cancel], index == PaneType_Cancel ? "Wait" : "Select",
                    nullptr);
    mPaneIndex = (PaneType)index;
}

void WindowConfirmData::exeWait() {
    if (al::isFirstStep(this)) {
        updateCursorPos();
        al::startAction(mParCursor, "Appear", nullptr);
    }

    if (al::isActionPlaying(mParCursor, "Appear", nullptr) && al::isActionEnd(mParCursor, nullptr))
        al::startAction(mParCursor, "Wait", nullptr);

    updateCursorPos();

    if (rs::isRepeatUiDown(mWindowConfirmLayout)) {
        if (!mSelectionCooldown)
            changeSelectingIdx(al::modi(mPaneIndex + 3, 2));
        mSelectionCooldown = al::modi((mSelectionCooldown + 11), 10);
        return;
    }

    if (rs::isRepeatUiUp(mWindowConfirmLayout)) {
        if (!mSelectionCooldown)
            changeSelectingIdx(al::modi(mPaneIndex + 1, 2));
        mSelectionCooldown = al::modi((mSelectionCooldown + 11), 10);
        return;
    }

    if (rs::isTriggerUiDecide(mWindowConfirmLayout)) {
        al::startHitReaction(mWindowConfirmLayout, "決定", nullptr);
        al::setNerve(this, &Select);
        return;
    }

    if (rs::isTriggerUiCancel(mWindowConfirmLayout)) {
        al::startHitReaction(mWindowConfirmLayout, "キャンセル", nullptr);

        if (mPaneIndex != PaneType_Cancel) {
            al::startAction(mParOptions[PaneType_Confirm], "Wait", nullptr);
            al::startAction(mParOptions[PaneType_Cancel], "Select", nullptr);
            mPaneIndex = PaneType_Cancel;
            updateCursorPos();
        }

        al::setNerve(this, &Select);
        return;
    }

    mSelectionCooldown = 0;
}

void WindowConfirmData::updateCursorPos() {
    sead::Vector2f paneTrans = sead::Vector2f::zero;
    al::calcPaneTrans(&paneTrans, mParOptions[mPaneIndex], "Cursor");
    al::setLocalTrans(mParCursor, paneTrans);
}

void WindowConfirmData::exeSelect() {
    if (al::isFirstStep(this)) {
        al::startAction(mParCursor, "End", nullptr);
        al::startAction(mParOptions[mPaneIndex], "Decide", nullptr);
    }
}

void WindowConfirmData::exeVanish() {
    if (al::isFirstStep(this))
        mWindowConfirmLayout->end();

    if (al::isDead(mWindowConfirmLayout))
        kill();
}

void WindowConfirmData::exeDisable() {}
