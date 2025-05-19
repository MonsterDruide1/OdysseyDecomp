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

void RollParts::setData(const char16** messages, s32 param_2, bool param_3, s32 selectedIdx,
                        const char* paneName) {
    _140 = param_3;
    mMessages = messages;
    _138 = param_2;
    mPaneName = paneName;
    _190 = 3;
    _194 = 3;

    if (selectedIdx >= 0)
        mSelectedIdx = selectedIdx;

    setPaneString(this, mPaneName, mMessages[mSelectedIdx], 0);
    updateHeaderText();

    if (!_198)
        return;

    if (param_2 == 1) {
        if (!isExistAction(this, "Off", "State"))
            return;

        startAction(this, "Off", "State");

        return;
    }

    if (!isExistAction(this, "On", "State"))
        return;

    startAction(this, "On", "State");
}

// NON_MATCHING
void RollParts::updateHeaderText() {
    if (!_150)
        return;

    WStringTmp<0x20> a;

    for (s32 i = 0; i < _138; i++)
        if (i == mSelectedIdx)
            a.append(u"1");
        else
            a.append(u"0");

    setPaneString(this, _150, a.cstr(), 0);
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
        _194 = 2;
}

void RollParts::rollRight() {
    if ((!_140 && mSelectedIdx + 1 >= _138) || _138 <= 1)
        return;

    if (isNerve(this, &NrvRollParts.Active)) {
        _190 = 0;
        setNerve(this, &NrvRollParts.RollOut);

        return;
    }

    if (isNerve(this, &NrvRollParts.RollOut) || isNerve(this, &NrvRollParts.RollIn))
        _194 = 0;
}

void RollParts::rollLeft() {
    if ((!_140 && mSelectedIdx <= 0) || _138 <= 1)
        return;

    if (isNerve(this, &NrvRollParts.Active)) {
        _190 = 1;
        setNerve(this, &NrvRollParts.RollOut);

        return;
    }

    if (isNerve(this, &NrvRollParts.RollOut) || isNerve(this, &NrvRollParts.RollIn))
        _194 = 1;
}

void RollParts::calcCursorTrans(sead::Vector2f* outCursorTrans) const {
    calcPaneTrans(outCursorTrans, this, "Cursor");
}

bool RollParts::isJustChangeRoll() const {
    if (!isNerve(this, &NrvRollParts.RollIn))
        return false;

    return getNerveStep(this) == 1;
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
        startAction(this, _190 == 1 ? _188 : _178, _158);

        if (_190 == 1) {
            startHitReaction(this, "左ロール", nullptr);
            mSelectedIdx--;

            if (mSelectedIdx < 0)
                mSelectedIdx = _140 ? _138 - 1 : 0;
        } else {
            startHitReaction(this, "右ロール", nullptr);
            mSelectedIdx++;

            if (mSelectedIdx >= _138)
                mSelectedIdx = _140 ? 0 : _138 - 1;
        }

        updateHeaderText();
    }

    if (isActionEnd(this, _158))
        setNerve(this, &NrvRollParts.RollIn);
}

void RollParts::exeRollIn() {
    if (isFirstStep(this)) {
        startAction(this, _190 == 1 ? _180 : _170, _158);
        setPaneString(this, mPaneName, mMessages[mSelectedIdx], 0);
    }

    if (isActionEnd(this, _158)) {
        _190 = _194;
        _194 = 3;

        switch (_190) {
        case 0:
        case 1:
            setNerve(this, &NrvRollParts.RollOut);
            break;
        case 2:
            setNerve(this, &NrvRollParts.Deactive);
            break;
        default:
            setNerve(this, &NrvRollParts.Active);
            break;
        }
    }
}
}  // namespace al
