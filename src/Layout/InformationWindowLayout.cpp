#include "Layout/InformationWindowLayout.h"

#include "Library/Layout/IUseLayout.h"
#include "Library/Layout/IUseLayoutAction.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerUtil.h"

namespace {
inline bool isValidLayout(const InformationWindowLayout* layout) {
    return layout;
}

NERVE_IMPL(InformationWindowLayout, Wait);
NERVE_IMPL(InformationWindowLayout, Appear);
NERVE_IMPL(InformationWindowLayout, End);

NERVES_MAKE_NOSTRUCT(InformationWindowLayout, Wait, Appear, End);
}  // namespace

InformationWindowLayout::InformationWindowLayout(const al::LayoutInitInfo& info)
    : al::LayoutActor("") {
    al::initLayoutActor(this, info, "PlayGuide");
    initNerve(&Wait);
}

void InformationWindowLayout::changeSeparatePlay() {
    mPlayMode = 1;
}

bool InformationWindowLayout::isSeparateTutorial() const {
    return mPlayMode == 1;
}

bool InformationWindowLayout::isSingleTutorial() const {
    return mPlayMode == 0;
}

void InformationWindowLayout::changeSinglePlay() {
    mPlayMode = 0;
}

void InformationWindowLayout::setSeparatePlayerOnlyLayout() {
    al::IUseLayoutAction* layoutAction = isValidLayout(this) ? this : nullptr;
    al::startAction(layoutAction, "1P", "State");
}

void InformationWindowLayout::setSinglePlayLayout() {
    al::IUseLayoutAction* layoutAction = isValidLayout(this) ? this : nullptr;
    al::startAction(layoutAction, "Normal", "State");
}

void InformationWindowLayout::setHackTutorial(const al::LiveActor* actor, const char* label) {
    const al::IUseMessageSystem* messageSystem = this;
    const char16* message =
        rs::getPlayerHackSystemMessageString(actor, messageSystem, "Tutorial", label);
    s32 playMode = mPlayMode;
    al::IUseLayout* layout = isValidLayout(this) ? this : nullptr;

    al::setPaneString(layout, "TxtGuide", message);
    al::IUseLayoutAction* layoutAction = isValidLayout(this) ? this : nullptr;
    al::setPaneString(layout, "TxtGuideSh", message);
    al::requestCaptureRecursive(this);

    if (playMode)
        al::startAction(layoutAction, "1P", "State");
    else
        al::startAction(layoutAction, "Normal", "State");
}

void InformationWindowLayout::setTutorial(const char* label) {
    const al::IUseMessageSystem* messageSystem = this;
    const char16* message = al::getSystemMessageString(messageSystem, "Tutorial", label);
    s32 playMode = mPlayMode;
    al::IUseLayout* layout = isValidLayout(this) ? this : nullptr;

    al::setPaneString(layout, "TxtGuide", message);
    al::IUseLayoutAction* layoutAction = isValidLayout(this) ? this : nullptr;
    al::setPaneString(layout, "TxtGuideSh", message);
    al::requestCaptureRecursive(this);

    if (playMode)
        al::startAction(layoutAction, "1P", "State");
    else
        al::startAction(layoutAction, "Normal", "State");
}

void InformationWindowLayout::setSeparatePlayTutorial(const char* leftLabel,
                                                      const char* rightLabel) {
    const al::IUseMessageSystem* messageSystem = this;
    const char* category = "Tutorial";
    const char16* leftMessage = al::getSystemMessageString(messageSystem, category, leftLabel);
    const char16* rightMessage = al::getSystemMessageString(messageSystem, category, rightLabel);
    al::IUseLayout* layout = isValidLayout(this) ? this : nullptr;

    al::setPaneString(layout, "TxtGuideL", leftMessage);
    al::IUseLayoutAction* layoutAction = isValidLayout(this) ? this : nullptr;
    al::setPaneString(layout, "TxtGuideLSh", leftMessage);
    al::setPaneString(layout, "TxtGuideR", rightMessage);
    al::setPaneString(layout, "TxtGuideRSh", rightMessage);
    al::requestCaptureRecursive(this);
    al::startAction(layoutAction, "2P", "State");
}

void InformationWindowLayout::setAreaTutorial(const char16* message) {
    s32 playMode = mPlayMode;
    al::IUseLayout* layout = isValidLayout(this) ? this : nullptr;

    al::setPaneString(layout, "TxtGuide", message);
    al::IUseLayoutAction* layoutAction = isValidLayout(this) ? this : nullptr;
    al::setPaneString(layout, "TxtGuideSh", message);
    al::requestCaptureRecursive(this);

    if (playMode)
        al::startAction(layoutAction, "1P", "State");
    else
        al::startAction(layoutAction, "Normal", "State");
}

void InformationWindowLayout::appear() {
    al::adjustPaneSizeToTextSizeAll(this);
    al::setNerve(this, &Appear);
    al::LayoutActor::appear();
}

void InformationWindowLayout::end() {
    al::setNerve(this, &End);
}

const char* InformationWindowLayout::getTutorialMstxtName() const {
    return "Tutorial";
}

bool InformationWindowLayout::isWait() const {
    return al::isNerve(this, &Wait);
}

void InformationWindowLayout::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void InformationWindowLayout::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void InformationWindowLayout::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End");
    if (al::isActionEnd(this))
        kill();
}
