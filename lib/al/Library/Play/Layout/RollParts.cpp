#include "Library/Play/Layout/RollParts.h"

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;

NERVE_IMPL(RollParts, Deactive)
NERVE_IMPL(RollParts, Active)
NERVE_IMPL(RollParts, RollIn)
NERVE_IMPL(RollParts, RollOut)

NERVES_MAKE_STRUCT(RollParts, Deactive, Active, RollIn, RollOut)
}  // namespace

namespace al {
RollParts::RollParts(LayoutActor* parent, const LayoutInitInfo& info, const char* archiveName)
    : LayoutActor("ロールパーツ") {
    initLayoutPartsActor(this, parent, info, archiveName, nullptr);
    initNerve(&NrvRollParts.Deactive, 0);
}

void RollParts::startLoopAction(const char* actionName, const char* paneName) {
    startAction(this, actionName, paneName);
}

void RollParts::setData(const char16** messages, s32 messageCount, bool isLoop, s32 selectedIdx,
                        const char* paneName) {
    mIsLoop = isLoop;
    mMessages = messages;
    mMessageCount = messageCount;
    mPaneName = paneName;
    mCurrentActionType = ActionType::Active;
    mNextActionType = ActionType::Active;

    if (selectedIdx >= 0)
        mSelectedIdx = selectedIdx;

    setPaneString(this, mPaneName, mMessages[mSelectedIdx], 0);
    updateHeaderText();

    if (!mHasStatePane)
        return;

    if (messageCount == 1) {
        if (!isExistAction(this, "Off", "State"))
            return;

        startAction(this, "Off", "State");

        return;
    }

    if (!isExistAction(this, "On", "State"))
        return;

    startAction(this, "On", "State");
}

void RollParts::updateHeaderText() {
    if (!mHeaderPaneName)
        return;

    WStringTmp<0x20> text;
    text.clear();

    if (mMessageCount > 1) {
        for (s32 i = 0; i < mMessageCount; i++)
            if (i == mSelectedIdx)
                text.append(u"1");
            else
                text.append(u"0");
    }

    setPaneString(this, mHeaderPaneName, text.cstr(), 0);
}

void RollParts::setSelectedIdx(s32 idx) {
    mSelectedIdx = idx;
    setPaneString(this, mPaneName, mMessages[mSelectedIdx], 0);
}

void RollParts::activate() {
    if (isNerve(this, &NrvRollParts.Active))
        return;

    if (mMessages)
        setPaneString(this, mPaneName, mMessages[mSelectedIdx], 0);

    setNerve(this, &NrvRollParts.Active);
}

void RollParts::activate(s32 selectedIdx) {
    if (isNerve(this, &NrvRollParts.Active))
        return;

    mSelectedIdx = selectedIdx;
    activate();
}

void RollParts::deactivate() {
    if (isNerve(this, &NrvRollParts.Active)) {
        setNerve(this, &NrvRollParts.Deactive);

        return;
    }

    if (isRoll())
        mNextActionType = ActionType::Deactive;
}

void RollParts::rollRight() {
    if ((!mIsLoop && mSelectedIdx + 1 >= mMessageCount) || mMessageCount <= 1)
        return;

    if (isNerve(this, &NrvRollParts.Active)) {
        mCurrentActionType = ActionType::RollRight;
        setNerve(this, &NrvRollParts.RollOut);

        return;
    }

    if (isNerve(this, &NrvRollParts.RollOut) || isNerve(this, &NrvRollParts.RollIn))
        mNextActionType = ActionType::RollRight;
}

void RollParts::rollLeft() {
    if ((!mIsLoop && mSelectedIdx <= 0) || mMessageCount <= 1)
        return;

    if (isNerve(this, &NrvRollParts.Active)) {
        mCurrentActionType = ActionType::RollLeft;
        setNerve(this, &NrvRollParts.RollOut);

        return;
    }

    if (isNerve(this, &NrvRollParts.RollOut) || isNerve(this, &NrvRollParts.RollIn))
        mNextActionType = ActionType::RollLeft;
}

void RollParts::calcCursorTrans(sead::Vector2f* outCursorTrans) const {
    calcPaneTrans(outCursorTrans, this, "Cursor");
}

bool RollParts::isJustChangeRoll() const {
    return isNerve(this, &NrvRollParts.RollIn) && getNerveStep(this) == 1;
}

bool RollParts::isRoll() const {
    return isNerve(this, &NrvRollParts.RollIn) || isNerve(this, &NrvRollParts.RollOut);
}

void RollParts::exeDeactive() {
    if (isFirstStep(this) && mDeactiveAction)
        startAction(this, mDeactiveAction, nullptr);
}

void RollParts::exeActive() {
    if (isFirstStep(this) && mActiveAction)
        startAction(this, mActiveAction, nullptr);
}

void RollParts::exeRollOut() {
    if (isFirstStep(this)) {
        startAction(this,
                    mCurrentActionType == ActionType::RollLeft ? mRollLeftOutAction :
                                                                 mRollRightOutAction,
                    mRollPaneName);

        if (mCurrentActionType == ActionType::RollLeft) {
            startHitReaction(this, "左ロール", nullptr);
            mSelectedIdx--;

            if (mSelectedIdx < 0)
                mSelectedIdx = mIsLoop ? mMessageCount - 1 : 0;
        } else {
            startHitReaction(this, "右ロール", nullptr);
            mSelectedIdx++;

            if (mSelectedIdx >= mMessageCount)
                mSelectedIdx = mIsLoop ? 0 : mMessageCount - 1;
        }

        updateHeaderText();
    }

    if (isActionEnd(this, mRollPaneName))
        setNerve(this, &NrvRollParts.RollIn);
}

void RollParts::exeRollIn() {
    if (isFirstStep(this)) {
        startAction(this,
                    mCurrentActionType == ActionType::RollLeft ? mRollLeftInAction :
                                                                 mRollRightInAction,
                    mRollPaneName);
        setPaneString(this, mPaneName, mMessages[mSelectedIdx], 0);
    }

    if (isActionEnd(this, mRollPaneName)) {
        mCurrentActionType = mNextActionType;
        mNextActionType = ActionType::Active;

        switch (mCurrentActionType) {
        case ActionType::RollRight:
        case ActionType::RollLeft:
            setNerve(this, &NrvRollParts.RollOut);
            break;
        case ActionType::Deactive:
            setNerve(this, &NrvRollParts.Deactive);
            break;
        default:
            setNerve(this, &NrvRollParts.Active);
            break;
        }
    }
}
}  // namespace al
