#include "MapObj/TalkMessageInfoDirector.h"

#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Message/LanguageUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Scene/SceneUtil.h"
#include "Library/Se/SeFunction.h"

#include "Layout/ProjectReplaceTagProcessor.h"
#include "MapObj/CapMessageKeeper.h"
#include "MapObj/TalkMessageInfoDirectorStateAppearMessage.h"
#include "MapObj/TalkMessageInfoDirectorStateAppearWaitEndMessage.h"
#include "MapObj/TalkMessageInfoMessageParam.h"
#include "MapObj/TalkMessageInfoMoon.h"
#include "MapObj/TalkMessageInfoParam.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataUtil.h"
#include "Util/StageInputFunction.h"

namespace alSeFunction {
void startSituation(const al::IUseAudioKeeper* user, const char* name, s32 unk);
void endSituation(const al::IUseAudioKeeper* user, const char* name, s32 unk);
}  // namespace alSeFunction

namespace {
NERVE_IMPL(TalkMessageInfoDirector, Watch);
NERVE_IMPL(TalkMessageInfoDirector, AppearMessage);
NERVE_IMPL(TalkMessageInfoDirector, AppearMessageLow);
NERVE_IMPL(TalkMessageInfoDirector, AppearMessageDemo);
NERVE_IMPL(TalkMessageInfoDirector, AppearMessageBoss);
NERVE_IMPL(TalkMessageInfoDirector, AppearMessageMoon);
NERVE_IMPL(TalkMessageInfoDirector, AppearMessageMinigame);
NERVE_IMPL(TalkMessageInfoDirector, AppearWaitEndMessage);
NERVE_IMPL(TalkMessageInfoDirector, EndForce);
NERVE_IMPL(TalkMessageInfoDirector, Appear);
NERVE_IMPL(TalkMessageInfoDirector, FollowText);
NERVE_IMPL(TalkMessageInfoDirector, Wait);
NERVE_IMPL(TalkMessageInfoDirector, Disappear);
NERVE_IMPL(TalkMessageInfoDirector, Delay);
NERVES_MAKE_NOSTRUCT(TalkMessageInfoDirector, Watch, AppearMessage, AppearMessageLow,
                     AppearMessageDemo, AppearMessageBoss, AppearMessageMoon, AppearMessageMinigame,
                     AppearWaitEndMessage, EndForce, Appear, FollowText, Wait, Disappear, Delay);
}  // namespace

TalkMessageInfoDirector::TalkMessageInfoDirector() {
    for (s32 i = 0; i < 32; ++i)
        mInfoParams[i] = nullptr;
}

void TalkMessageInfoDirector::init(const al::ActorInitInfo& info,
                                   al::SceneObjHolder* sceneObjHolder) {
    mSceneObjHolder = sceneObjHolder;
    mNerveKeeper = new al::NerveKeeper(this, &Watch, 7);
    mLayout = new al::SimpleLayoutAppearWaitEnd("帽子メッセージ レイアウト", "TalkMessageInfo",
                                                al::getLayoutInitInfo(info), nullptr, false);
    al::initLayoutTextPaneAnimator(mLayout, "TxtMessage");
    al::setSeSourceVolume(mLayout, 1.0f);
    al::startAction(mLayout, "Loop", "Loop");
    mAudioKeeper = alAudioKeeperFunction::createAudioKeeper(info.audioDirector, nullptr, nullptr);
    mStateAppearMessage = new TalkMessageInfoDirectorStateAppearMessage(this, mLayout);
    mStateAppearWaitEndMessage =
        new TalkMessageInfoDirectorStateAppearWaitEndMessage(this, mLayout);
    al::initNerveState(this, mStateAppearMessage, &AppearMessage, "[state]システムメッセージ出現");
    al::addNerveState(this, mStateAppearMessage, &AppearMessageLow,
                      "[state]システムメッセージ(優先度：低)出現");
    al::addNerveState(this, mStateAppearMessage, &AppearMessageDemo, "[state]メッセージ表示(デモ)");
    al::addNerveState(this, mStateAppearMessage, &AppearMessageBoss,
                      "[state]システムメッセージ(ボス)");
    al::addNerveState(this, mStateAppearMessage, &AppearMessageMoon,
                      "[state]ムーン通知(配置) 出現");
    al::addNerveState(this, mStateAppearMessage, &AppearMessageMinigame,
                      "[state]システムメッセージ(ミニゲーム)");
    al::initNerveState(this, mStateAppearWaitEndMessage, &AppearWaitEndMessage,
                       "[state]メッセージ表示(プログラムで開始・終了)");
    mCapMessageKeeper = new CapMessageKeeper();
}

void TalkMessageInfoDirector::execute() {
    if (al::isNerve(this, &AppearMessageBoss) || al::isNerve(this, &AppearMessageMinigame)) {
        mNerveKeeper->update();
        return;
    }

    if (!mIsPlayCapMessage) {
        if (al::isNerve(this, &AppearMessageDemo) || al::isNerve(this, &EndForce))
            mNerveKeeper->update();
        return;
    }

    s32 bestIdx = findInAreaTalkMessageInfoIdx();
    TalkMessageInfoParam* param = nullptr;
    if (bestIdx >= 0)
        param = mInfoParams[bestIdx];

    if (param != nullptr && param->isShowDisableCap()) {
        if (!GameDataFunction::isEnableCap(GameDataHolderAccessor(this))) {
            mNerveKeeper->update();
            return;
        }

        param->onAppearEnd();
        if (isActiveCapMessage(nullptr) || isDelayCapMessage(nullptr))
            endForce();
        return;
    }

    if (GameDataFunction::isEnableCap(GameDataHolderAccessor(this)))
        mNerveKeeper->update();
}

s32 TalkMessageInfoDirector::findInAreaTalkMessageInfoIdx() const {
    s32 bestPriority = -999999;
    s32 bestIdx = -1;
    for (s32 i = 0; i < mInfoParamCount; ++i) {
        if (mInfoParams[i]->isEnableAppear() && bestPriority < mInfoParams[i]->getPriority()) {
            bestPriority = mInfoParams[i]->getPriority();
            bestIdx = i;
        }
    }
    return bestIdx;
}

bool TalkMessageInfoDirector::isActiveCapMessage(const char* labelName) const {
    bool isMessage = false;
    if (al::isNerve(this, &AppearMessage) || al::isNerve(this, &AppearMessageMoon) ||
        al::isNerve(this, &AppearMessageDemo) || al::isNerve(this, &AppearMessageBoss) ||
        al::isNerve(this, &AppearMessageMinigame)) {
        // BUG: This does nothing?
        al::isNerve(this, &AppearWaitEndMessage);

        isMessage = true;
    } else {
        bool isLow = al::isNerve(this, &AppearMessageLow);
        bool isWaitEnd = al::isNerve(this, &AppearWaitEndMessage);
        if (isLow)
            isMessage = true;
        else if (isWaitEnd) {
            if (mStateAppearWaitEndMessage->isActiveCapMessage())
                return true;
        } else if (labelName == nullptr) {
            if (al::isNerve(this, &Appear) || al::isNerve(this, &FollowText) ||
                al::isNerve(this, &Wait) || al::isNerve(this, &Disappear))
                return true;
            return false;
        }
    }
    if (isMessage && mStateAppearMessage->isActiveCapMessage(labelName))
        return true;
    return false;
}

bool TalkMessageInfoDirector::isDelayCapMessage(const char* labelName) const {
    bool isMessage =
        al::isNerve(this, &AppearMessage) || al::isNerve(this, &AppearMessageMoon) ||
        al::isNerve(this, &AppearMessageDemo) || al::isNerve(this, &AppearMessageBoss) ||
        al::isNerve(this, &AppearMessageMinigame) || al::isNerve(this, &AppearMessageLow);

    if (labelName == nullptr)
        return al::isNerve(this, &Delay);
    if (isMessage)
        return mStateAppearMessage->isDelayCapMessage(labelName);
    return false;
}

void TalkMessageInfoDirector::endForce() {
    if (al::isNerve(this, &AppearMessageBoss) || al::isNerve(this, &EndForce))
        return;

    if (al::isNerve(this, &AppearMessage) || al::isNerve(this, &AppearMessageDemo))
        mStateAppearMessage->kill();
    al::setNerve(this, &EndForce);
}

void TalkMessageInfoDirector::exeWatch() {
    mCurrentInfoParamIdx = findInAreaTalkMessageInfoIdx();
    if (mCurrentInfoParamIdx != -1) {
        TalkMessageInfoParam* param = nullptr;
        if (mCurrentInfoParamIdx >= 0 && mCurrentInfoParamIdx < mInfoParamCount)
            param = mInfoParams[mCurrentInfoParamIdx];
        param->onAppear();
        if (param->getDelayFrame() >= 1)
            al::setNerve(this, &Delay);
        else
            al::setNerve(this, &Appear);
    }
}

void TalkMessageInfoDirector::setNerveMessage() {
    TalkMessageInfoParam* param = nullptr;
    if (mCurrentInfoParamIdx >= 0 && mCurrentInfoParamIdx < mInfoParamCount)
        param = mInfoParams[mCurrentInfoParamIdx];
    param->onAppear();
    if (param->getDelayFrame() >= 1)
        al::setNerve(this, &Delay);
    else
        al::setNerve(this, &Appear);
}

void TalkMessageInfoDirector::exeDelay() {
    TalkMessageInfoParam* current = nullptr;
    if (mCurrentInfoParamIdx >= 0 && mCurrentInfoParamIdx < mInfoParamCount)
        current = mInfoParams[mCurrentInfoParamIdx];
    s32 nextIdx = findInAreaTalkMessageInfoIdx();
    if (nextIdx != -1 && mCurrentInfoParamIdx != nextIdx) {
        getInfoParam()->onAppearEnd();
        mCurrentInfoParamIdx = nextIdx;
        TalkMessageInfoParam* next = nullptr;
        if (mCurrentInfoParamIdx >= 0 && mCurrentInfoParamIdx < mInfoParamCount)
            next = mInfoParams[mCurrentInfoParamIdx];
        next->onAppear();
        if (next->getDelayFrame() >= 1)
            al::setNerve(this, &Delay);
        else
            al::setNerve(this, &Appear);
        return;
    }

    if (!current->isInArea() && current->isDelayCancel()) {
        al::setNerve(this, &Watch);
        return;
    }

    if (mInfoParams[mCurrentInfoParamIdx]->isRequestEndForce()) {
        al::setNerve(this, &EndForce);
        return;
    }

    if (al::isGreaterEqualStep(this, current->getDelayFrame()))
        al::setNerve(this, &Appear);
}

TalkMessageInfoParam* TalkMessageInfoDirector::getInfoParam() const {
    if (mCurrentInfoParamIdx < 0 || mCurrentInfoParamIdx >= mInfoParamCount)
        return nullptr;
    return mInfoParams[mCurrentInfoParamIdx];
}

// NONMATCHING: https://decomp.me/scratch/uBpVm
bool TalkMessageInfoDirector::changeTalkMessageIdx() {
    s32 bestPriority = -999999;
    s32 idx = -1;
    for (s32 i = 0; i < mInfoParamCount; i++) {
        if (mInfoParams[i]->isEnableAppear() && bestPriority < mInfoParams[i]->getPriority()) {
            bestPriority = mInfoParams[i]->getPriority();
            idx = i;
        }
    }

    if (idx != -1 && mCurrentInfoParamIdx != idx) {
        TalkMessageInfoParam* current = getInfoParam();
        current->onAppearEnd();
        mCurrentInfoParamIdx = idx;
        return true;
    }
    return false;
}

void TalkMessageInfoDirector::exeAppear() {
    if (al::isFirstStep(this)) {
        al::appearLayoutIfDead(mLayout);
        al::setPaneString(mLayout, "TxtMessage", mInfoParams[mCurrentInfoParamIdx]->getMessage(),
                          0);
        al::SimpleLayoutAppearWaitEnd* layout = mLayout;
        const char16* message = mInfoParams[mCurrentInfoParamIdx]->getMessage();
        ProjectReplaceTagProcessor processor(layout);
        al::startTextPaneAnim(layout, message, nullptr, &processor);
        al::startAction(mLayout, "Appear", nullptr);
        TalkMessageInfoParam* param = nullptr;
        if (mCurrentInfoParamIdx >= 0 && mCurrentInfoParamIdx < mInfoParamCount)
            param = mInfoParams[mCurrentInfoParamIdx];
        param->invalidateClipping();
        mCurrentPage = 0;
        al::startBgmSituation(this, "AppearCapMessage", false, true);
        alSeFunction::startSituation(mLayout, "キャップメッセージ", -1);
    }

    if (mInfoParams[mCurrentInfoParamIdx]->isRequestEndForce()) {
        al::setNerve(this, &EndForce);
        return;
    }

    if (al::isActionEnd(mLayout, nullptr))
        al::setNerve(this, &FollowText);
}

void TalkMessageInfoDirector::exeFollowText() {
    if (al::isFirstStep(this)) {
        if (!al::isActionPlaying(mLayout, "Wait", nullptr))
            al::startAction(mLayout, "Wait", nullptr);
    }

    if (mInfoParams[mCurrentInfoParamIdx]->isRequestEndForce()) {
        al::setNerve(this, &EndForce);
        return;
    }

    if (al::isEndTextPaneAnim(mLayout, false))
        al::setNerve(this, &Wait);
}

void TalkMessageInfoDirector::exeWait() {
    if (mInfoParams[mCurrentInfoParamIdx]->isRequestEndForce()) {
        al::setNerve(this, &EndForce);
        return;
    }

    s32 waitFrame =
        calcWaitFrame(mInfoParams[mCurrentInfoParamIdx]->getMessageParam(), mCurrentPage);
    if (rs::isKidsMode(mLayout))
        waitFrame <<= 1;

    if (al::isGreaterEqualStep(this, waitFrame)) {
        ProjectReplaceTagProcessor processor(mLayout);
        if (al::tryChangeNextPage(mLayout, nullptr, &processor)) {
            ++mCurrentPage;
            al::setNerve(this, &FollowText);
            return;
        }

        s32 idx = mCurrentInfoParamIdx;
        TalkMessageInfoParam* param = mInfoParams[idx];
        if (!param->isKeepDisp())
            al::setNerve(this, &Disappear);
        else {
            TalkMessageInfoParam* enableParam = nullptr;
            if (idx >= 0 && idx < mInfoParamCount)
                enableParam = param;
            if (!enableParam->isEnableAppear())
                al::setNerve(this, &EndForce);
        }
    }
}

s32 TalkMessageInfoDirector::calcWaitFrame(const TalkMessageInfoMessageParam* messageParam,
                                           s32 page) const {
    s32 language = al::getLanguageCode();
    s32 speed = language == 1 ? 4 : 8;
    if (language == 0)
        speed = 8;
    return speed * messageParam->getCharNum(page);
}

void TalkMessageInfoDirector::exeDisappear() {
    if (al::isFirstStep(this)) {
        al::startAction(mLayout, "End", nullptr);
        al::endBgmSituation(this, "AppearCapMessage", false);
        alSeFunction::endSituation(mLayout, "キャップメッセージ", -1);
    }

    if (!al::isActionEnd(mLayout, nullptr))
        return;

    TalkMessageInfoParam* param = getInfoParam();
    param->onAppearEnd();
    mLayout->kill();

    s32 idx = mCurrentInfoParamIdx;
    param = nullptr;
    bool shouldKill = false;
    if (idx >= 0) {
        TalkMessageInfoParam* rawParam = mInfoParams[idx];
        u32 endActionType = rawParam->getEndActionType() - 1;
        if (idx >= mInfoParamCount)
            param = nullptr;
        else
            param = rawParam;
        if (endActionType <= 1)
            shouldKill = true;
    } else {
        TalkMessageInfoParam* rawParam = param;
        s32 endActionType = rawParam->getEndActionType();
        param = rawParam;
        if (!(endActionType <= 0 || endActionType >= 3))
            shouldKill = true;
    }

    if (shouldKill) {
        param->kill();
        al::setNerve(this, &Watch);
        return;
    }

    if (!param->isInArea()) {
        al::setNerve(this, &Watch);
        return;
    }

    s32 nextIdx = findInAreaTalkMessageInfoIdx();
    if (nextIdx != -1) {
        if (mCurrentInfoParamIdx != nextIdx) {
            TalkMessageInfoParam* current = getInfoParam();
            current->onAppearEnd();
            mCurrentInfoParamIdx = nextIdx;
            setNerveMessage();
        }
    }
}

void TalkMessageInfoDirector::exeEndForce() {
    TalkMessageInfoParam* param = tryGetInfoParam();
    if (al::isFirstStep(this) && param != nullptr)
        param->onAppearEnd();

    if (updateEndForceCommon()) {
        if (param != nullptr) {
            s32 endActionType = param->getEndActionType();
            if (endActionType >= 1 && endActionType <= 2)
                param->kill();
        }
        al::setNerve(this, &Watch);
    }
}

TalkMessageInfoParam* TalkMessageInfoDirector::tryGetInfoParam() const {
    if (mCurrentInfoParamIdx < 0 || mCurrentInfoParamIdx >= mInfoParamCount)
        return nullptr;
    return mInfoParams[mCurrentInfoParamIdx];
}

bool TalkMessageInfoDirector::updateEndForceCommon() {
    if (al::isFirstStep(this)) {
        if (!al::isActionPlaying(mLayout, "End", nullptr))
            al::startAction(mLayout, "End", nullptr);
    }

    if (al::isActionEnd(mLayout, nullptr) || al::isDead(mLayout)) {
        al::killLayoutIfActive(mLayout);
        return true;
    }
    return false;
}

void TalkMessageInfoDirector::exeAppearMessage() {
    if (al::isFirstStep(this)) {
        mStateAppearMessage->appear();
        al::startBgmSituation(this, "AppearCapMessage", false, true);
        alSeFunction::startSituation(mLayout, "キャップメッセージ", -1);
    }

    if (mStateAppearMessage->isActiveCapMessage(nullptr)) {
        const char* labelName = mStateAppearMessage->getLabelName();
        if (labelName != nullptr) {
            CapMessageKeeper::CapMsgType moonGetSpecial1(
                CapMessageKeeper::CapMsgType::MoonGetSpecial1);
            CapMessageKeeper::CapMsgType moonGetSpecial2(
                CapMessageKeeper::CapMsgType::MoonGetSpecial2);
            if (al::isEqualString(labelName, moonGetSpecial1.text()) ||
                al::isEqualString(labelName, moonGetSpecial2.text()))
                mCapMessageKeeper->saveCapMsg(this, labelName);
        }
    }

    if (al::updateNerveStateAndNextNerve(this, &Watch)) {
        al::endBgmSituation(this, "AppearCapMessage", false);
        alSeFunction::endSituation(mLayout, "キャップメッセージ", -1);
    }
}

void TalkMessageInfoDirector::exeAppearMessageLow() {
    if (al::isFirstStep(this)) {
        mStateAppearMessage->appear();
        al::startBgmSituation(this, "AppearCapMessage", false, true);
        alSeFunction::startSituation(mLayout, "キャップメッセージ", -1);
    }

    if (al::updateNerveStateAndNextNerve(this, &Watch)) {
        al::endBgmSituation(this, "AppearCapMessage", false);
        alSeFunction::endSituation(mLayout, "キャップメッセージ", -1);
    }

    s32 idx = findInAreaTalkMessageInfoIdx();
    if (idx != -1) {
        mCurrentInfoParamIdx = idx;
        al::endBgmSituation(this, "AppearCapMessage", false);
        alSeFunction::endSituation(mLayout, "キャップメッセージ", -1);
        al::setNerve(this, &EndForce);
    }
}

void TalkMessageInfoDirector::exeAppearMessageDemo() {
    if (al::isFirstStep(this))
        mStateAppearMessage->appear();
    al::updateNerveStateAndNextNerve(this, &Watch);
}

void TalkMessageInfoDirector::exeAppearMessageBoss() {
    if (al::isFirstStep(this))
        mStateAppearMessage->appear();
    al::updateNerveStateAndNextNerve(this, &Watch);
}

void TalkMessageInfoDirector::exeAppearMessageMinigame() {
    if (al::isFirstStep(this))
        mStateAppearMessage->appear();
    al::updateNerveStateAndNextNerve(this, &Watch);
}

void TalkMessageInfoDirector::exeAppearMessageMoon() {
    if (al::isFirstStep(this)) {
        mStateAppearMessage->setDelay(45);
        mStateAppearMessage->appear();
    }
    al::updateNerveStateAndNextNerve(this, &Watch);
}

void TalkMessageInfoDirector::exeAppearWaitEndMessage() {
    if (al::isFirstStep(this))
        mStateAppearWaitEndMessage->appear();
    al::updateNerveStateAndNextNerve(this, &Watch);
}

void TalkMessageInfoDirector::registerTalkMessageInfoParam(TalkMessageInfoParam* param) {
    mInfoParams[mInfoParamCount] = param;
    ++mInfoParamCount;
}

void TalkMessageInfoDirector::tryCreateCapMessageList(const al::StageInfo* stageInfo,
                                                      const al::ActorInitInfo& info) {
    if (!GameDataFunction::checkIsNewWorldInAlreadyGoWorld(GameDataHolderAccessor(this)))
        return;

    al::PlacementInfo listInfo;
    if (!al::tryGetPlacementInfoAndCount(&listInfo, &mMoonInfoCount, stageInfo, "CapMessageList"))
        return;

    mMoonInfos = new TalkMessageInfoMoon*[mMoonInfoCount];
    for (s32 i = 0; i < mMoonInfoCount; ++i) {
        al::PlacementInfo placementInfo;
        al::getPlacementInfoByIndex(&placementInfo, listInfo, i);
        if (al::isObjectName(placementInfo, "TalkMessageInfoPointMoon")) {
            const char* displayName;
            al::getDisplayName(&displayName, placementInfo);
            TalkMessageInfoMoon* moonInfo = new TalkMessageInfoMoon(displayName);
            mMoonInfos[i] = moonInfo;
            mMoonInfos[i]->initPlacement(info, placementInfo);
        }
    }
}

void TalkMessageInfoDirector::startPlayCapMessage() {
    mIsPlayCapMessage = true;
}

void TalkMessageInfoDirector::endPlayCapMessage() {
    mIsPlayCapMessage = false;
    if (al::isNerve(this, &AppearMessageBoss) || al::isNerve(this, &EndForce))
        return;

    if (al::isNerve(this, &AppearMessage) || al::isNerve(this, &AppearMessageMoon) ||
        al::isNerve(this, &AppearMessageDemo) || al::isNerve(this, &AppearMessageBoss) ||
        al::isNerve(this, &AppearMessageMinigame) || al::isNerve(this, &AppearMessageLow) ||
        tryGetInfoParam() != nullptr)
        al::setNerve(this, &EndForce);
}

void TalkMessageInfoDirector::startMessageContinue(const char* labelName) {
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    const char16* message = al::getSystemMessageString(layout, "CapMessage", labelName);
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, message, nullptr, &processor);
    al::setNerve(this, &AppearWaitEndMessage);
    al::setNerve(this, &AppearWaitEndMessage);
}

void TalkMessageInfoDirector::setNerveAppearWaitEndSystemMessage(const char* categoryName,
                                                                 const char* labelName) {
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    const char16* message = al::getSystemMessageString(layout, categoryName, labelName);
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, message, nullptr, &processor);
    al::setNerve(this, &AppearWaitEndMessage);
}

void TalkMessageInfoDirector::endMessageContinue() {
    if (al::isNerve(this, &AppearWaitEndMessage))
        mStateAppearWaitEndMessage->end();
}

bool TalkMessageInfoDirector::trySetNerveAppearMessageLow(const char* categoryName,
                                                          const char* labelName, s32 waitFrame,
                                                          s32 delay) {
    if (!GameDataFunction::isEnableCap(GameDataHolderAccessor(this)) ||
        !al::isExistLabelInSystemMessage(mLayout, "CapMessage", labelName) ||
        al::isNerve(this, &Delay) || al::isNerve(this, &Appear) || al::isNerve(this, &FollowText) ||
        al::isNerve(this, &Wait) || al::isNerve(this, &AppearMessage) ||
        al::isNerve(this, &AppearMessageMoon) || al::isNerve(this, &AppearMessageDemo) ||
        al::isNerve(this, &AppearMessageBoss) || al::isNerve(this, &AppearMessageMinigame) ||
        al::isNerve(this, &AppearMessageLow))
        return false;

    const char16* message = al::getSystemMessageString(mLayout, categoryName, labelName);
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, message, nullptr, &processor);
    mStateAppearMessage->setWaitFrame(waitFrame);
    mStateAppearMessage->setDelay(delay);
    mStateAppearMessage->setLabelName(labelName);
    al::setNerve(this, &AppearMessageLow);
    return true;
}

void TalkMessageInfoDirector::setNerveAppearMessageCommon(const char* categoryName,
                                                          const char* labelName, s32 waitFrame,
                                                          s32 delay, const al::Nerve* nerve,
                                                          bool isStageMessage) {
    const char16* message = nullptr;
    if (isStageMessage)
        message = al::getStageMessageString(mLayout, categoryName, labelName);
    else
        message = al::getSystemMessageString(mLayout, categoryName, labelName);
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, message, nullptr, &processor);
    mStateAppearMessage->setWaitFrame(waitFrame);
    mStateAppearMessage->setDelay(delay);
    mStateAppearMessage->setLabelName(labelName);
    al::setNerve(this, nerve);
}

void TalkMessageInfoDirector::trySetNerveAppearMessageMoon() {
    if (al::isNerve(this, &AppearMessageMoon))
        return;

    TalkMessageInfoMoon* bestMoon = nullptr;
    for (s32 i = 0; i < mMoonInfoCount; ++i) {
        TalkMessageInfoMoon* moonInfo = mMoonInfos[i];
        moonInfo->update();
        if (moonInfo->isRequest()) {
            moonInfo->accepted();
            if (bestMoon == nullptr || bestMoon->getPriority() < moonInfo->getPriority())
                bestMoon = moonInfo;
        }
    }

    if (bestMoon == nullptr)
        return;

    const char* labelName = bestMoon->getLabelName();
    const char16* message = al::getSystemMessageString(mLayout, "CapMessage", labelName);
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, message, nullptr, &processor);
    mStateAppearMessage->setWaitFrame(90);
    mStateAppearMessage->setDelay(45);
    mStateAppearMessage->setLabelName("(GetMoonSpecial)");
    al::setNerve(this, &AppearMessageMoon);
}

void TalkMessageInfoDirector::setNerveAppearMessageMoon(const char* categoryName,
                                                        const char* labelName) {
    const char16* message = al::getSystemMessageString(mLayout, categoryName, labelName);
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, message, nullptr, &processor);
    mStateAppearMessage->setWaitFrame(90);
    mStateAppearMessage->setDelay(45);
    mStateAppearMessage->setLabelName("(GetMoonSpecial)");
    al::setNerve(this, &AppearMessageMoon);
}

void TalkMessageInfoDirector::setNerveAppearMessage(const char* categoryName, const char* labelName,
                                                    s32 waitFrame, s32 delay, bool isDemo,
                                                    bool isStageMessage) {
    if (isDemo)
        setNerveAppearMessageCommon(categoryName, labelName, waitFrame, delay, &AppearMessageDemo,
                                    isStageMessage);
    else
        setNerveAppearMessageCommon(categoryName, labelName, waitFrame, delay, &AppearMessage,
                                    isStageMessage);
}

void TalkMessageInfoDirector::setNerveAppearMessageBoss(const char* categoryName,
                                                        const char* labelName, s32 waitFrame,
                                                        s32 delay, bool isStageMessage) {
    if (rs::isSeparatePlay(mLayout))
        return;

    const char16* message = nullptr;
    if (isStageMessage)
        message = al::getStageMessageString(mLayout, categoryName, labelName);
    else
        message = al::getSystemMessageString(mLayout, categoryName, labelName);
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, message, nullptr, &processor);
    mStateAppearMessage->setWaitFrame(waitFrame);
    mStateAppearMessage->setDelay(delay);
    mStateAppearMessage->setLabelName(labelName);
    al::setNerve(this, &AppearMessageBoss);
}

void TalkMessageInfoDirector::setNerveAppearMessageMinigame(const char* categoryName,
                                                            const char* labelName, s32 waitFrame,
                                                            s32 delay) {
    const char16* message = al::getSystemMessageString(mLayout, categoryName, labelName);
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, message, nullptr, &processor);
    mStateAppearMessage->setWaitFrame(waitFrame);
    mStateAppearMessage->setDelay(delay);
    mStateAppearMessage->setLabelName(labelName);
    al::setNerve(this, &AppearMessageMinigame);
}

void TalkMessageInfoDirector::setNerveAppearMessageMoonLowText(const char16* message,
                                                               const char* labelName) {
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, message, nullptr, &processor);
    mStateAppearMessage->setWaitFrame(90);
    mStateAppearMessage->setDelay(45);
    mStateAppearMessage->setLabelName("(GetMoonSpecial)");
    al::setNerve(this, &AppearMessageMoon);
}

void TalkMessageInfoDirector::setNerveAppearMessageMoonNum(const char* categoryName,
                                                           const char* labelName, s32 num) {
    sead::WFixedSafeString<256>* moonNumText = &mMoonNumText;
    al::SimpleLayoutAppearWaitEnd* layout = mLayout;
    const al::IUseMessageSystem* messageSystem = layout;
    const char16* message = al::getSystemMessageString(messageSystem, categoryName, labelName);
    al::replaceMessageTagScore(moonNumText, messageSystem, message, num, "Moon");
    const char16* replacedMessage = moonNumText->cstr();
    layout = mLayout;
    ProjectReplaceTagProcessor processor(layout);
    al::startTextPaneAnim(layout, replacedMessage, nullptr, &processor);
    mStateAppearMessage->setWaitFrame(90);
    mStateAppearMessage->setDelay(45);
    mStateAppearMessage->setLabelName("(GetMoonSpecial)");
    al::setNerve(this, &AppearMessageMoon);
}

void TalkMessageFunction::registerTalkMessageInfoParam(
    TalkMessageInfoParam* param, const al::IUseSceneObjHolder* sceneObjHolder) {
    getTalkMessageInfoDirector(sceneObjHolder)->registerTalkMessageInfoParam(param);
}

TalkMessageInfoDirector*
TalkMessageFunction::getTalkMessageInfoDirector(const al::IUseSceneObjHolder* sceneObjHolder) {
    return nullptr;
}

bool TalkMessageFunction::isExistTalkMessageInfoDirector(
    const al::IUseSceneObjHolder* sceneObjHolder) {
    return false;
}

al::NerveKeeper* TalkMessageInfoDirector::getNerveKeeper() const {
    return mNerveKeeper;
}

al::SceneObjHolder* TalkMessageInfoDirector::getSceneObjHolder() const {
    return mSceneObjHolder;
}

al::AudioKeeper* TalkMessageInfoDirector::getAudioKeeper() const {
    return mAudioKeeper;
}
