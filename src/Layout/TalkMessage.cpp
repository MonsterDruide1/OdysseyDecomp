#include "Layout/TalkMessage.h"

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/TalkMessageVoicePlayer.h"
#include "Library/Se/SeFunction.h"

#include "Layout/CommonSelectParts.h"
#include "Layout/ProjectReplaceTagProcessor.h"
#include "Util/StageInputFunction.h"

namespace {
const char16 sEmptyPaneString[1] = {0};

NERVE_IMPL(TalkMessage, IconPageNextAndLoadNextMessage);
NERVE_IMPL(TalkMessage, IconPageEnd);
NERVE_IMPL(TalkMessage, End);
NERVE_IMPL(TalkMessage, Wait);
NERVE_IMPL(TalkMessage, IconAppearDelay);
NERVE_IMPL(TalkMessage, IconAppear);
NERVE_IMPL(TalkMessage, IconWait);

NERVES_MAKE_NOSTRUCT(TalkMessage, IconWait);

NERVE_IMPL(TalkMessage, IconPageNextAndPlayNextPage);
NERVE_IMPL(TalkMessage, IconWaitTriggered);
NERVE_IMPL(TalkMessage, TextAnim);
NERVE_IMPL(TalkMessage, AppearWithText);
NERVE_IMPL(TalkMessage, Appear);
NERVE_IMPL(TalkMessage, IconPageNext);

NERVES_MAKE_NOSTRUCT(TalkMessage, IconPageNext);
NERVES_MAKE_STRUCT(TalkMessage, IconPageNextAndLoadNextMessage, IconPageEnd, End, Wait,
                   IconAppearDelay, IconAppear, IconPageNextAndPlayNextPage, IconWaitTriggered,
                   TextAnim, AppearWithText, Appear);
}  // namespace

TalkMessage::TalkMessage(const char* name) : al::LayoutActor(name) {}

void TalkMessage::initLayoutTalk(const al::LayoutInitInfo& info, const char* suffix) {
    initLayoutWithArchiveName(info, "TalkMessage", suffix);
}

void TalkMessage::initLayoutWithArchiveName(const al::LayoutInitInfo& info, const char* archiveName,
                                            const char* suffix) {
    al::initLayoutActor(this, info, archiveName, suffix);

    if (al::isExistPane(this, "TxtMessageSh"))
        al::initLayoutTextPaneAnimatorWithShadow(this, "TxtMessage");
    else
        al::initLayoutTextPaneAnimator(this, "TxtMessage");

    initNerve(&NrvTalkMessage.Appear);

    mParHardKey = new al::LayoutActor("");
    al::initLayoutPartsActor(mParHardKey, this, info, "ParHardKey", nullptr);

    if (al::isEqualString(archiveName, "TalkMessage") ||
        al::isEqualString(archiveName, "TalkMessageOver"))
        mSelectParts = new CommonSelectParts("", this, info, 4, false);

    mVoicePlayer = new al::TalkMessageVoicePlayer();
    kill();
}

void TalkMessage::initLayoutImportant(const al::LayoutInitInfo& info, const char* suffix) {
    mIsImportant = true;
    mIsForceIcon = true;
    initLayoutWithArchiveName(info, "ImportantMessage", suffix);
}

void TalkMessage::initLayoutOver(const al::LayoutInitInfo& info, const char* suffix) {
    initLayoutWithArchiveName(info, "TalkMessageOver", suffix);
}

void TalkMessage::initLayoutForEventTalk(const al::LayoutInitInfo& info) {
    mIsEventTalk = true;
    initLayoutWithArchiveName(info, "TalkMessage", nullptr);
}

void TalkMessage::initLayoutForEventImportant(const al::LayoutInitInfo& info) {
    mIsImportant = true;
    mIsEventTalk = true;
    mIsForceIcon = true;
    initLayoutWithArchiveName(info, "ImportantMessage", nullptr);
}

void TalkMessage::startForNpc(const al::LiveActor* npcActor, const char16* message,
                              const char16* name, const al::MessageTagDataHolder* tagData,
                              bool isWaitNextPage) {
    bool waitNextPageArg = isWaitNextPage;
    const al::MessageTagDataHolder* tagDataArg = tagData;
    const char16* nameArg = name;
    const char16* messageArg = message;
    const al::LiveActor* npcActorArg = npcActor;

    reset();

    mNpcActor = npcActorArg;
    mIsLoadedNextPage = false;
    mMessageTagDataHolder = tagDataArg;
    mMessage = messageArg;
    mIsWaitNextPage = waitNextPageArg;

    if (nameArg) {
        al::setPaneString(this, "TxtName", nameArg, 0);
        al::adjustPaneSizeToTextSizeAll(this);
        if (al::isHidePane(this, "WinNameBase"))
            al::showPane(this, "WinNameBase");
    }

    const char16* messageLocal = mMessage;
    const al::MessageTagDataHolder* tagDataLocal = mMessageTagDataHolder;
    CommonSelectParts* selectParts = mSelectParts;
    const al::LiveActor* storedNpcActor = mNpcActor;
    bool isImportant = mIsImportant;
    bool waitNextPage = mIsWaitNextPage;
    const al::IUseAudioKeeper* audioUser = storedNpcActor;

    if (selectParts)
        selectParts->reset();

    if (isAlive()) {
        if (al::isNerve(this, &NrvTalkMessage.IconWaitTriggered)) {
            al::setNerve(this, &NrvTalkMessage.IconPageNextAndLoadNextMessage);
            return;
        }
    } else {
        appear();
        return;
    }

    ProjectReplaceTagProcessor processor(this);
    al::startTextPaneAnimWithAudioUser(this, messageLocal, tagDataLocal, &processor, audioUser);
    al::adjustPaneSizeToTextSizeAll(this);

    if (isImportant) {
        al::flushTextPaneAnim(this);
        if (waitNextPage && !al::isExistNextPage(this))
            al::setNerve(this, &NrvTalkMessage.Wait);
        else if (mIsForceIcon || mIconAppearDelay >= 1)
            al::setNerve(this, &NrvTalkMessage.IconAppearDelay);
        else
            al::setNerve(this, &NrvTalkMessage.IconAppear);
    } else {
        al::setNerve(this, &NrvTalkMessage.TextAnim);
    }
}

void TalkMessage::reset() {
    mNpcActor = nullptr;
    mIsWaitNextPage = false;
    mIconAppearDelay = -1;
    mMessageTagDataHolder = nullptr;
    mMessage = nullptr;

    if (al::isExistPane(this, "TxtName")) {
        al::setPaneString(this, "TxtName", sEmptyPaneString, 0);
        if (!al::isHidePane(this, "WinNameBase"))
            al::hidePane(this, "WinNameBase");
    }

    if (al::isExistPane(this, "Select"))
        al::startAction(this, "SelectHardKey", "Select");
}

void TalkMessage::startForSystem(const char16* message, const al::MessageTagDataHolder* tagData,
                                 bool isWaitNextPage) {
    bool waitNextPageArg = isWaitNextPage;
    const al::MessageTagDataHolder* tagDataArg = tagData;
    const char16* messageArg = message;

    reset();

    CommonSelectParts* selectParts = mSelectParts;
    bool isImportant = mIsImportant;

    mMessageTagDataHolder = tagDataArg;
    mMessage = messageArg;
    mIsWaitNextPage = waitNextPageArg;

    if (selectParts)
        selectParts->reset();

    if (isAlive()) {
        if (al::isNerve(this, &NrvTalkMessage.IconWaitTriggered)) {
            al::setNerve(this, &NrvTalkMessage.IconPageNextAndLoadNextMessage);
            return;
        }
    } else {
        appear();
        return;
    }

    ProjectReplaceTagProcessor processor(this);
    al::startTextPaneAnimWithAudioUser(this, messageArg, tagDataArg, &processor, nullptr);
    al::adjustPaneSizeToTextSizeAll(this);

    if (isImportant) {
        al::flushTextPaneAnim(this);
        if (waitNextPageArg && !al::isExistNextPage(this))
            al::setNerve(this, &NrvTalkMessage.Wait);
        else if (mIsForceIcon || mIconAppearDelay >= 1)
            al::setNerve(this, &NrvTalkMessage.IconAppearDelay);
        else
            al::setNerve(this, &NrvTalkMessage.IconAppear);
    } else {
        al::setNerve(this, &NrvTalkMessage.TextAnim);
    }
}

void TalkMessage::end() {
    if (al::isNerve(this, &NrvTalkMessage.IconWaitTriggered))
        al::setNerve(this, &NrvTalkMessage.IconPageEnd);
    else
        al::setNerve(this, &NrvTalkMessage.End);
}

bool TalkMessage::isIconWait() const {
    return al::isNerve(this, &NrvTalkMessage.IconWaitTriggered);
}

void TalkMessage::kill() {
    al::LayoutActor::kill();
    al::setPaneString(this, "TxtMessage", sEmptyPaneString, 0);
}

bool TalkMessage::isWait() const {
    return al::isNerve(this, &NrvTalkMessage.Wait) ||
           al::isNerve(this, &NrvTalkMessage.IconWaitTriggered);
}

void TalkMessage::startSelectWithChoiceTable(const char16** choiceTable, s32 choiceNum,
                                             s32 selectedChoice) {
    mSelectParts->startSelectWithChoiceTable(choiceTable, choiceNum, selectedChoice);
}

void TalkMessage::startSelectWithChoiceInfo(const al::EventFlowChoiceInfo* choiceInfo) {
    mSelectParts->startSelectWithChoiceInfo(choiceInfo);
}

bool TalkMessage::isSelectDecide() const {
    return mSelectParts->isDecideEnd();
}

s32 TalkMessage::getSelectedChoiceIndex() const {
    return mSelectParts->getSelectedChoiceIndex();
}

void TalkMessage::exeAppear() {
    if (al::isFirstStep(this) && !al::isActionPlaying(this, "Appear"))
        al::startAction(this, "Appear");

    if (!al::isActionEnd(this))
        return;

    const al::MessageTagDataHolder* tagData = mMessageTagDataHolder;
    const char16* message = mMessage;
    bool isImportant = mIsImportant;
    bool waitNextPage = mIsWaitNextPage;
    const al::IUseAudioKeeper* audioUser = mNpcActor;

    const TalkMessage* self = this;
    const al::IUseSceneObjHolder* sceneObjHolder = self;
    ProjectReplaceTagProcessor processor(sceneObjHolder);
    al::startTextPaneAnimWithAudioUser(this, message, tagData, &processor, audioUser);
    al::adjustPaneSizeToTextSizeAll(this);

    if (isImportant) {
        al::flushTextPaneAnim(this);
        if (waitNextPage && !al::isExistNextPage(this))
            al::setNerve(this, &NrvTalkMessage.Wait);
        else if (mIsForceIcon || mIconAppearDelay >= 1)
            al::setNerve(this, &NrvTalkMessage.IconAppearDelay);
        else
            al::setNerve(this, &NrvTalkMessage.IconAppear);
    } else {
        al::setNerve(this, &NrvTalkMessage.TextAnim);
    }
}

void TalkMessage::exeAppearWithText() {
    if (al::isFirstStep(this) && !al::isActionPlaying(this, "Appear"))
        al::startAction(this, "Appear");

    if (!al::isActionEnd(this))
        return;

    if (mIsWaitNextPage && !al::isExistNextPage(this))
        al::setNerve(this, &NrvTalkMessage.Wait);
    else if (mIsForceIcon || mIconAppearDelay >= 1)
        al::setNerve(this, &NrvTalkMessage.IconAppearDelay);
    else
        al::setNerve(this, &NrvTalkMessage.IconAppear);
}

void TalkMessage::exeTextAnim() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        mVoicePlayer->stop();

        if (mNpcActor) {
            al::TalkMessageVoicePlayer* voicePlayer = mVoicePlayer;
            const al::IUseMessageSystem* messageSystem = this;
            const al::IUseAudioKeeper* audioUser = mNpcActor;
            auto* messagePaneAnim = al::getCurrentMessagePaneAnim(this);
            s32 voiceOffset = mIsLoadedNextPage ? 15 : 0;
            voicePlayer->start(messageSystem, audioUser, messagePaneAnim, voiceOffset);
        }

        mIsSkipSePlayed = false;
    }

    if (al::isEndTextPaneAnim(this, false)) {
        if (mIsWaitNextPage && !al::isExistNextPage(this))
            al::setNerve(this, &NrvTalkMessage.Wait);
        else if (mIsForceIcon || mIconAppearDelay >= 1)
            al::setNerve(this, &NrvTalkMessage.IconAppearDelay);
        else
            al::setNerve(this, &NrvTalkMessage.IconAppear);
    } else if (rs::isTriggerUiDecide(this) || rs::isTriggerUiCancel(this)) {
        al::skipTextPaneAnim(this);
        if (!mIsSkipSePlayed) {
            mIsSkipSePlayed = true;
            mVoicePlayer->stop();
            al::startSe(this, "Skip");
        }
    }
}

void TalkMessage::exeIconAppearDelay() {
    s32 step = mIconAppearDelay > 0 ? mIconAppearDelay : 45;
    al::setNerveAtGreaterEqualStep(this, &NrvTalkMessage.IconAppear, step);
}

void TalkMessage::exeIconAppear() {
    if (al::isFirstStep(this))
        al::startAction(mParHardKey, "Appear");

    if (al::isActionEnd(mParHardKey))
        al::setNerve(this, &IconWait);
}

void TalkMessage::exeIconWait() {
    if (al::isFirstStep(this))
        al::startAction(mParHardKey, "Wait");

    if (rs::isTriggerUiDecide(this) || rs::isTriggerUiCancel(this)) {
        if (al::isExistNextPage(this))
            al::setNerve(this, &NrvTalkMessage.IconPageNextAndPlayNextPage);
        else if (mIsEventTalk)
            al::setNerve(this, &NrvTalkMessage.IconWaitTriggered);
        else
            al::setNerve(this, &NrvTalkMessage.IconPageEnd);
    }
}

void TalkMessage::exeIconWaitTriggered() {}

void TalkMessage::exeIconPageNext() {
    if (al::isFirstStep(this))
        al::startAction(mParHardKey, "PageNext");

    if (al::isActionEnd(mParHardKey))
        al::setNerve(this, &NrvTalkMessage.Wait);
}

void TalkMessage::exeIconPageNextAndPlayNextPage() {
    if (al::isFirstStep(this))
        al::startAction(mParHardKey, "PageNext");

    if (!al::isActionEnd(mParHardKey))
        return;

    ProjectReplaceTagProcessor processor(this);
    al::changeNextPage(this, mMessageTagDataHolder, &processor);
    mIsLoadedNextPage = true;

    if (mIsImportant)
        if (mIsWaitNextPage && !al::isExistNextPage(this))
            al::setNerve(this, &NrvTalkMessage.Wait);
        else
            al::setNerve(this, &NrvTalkMessage.IconAppear);
    else
        al::setNerve(this, &NrvTalkMessage.TextAnim);
}

void TalkMessage::exeIconPageNextAndLoadNextMessage() {
    if (al::isFirstStep(this))
        al::startAction(mParHardKey, "PageNext");

    if (!al::isActionEnd(mParHardKey))
        return;

    const al::MessageTagDataHolder* tagData = mMessageTagDataHolder;
    const char16* message = mMessage;
    bool isImportant = mIsImportant;
    bool waitNextPage = mIsWaitNextPage;
    const al::IUseAudioKeeper* audioUser = mNpcActor;

    ProjectReplaceTagProcessor processor(this);
    al::startTextPaneAnimWithAudioUser(this, message, tagData, &processor, audioUser);
    al::adjustPaneSizeToTextSizeAll(this);

    if (isImportant) {
        al::flushTextPaneAnim(this);
        if (waitNextPage && !al::isExistNextPage(this))
            al::setNerve(this, &NrvTalkMessage.Wait);
        else if (mIsForceIcon || mIconAppearDelay >= 1)
            al::setNerve(this, &NrvTalkMessage.IconAppearDelay);
        else
            al::setNerve(this, &NrvTalkMessage.IconAppear);
    } else {
        al::setNerve(this, &NrvTalkMessage.TextAnim);
    }
}

void TalkMessage::exeIconPageEnd() {
    if (al::isFirstStep(this))
        al::startAction(mParHardKey, "PageEnd");

    if (al::isActionEnd(mParHardKey))
        al::setNerve(this, &NrvTalkMessage.End);
}

void TalkMessage::exeWait() {
    if (al::isFirstStep(this) && !al::isActionPlaying(this, "Wait"))
        al::startAction(this, "Wait");
}

void TalkMessage::exeEnd() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "End");

        if (mNpcActor) {
            al::tryStopAllSeFromUser(mNpcActor, 0, "Mouth");
            al::tryStopAllSeFromUser(mNpcActor, 0, "Lip");
            al::tryStopAllSeFromUser(mNpcActor, 0, "LipNo3D");
        }
    }

    if (al::isActionEnd(this))
        kill();
}

void TalkMessage::appear() {
    al::LayoutActor::appear();
    al::startAction(mParHardKey, "Hide");
    al::startAction(this, "Appear");
    calcAnim(false);

    if (!mIsImportant) {
        al::setNerve(this, &NrvTalkMessage.Appear);
        return;
    }

    const al::MessageTagDataHolder* tagData = mMessageTagDataHolder;
    const char16* message = mMessage;
    bool waitNextPage = mIsWaitNextPage;
    const al::IUseAudioKeeper* audioUser = mNpcActor;

    ProjectReplaceTagProcessor processor(this);
    al::startTextPaneAnimWithAudioUser(this, message, tagData, &processor, audioUser);
    al::adjustPaneSizeToTextSizeAll(this);
    al::flushTextPaneAnim(this);

    if (waitNextPage && !al::isExistNextPage(this))
        al::setNerve(this, &NrvTalkMessage.Wait);
    else if (mIsForceIcon || mIconAppearDelay >= 1)
        al::setNerve(this, &NrvTalkMessage.IconAppearDelay);
    else
        al::setNerve(this, &NrvTalkMessage.IconAppear);

    al::setNerve(this, &NrvTalkMessage.AppearWithText);
}

void TalkMessage::control() {
    if (mSelectParts)
        mSelectParts->updateNerve();

    mVoicePlayer->update();
}

void TalkMessage::startIconPageNext() {
    al::setNerve(this, &IconPageNext);
}
