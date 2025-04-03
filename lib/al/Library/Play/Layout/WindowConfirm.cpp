#include "Library/Play/Layout/WindowConfirm.h"

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;
NERVE_IMPL(WindowConfirm, Hide);
NERVE_IMPL(WindowConfirm, Appear);
NERVE_IMPL(WindowConfirm, End);
NERVE_IMPL(WindowConfirm, Decide);
NERVE_IMPL(WindowConfirm, Wait);
NERVE_IMPL(WindowConfirm, DecideAfter);

NERVES_MAKE_NOSTRUCT(WindowConfirm, Hide, Appear, DecideAfter);
NERVES_MAKE_STRUCT(WindowConfirm, End, Decide, Wait);

}  // namespace

namespace al {

WindowConfirm::WindowConfirm(const LayoutInitInfo& info, const char* name, const char* actorName)
    : LayoutActor(actorName) {
    initLayoutActor(this, info, name, nullptr);

    s32 paneChildNum = getPaneChildNum(this, "All");
    s32 i = 0;

    s32 numberOfPanes = 0;
    for (i = 0; i < paneChildNum; i++)
        numberOfPanes += isExistPane(this, StringTmp<64>("ParList%02d", i).cstr());

    mParListArray.allocBuffer(numberOfPanes, nullptr, 8);

    for (i = 0; i < numberOfPanes; i++) {
        LayoutActor* choiceActor = new LayoutActor(StringTmp<64>("選択肢%02d", i).cstr());
        initLayoutPartsActor(choiceActor, this, info, StringTmp<64>("ParList%02d", i).cstr(),
                             nullptr);

        mParListArray.pushBack(choiceActor);
    }

    if (isExistPane(this, "ParHardKey")) {
        mButtonActor = new LayoutActor("Ａボタン");
        initLayoutPartsActor(mButtonActor, this, info, "ParHardKey", nullptr);
    }

    mCursorActor = new LayoutActor("カーソル");
    initLayoutPartsActor(mCursorActor, this, info, "ParCursor", nullptr);

    initNerve(&Hide, 0);
}

void WindowConfirm::setTxtMessage(const char16* message) {
    setPaneString(this, "TxtMessage", message, 0);
}

void WindowConfirm::setTxtList(s32 index, const char16* message) {
    setPaneString(mParListArray.at(index), "TxtContent", message, 0);
}

void WindowConfirm::setListNum(s32 num) {
    SelectionType selectionType = (SelectionType)num;
    mSelection.selectionType = selectionType;
    if (selectionType == SelectionType::List01)
        setCancelIdx(1);
    if (selectionType == SelectionType::List02)
        setCancelIdx(0);
}

void WindowConfirm::setCancelIdx(s32 index) {
    mCancelIdx = index;
}

void WindowConfirm::appear() {
    if (isAlive())
        return;

    mSelection.index = 0;
    mDirection = Direction::None;

    startAction(this, "Appear", nullptr);
    switch (mSelection.selectionType) {
    case SelectionType::HardKey:
        startAction(this, "SelectHardKey", "Select");

        hidePane(this, "ParCursor");
        hidePane(this, "ParHardKey");
        break;
    case SelectionType::List00:
        startAction(this, "SelectHardKey", "Select");
        startAction(mButtonActor, "Appear", nullptr);

        hidePane(this, "ParCursor");
        showPane(this, "ParHardKey");
        break;
    case SelectionType::List01:
        startAction(this, "Select2", "Select");
        startAction(mCursorActor, "Appear", nullptr);
        startAction(mParListArray[0], "Select", nullptr);
        startAction(mParListArray[1], "Wait", nullptr);

        showPane(this, "ParCursor");
        hidePane(this, "ParHardKey");
        break;
    case SelectionType::List02:
        startAction(this, "Select3", "Select");
        startAction(mCursorActor, "Appear", nullptr);
        startAction(mParListArray[0], "Select", nullptr);
        startAction(mParListArray[1], "Wait", nullptr);
        startAction(mParListArray[2], "Wait", nullptr);

        showPane(this, "ParCursor");
        hidePane(this, "ParHardKey");
        break;
    default:
        break;
    }
    LayoutActor::appear();
    setNerve(this, &Appear);
}

void WindowConfirm::appearWithChoicingCancel() {
    if (isAlive())
        return;

    appear();

    if (mSelection.selectionType == SelectionType::List01) {
        startAction(mParListArray[mSelection.index], "Wait", nullptr);
        startAction(mParListArray[mCancelIdx], "Select", nullptr);
    } else if (mSelection.selectionType == SelectionType::List02) {
        startAction(mParListArray[mSelection.index], "Wait", nullptr);
        startAction(mParListArray[mCancelIdx], "Select", nullptr);
    }

    mSelection.index = mCancelIdx;
}

bool WindowConfirm::isNerveEnd() {
    return isNerve(this, &NrvWindowConfirm.End);
}

bool WindowConfirm::tryEnd() {
    if (isEnableInput()) {
        mIsDecided = false;
        setNerve(this, &NrvWindowConfirm.End);
        return true;
    }
    return false;
}

bool WindowConfirm::isEnableInput() {
    if (mCooldown <= 0 && isNerve(this, &NrvWindowConfirm.Wait) && isGreaterEqualStep(this, 10)) {
        mCooldown = 10;
        return true;
    }
    return false;
}

bool WindowConfirm::tryUp() {
    if (isEnableInput()) {
        mDirection = Direction::Up;
        return true;
    }
    return false;
}

bool WindowConfirm::tryDown() {
    if (isEnableInput()) {
        mDirection = Direction::Down;
        return true;
    }
    return false;
}

bool WindowConfirm::tryDecide() {
    if (isEnableInput()) {
        mIsDecided = false;
        startHitReaction(this, "決定", nullptr);
        setNerve(this, &NrvWindowConfirm.Decide);
        return true;
    }
    return false;
}

bool WindowConfirm::tryDecideWithoutEnd() {
    if (isEnableInput()) {
        mIsDecided = false;
        setNerve(this, &NrvWindowConfirm.Decide);
        return true;
    }
    return false;
}

bool WindowConfirm::tryCancel() {
    if (!isEnableInput())
        return false;
    if ((mSelection.selectionType == SelectionType::List01 ||
         mSelection.selectionType == SelectionType::List02) &&
        mSelection.index != mCancelIdx) {
        startAction(mParListArray[(s32)mSelection.index], "Wait", nullptr);
        startAction(mParListArray[(s32)mCancelIdx], "Select", nullptr);
        mSelection.index = mCancelIdx;
        setCursorToPane();
    }
    startHitReaction(this, "キャンセル", nullptr);
    mIsDecided = false;
    setNerve(this, &NrvWindowConfirm.Decide);
    return true;
}

void WindowConfirm::setCursorToPane() {
    if (mSelection.selectionType < SelectionType::List01)
        return;
    sead::Vector3f trans = {1.0f, 1.0f, 1.0f};
    calcPaneTrans(&trans, mParListArray[mSelection.index], "Cursor");
    setPaneLocalTrans(this, "ParCursor", trans);
}

bool WindowConfirm::tryCancelWithoutEnd() {
    if (isEnableInput()) {
        if ((mSelection.selectionType == SelectionType::List01 ||
             mSelection.selectionType == SelectionType::List02) &&
            mSelection.index != mCancelIdx) {
            startAction(mParListArray[(s32)mSelection.index], "Wait", nullptr);
            startAction(mParListArray[mCancelIdx], "Select", nullptr);
            mSelection.index = mCancelIdx;
            setCursorToPane();
        }
        mIsDecided = true;
        setNerve(this, &NrvWindowConfirm.Decide);
        return true;
    }
    return false;
}

void WindowConfirm::exeHide() {}

void WindowConfirm::exeAppear() {
    if (mSelection.selectionType == SelectionType::List01 ||
        mSelection.selectionType == SelectionType::List02)
        setCursorToPane();
    if (isActionEnd(this, nullptr))
        setNerve(this, &NrvWindowConfirm.Wait);
}

void WindowConfirm::exeWait() {
    if (isFirstStep(this)) {
        startAction(this, "Wait", nullptr);
        mCooldown = -1;
    }
    if (mSelection.selectionType == SelectionType::List01 ||
        mSelection.selectionType == SelectionType::List02) {
        if (isActionPlaying(mCursorActor, "Appear", nullptr) && isActionEnd(mCursorActor, nullptr))
            startAction(mCursorActor, "Wait", nullptr);
    } else {
        if (mSelection.selectionType == SelectionType::List00)
            if (isActionPlaying(mButtonActor, "Appear", nullptr) &&
                isActionEnd(mButtonActor, nullptr))
                startAction(mButtonActor, "Wait", nullptr);
    }

    if (mDirection == Direction::Up) {
        startAction(mParListArray[mSelection.index], "Wait", nullptr);
        if (mSelection.index-- <= 0)
            mSelection.index = (s32)mSelection.selectionType - 1;

        startAction(mParListArray[mSelection.index], "Select", nullptr);
        setCursorToPane();
    }
    if (mDirection == Direction::Down) {
        startAction(mParListArray[mSelection.index], "Wait", nullptr);
        mSelection.index++;
        if (mSelection.index >= (s32)mSelection.selectionType)
            mSelection.index = 0;

        startAction(mParListArray[mSelection.index], "Select", nullptr);
        setCursorToPane();
    }

    mDirection = Direction::None;

    if (mCooldown >= 0)
        mCooldown--;
}

void WindowConfirm::exeDecide() {
    if (isFirstStep(this)) {
        if (((s32)mSelection.selectionType & 0xFFFFFFFE) == 2) {
            startAction(mParListArray[mSelection.index], "Decide", nullptr);
            startAction(mCursorActor, "End", nullptr);
        } else if (((s32)mSelection.selectionType & 0xFFFFFFFE) == 1) {
            startAction(mButtonActor, "PageEnd", nullptr);
        } else if (((s32)mSelection.selectionType & 0xFFFFFFFE) == 0) {
            setNerve(this, &DecideAfter);
        }
    }
    if (((s32)mSelection.selectionType & 0xFFFFFFFE) == 2) {
        if (isActionEnd(mParListArray[mSelection.index], nullptr))
            setNerve(this, &DecideAfter);
    } else if (isActionEnd(mButtonActor, nullptr))
        setNerve(this, &DecideAfter);
}

void WindowConfirm::exeDecideAfter() {
    setNerveAtGreaterEqualStep(this, &NrvWindowConfirm.End, 0);
}

void WindowConfirm::exeEnd() {
    if (!mIsDecided) {
        if (isFirstStep(this))
            startAction(this, "End", nullptr);

        if (isActionEnd(this, nullptr))
            kill();
    }
}

}  // namespace al
