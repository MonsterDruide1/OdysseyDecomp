#include "Amiibo/HelpAmiiboDirector.h"

#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Player/PlayerHolder.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Se/SeFunction.h"

#include "Amiibo/HelpAmiiboCoinCollect.h"
#include "Amiibo/HelpAmiiboCountUpCoin.h"
#include "Amiibo/HelpAmiiboExecutor.h"
#include "Amiibo/HelpAmiiboLifeMaxUpItem.h"
#include "Amiibo/HelpAmiiboPlayerInvincible.h"
#include "Scene/SceneObjFactory.h"
#include "System/GameDataFunction.h"
#include "System/ProjectNfpDirector.h"
#include "Util/PlayerUtil.h"
#include "Util/StageInputFunction.h"

namespace {
NERVE_IMPL(HelpAmiiboDirector, Wait);
NERVE_IMPL(HelpAmiiboDirector, CountHold);
NERVE_IMPL(HelpAmiiboDirector, Active);
NERVE_IMPL(HelpAmiiboDirector, IconClose);

NERVES_MAKE_STRUCT(HelpAmiiboDirector, Wait, CountHold, Active, IconClose);
}  // namespace

HelpAmiiboDirector::HelpAmiiboDirector() {}

void HelpAmiiboDirector::init(ProjectNfpDirector* projectNfpDirector,
                              const al::PlayerHolder* playerHolder,
                              al::AudioDirector* audioDirector,
                              const al::LayoutInitInfo& initInfo) {
    mProjectNfpDirector = projectNfpDirector;
    mPlayerHolder = playerHolder;
    mAudioKeeper =
        alAudioKeeperFunction::createAudioKeeper(audioDirector, "HelpAmiiboDirector", nullptr);

    mNerveKeeper = new al::NerveKeeper(this, &NrvHelpAmiiboDirector.Wait, 0);
    mSimpleLayout =
        new al::SimpleLayoutAppearWaitEnd("amiiboアイコン", "AmiiboMode", initInfo, nullptr, false);
    mLayoutActor = new al::LayoutActor("amiiboアイコン[アイコンパーツ]");
    al::initLayoutPartsActor(mLayoutActor, mSimpleLayout, initInfo, "ParAmiiboIcon", nullptr);
}

void HelpAmiiboDirector::initAfterPlacementSceneObj(const al::ActorInitInfo& initInfo) {
    al::LiveActor* actor = al::getPlayerActor(mPlayerHolder, 0);
    mCoinCollect = new HelpAmiiboCoinCollect(this, actor);

    mTouchEntries.allocBuffer(4, nullptr);
    mTouchEntries.pushBack(new HelpAmiiboPlayerInvincible(this, actor));
    mTouchEntries.pushBack(new HelpAmiiboLifeMaxUpItem(this, actor));
    mTouchEntries.pushBack(mCoinCollect);
    mTouchEntries.pushBack(new HelpAmiiboCountUpCoin(this, actor));

    mTouchEntries[0]->initAfterPlacement(initInfo);
    mTouchEntries[1]->initAfterPlacement(initInfo);
    mTouchEntries[2]->initAfterPlacement(initInfo);
    mTouchEntries[3]->initAfterPlacement(initInfo);
}

static bool isTriggerTouchAmiibo(HelpAmiiboType type,
                                 const sead::PtrArray<HelpAmiiboExecutor>& touchEntries) {
    volatile HelpAmiiboType volatileType = type;

    s32 entryCount = touchEntries.size();
    for (s32 i = 0; i < entryCount; i++)
        if (touchEntries[i]->getType() == volatileType && touchEntries[i]->isTouched())
            return true;

    return false;
}

bool HelpAmiiboDirector::isTriggerTouchAmiiboMario() const {
    return isTriggerTouchAmiibo(HelpAmiiboType::Mario, mTouchEntries);
}

bool HelpAmiiboDirector::isTriggerTouchAmiiboPeach() const {
    return isTriggerTouchAmiibo(HelpAmiiboType::Peach, mTouchEntries);
}

bool HelpAmiiboDirector::isTriggerTouchAmiiboKoopa() const {
    return isTriggerTouchAmiibo(HelpAmiiboType::Koopa, mTouchEntries);
}

bool HelpAmiiboDirector::isTriggerTouchAmiiboYoshi() const {
    return isTriggerTouchAmiibo(HelpAmiiboType::Yoshi, mTouchEntries);
}

bool HelpAmiiboDirector::isTriggerTouchAmiiboAll() const {
    return isTriggerTouchAmiiboMario() || isTriggerTouchAmiiboPeach() ||
           isTriggerTouchAmiiboKoopa() || isTriggerTouchAmiiboYoshi();
}

void HelpAmiiboDirector::execute() {
    al::LiveActor* actor = al::getPlayerActor(mPlayerHolder, 0);
    if (rs::isPlayerEnablePeachAmiibo(actor))
        mIsTouchAmiibo = true;

    mNerveKeeper->update();
    if (al::isActionPlaying(mLayoutActor, "Reject", nullptr)) {
        if (al::isActionEnd(mLayoutActor, nullptr))
            al::startAction(mLayoutActor, "Wait", nullptr);
    }

    s32 entries = mTouchEntries.size();
    for (s32 i = 0; i < entries; i++)
        mTouchEntries[i]->tryExecute();
}

void HelpAmiiboDirector::reset() {
    mSimpleLayout->end();
    mProjectNfpDirector->stop();
    al::setNerve(this, &NrvHelpAmiiboDirector.Wait);
}

bool HelpAmiiboDirector::isHelpAmiiboMode() const {
    if (al::isNerve(this, &NrvHelpAmiiboDirector.CountHold))
        return true;
    return al::isNerve(this, &NrvHelpAmiiboDirector.Active);
}

void HelpAmiiboDirector::appearCoinCollectEffect() {
    mCoinCollect->appearEffect();
}

void HelpAmiiboDirector::killCoinCollectEffect() {
    mCoinCollect->killEffect();
}

static bool isNotExecutable(const al::PlayerHolder* mPlayerHolder) {
    al::LiveActor* actor = al::getPlayerActor(mPlayerHolder, 0);

    return GameDataFunction::isRaceStartYukimaru(actor) || rs::isSeparatePlay(actor);
}

bool HelpAmiiboDirector::tryExecute(const al::NfpInfo* nfpInfo) {
    if (isNotExecutable(mPlayerHolder))
        return false;

    if (!mIsTouchAmiibo) {
        al::startSe(this, "Invalid");
        return false;
    }

    s32 entries = mTouchEntries.size();
    for (s32 i = 0; i < entries; i++) {
        if (mTouchEntries[i]->isTriggerTouch(*nfpInfo) && mTouchEntries[i]->isEnableUse() &&
            mTouchEntries[i]->tryTouch(*nfpInfo)) {
            mTouchEntries[i]->tryExecute();
            al::startSe(this, "TouchAmiibo");
            al::startHitReaction(mSimpleLayout, "タッチ", nullptr);
            mIsTouchAmiibo = false;
            return true;
        }
    }

    al::startSe(this, "Invalid");
    al::startAction(mLayoutActor, "Reject", nullptr);
    return false;
}

void HelpAmiiboDirector::exeWait() {
    al::LiveActor* actor = al::getPlayerActor(mPlayerHolder, 0);
    if (rs::isPlayerHackFukankun(actor))
        return;

    if (rs::isTriggerAmiiboMode(actor))
        al::setNerve(this, &NrvHelpAmiiboDirector.CountHold);
}

void HelpAmiiboDirector::exeCountHold() {
    al::LiveActor* actor = al::getPlayerActor(mPlayerHolder, 0);

    if (!rs::isHoldAmiiboMode(actor))
        al::setNerve(this, &NrvHelpAmiiboDirector.Wait);
    else if (al::isGreaterEqualStep(this, 8))
        al::setNerve(this, &NrvHelpAmiiboDirector.Active);
}

void HelpAmiiboDirector::exeActive() {
    al::LiveActor* actor = al::getPlayerActor(mPlayerHolder, 0);
    if (al::isFirstStep(this))
        mProjectNfpDirector->start();

    if (!rs::isHoldAmiiboMode(actor)) {
        mProjectNfpDirector->stop();
        al::setNerve(this, &NrvHelpAmiiboDirector.IconClose);
        return;
    }

    if (al::isLessStep(this, 10) && mProjectNfpDirector->isNfpErrorHandled()) {
        mProjectNfpDirector->stop();
        al::setNerve(this, &NrvHelpAmiiboDirector.Wait);
        al::startAction(mLayoutActor, "Hide", nullptr);
        return;
    }

    if (al::isStep(this, 10)) {
        mSimpleLayout->appear();
        al::startAction(mLayoutActor, "Appear", nullptr);
    }

    if (al::isActionPlaying(mLayoutActor, "Appear", nullptr)) {
        if (al::isActionEnd(mLayoutActor, nullptr))
            al::startAction(mLayoutActor, "Wait", nullptr);
    }

    al::NfpInfo* nfpInfo = mProjectNfpDirector->tryGetTriggerTouchNfpInfo();
    if (nfpInfo && tryExecute(nfpInfo)) {
        mProjectNfpDirector->stop();
        al::setNerve(this, &NrvHelpAmiiboDirector.IconClose);
    }
}

void HelpAmiiboDirector::exeIconClose() {
    if (al::isFirstStep(this))
        al::startAction(mLayoutActor, "End", nullptr);
    if (al::isActionEnd(mLayoutActor, nullptr) || al::isDead(mSimpleLayout)) {
        mSimpleLayout->kill();
        al::setNerve(this, &NrvHelpAmiiboDirector.Wait);
    }
}

namespace AmiiboFunction {

void tryCreateHelpAmiiboDirector(const al::IUseSceneObjHolder* objHolder) {
    al::createSceneObj(objHolder, SceneObjID_HelpAmiiboDirector);
}

bool isTriggerTouchAmiiboMario(const al::IUseSceneObjHolder* objHolder) {
    HelpAmiiboDirector* director = al::tryGetSceneObj<HelpAmiiboDirector>(objHolder);

    if (!director)
        return false;
    return director->isTriggerTouchAmiiboMario();
}

bool isTriggerTouchAmiiboPeach(const al::IUseSceneObjHolder* objHolder) {
    HelpAmiiboDirector* director = al::tryGetSceneObj<HelpAmiiboDirector>(objHolder);

    if (!director)
        return false;
    return director->isTriggerTouchAmiiboPeach();
}

bool isTriggerTouchAmiiboKoopa(const al::IUseSceneObjHolder* objHolder) {
    HelpAmiiboDirector* director = al::tryGetSceneObj<HelpAmiiboDirector>(objHolder);

    if (!director)
        return false;
    return director->isTriggerTouchAmiiboKoopa();
}

bool isTriggerTouchAmiiboAll(const al::IUseSceneObjHolder* objHolder) {
    HelpAmiiboDirector* director = al::tryGetSceneObj<HelpAmiiboDirector>(objHolder);

    if (!director)
        return false;
    return director->isTriggerTouchAmiiboAll();
}

}  // namespace AmiiboFunction

namespace rs {

void resetHelpAmiiboState(const al::IUseSceneObjHolder* objHolder) {
    HelpAmiiboDirector* director = al::tryGetSceneObj<HelpAmiiboDirector>(objHolder);

    if (director)
        director->reset();
}

bool isHelpAmiiboMode(const al::IUseSceneObjHolder* objHolder) {
    HelpAmiiboDirector* director = al::tryGetSceneObj<HelpAmiiboDirector>(objHolder);

    if (!director)
        return false;
    return director->isHelpAmiiboMode();
}

}  // namespace rs
