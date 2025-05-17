#include "Library/Play/Layout/BalloonMessage.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/TalkMessageVoicePlayer.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Se/SeFunction.h"

namespace {
using namespace al;

NERVE_IMPL(BalloonMessage, Wait)
NERVE_IMPL(BalloonMessage, Appear)
NERVE_IMPL(BalloonMessage, Hide)
NERVE_IMPL(BalloonMessage, End)

NERVES_MAKE_NOSTRUCT(BalloonMessage, Wait)
NERVES_MAKE_STRUCT(BalloonMessage, Appear, Hide, End)
}  // namespace

namespace al {
BalloonMessage* BalloonMessage::create(const LiveActor* hostActor, const LayoutInitInfo& info,
                                       const BalloonMessageInitParam& initParam,
                                       bool isAutoUpdate) {
    return new BalloonMessage(hostActor, info, initParam, isAutoUpdate);
}

BalloonMessage::BalloonMessage(const LiveActor* hostActor, const LayoutInitInfo& info,
                               const BalloonMessageInitParam& initParam, bool isAutoUpdate)
    : LayoutActor(initParam.name), mHostActor(hostActor), mPaneName(initParam.paneName),
      mAppearDist(initParam.appearDist), mKillDist(initParam.killDist),
      mPlayerIndex(initParam.playerIndex), _158(initParam._38), mIsAutoUpdate(isAutoUpdate),
      _15d(initParam._3c) {
    mPosOffset.y = initParam.yPosOffset;

    initLayoutActor(this, info, initParam.archiveName, initParam.suffix);

    mTalkMessageVoicePlayer = new TalkMessageVoicePlayer();
    mTalkMessageVoicePlayer->set_420(true);

    setText(initParam.message);
    initNerve(&Wait, 0);

    kill();
}

void BalloonMessage::setText(const char* message) {
    setPaneStringFormat(this, mPaneName, message);
}

inline void startAppear(LayoutActor* layout) {
    startAction(layout, "Appear", nullptr);
}

void BalloonMessage::appear() {
    updateTrans();
    hidePushA();

    if (!mIsAutoUpdate) {
        startAction(this, "Appear", nullptr);
        LayoutActor::appear();

        if (!mIsTalkMessageVoicePlayerStarted)
            mTalkMessageVoicePlayer->start(this, mHostActor, getPaneStringBuffer(this, mPaneName),
                                           5);

        setNerve(this, &NrvBalloonMessage.Appear);

        return;
    }

    if (isEnableAppear()) {
        startAppear(this);
        LayoutActor::appear();
        setActionFrameRate(this, 1.0f, nullptr);
        setNerve(this, &NrvBalloonMessage.Appear);

        return;
    }

    LayoutActor::appear();
    startFreezeActionEnd(this, "End", nullptr);
    setNerve(this, &NrvBalloonMessage.Hide);
}

void BalloonMessage::updateTrans() {
    sead::Vector2f layoutPos = sead::Vector2f::zero;

    if (_158 == 0)
        calcLayoutPosFromWorldPos(&layoutPos, mHostActor, getTrans(mHostActor) + mPosOffset);
    else
        calcLayoutPosFromWorldPosSub(&layoutPos, mHostActor, getTrans(mHostActor) + mPosOffset);

    setLocalTrans(this, layoutPos);
}

void BalloonMessage::hidePushA() {
    if (!isExistPane(this, "TxtA"))
        return;

    hidePane(this, "TxtA");
}

bool BalloonMessage::isEnableAppear() const {
    if (!isNerve(this, &NrvBalloonMessage.End) && !isNerve(this, &NrvBalloonMessage.Hide) &&
        isAlive())
        return false;

    return isNearPlayerActor(mAppearDist);
}

void BalloonMessage::appearWithPushA() {
    appear();
    showPushA();
}

void BalloonMessage::showPushA() {
    if (!isExistPane(this, "TxtA"))
        return;

    showPane(this, "TxtA");
}

void BalloonMessage::control() {
    if (mIsAutoUpdate)
        update();

    if (!_15d || isNerve(this, &NrvBalloonMessage.End) || isNerve(this, &NrvBalloonMessage.Hide) ||
        (!isClipped(mHostActor) && !isDead(mHostActor))) {
        mTalkMessageVoicePlayer->update();
        updateTrans();

        return;
    }

    setNerve(this, &NrvBalloonMessage.End);
}

void BalloonMessage::update() {
    if (isEnableAppear())
        appear();

    if (isEnableEnd())
        setNerve(this, &NrvBalloonMessage.End);
}

bool BalloonMessage::isWait() const {
    return isNerve(this, &Wait);
}

bool BalloonMessage::isVoicePlayerPlaying() const {
    if (mTalkMessageVoicePlayer->isPlaying())
        return true;

    if (mSeName.cstr() == nullptr)
        return false;

    return checkIsPlayingSe(mHostActor, mSeName.cstr(), nullptr);
}

bool BalloonMessage::isShowPushA() const {
    return !isHidePane(this, "TxtA");
}

bool BalloonMessage::isEnableEnd() const {
    if (isNerve(this, &NrvBalloonMessage.End) || isNerve(this, &NrvBalloonMessage.Hide) ||
        !isAlive())
        return false;

    return !isNearPlayerActor(mKillDist);
}

void BalloonMessage::end() {
    if (isNerve(this, &NrvBalloonMessage.End) || isNerve(this, &NrvBalloonMessage.Hide))
        return;

    setNerve(this, &NrvBalloonMessage.End);
}

void BalloonMessage::setTextW(const char16* message) {
    setPaneString(this, mPaneName, message, 0);
}

void BalloonMessage::exeAppear() {
    if (isActionEnd(this, nullptr))
        setNerve(this, &Wait);
}

void BalloonMessage::exeWait() {
    if (isFirstStep(this))
        startAction(this, "Wait", nullptr);
}

void BalloonMessage::exeEnd() {
    if (isFirstStep(this))
        startAction(this, "End", nullptr);

    if (isActionEnd(this, nullptr)) {
        if (mIsAutoUpdate)
            setNerve(this, &NrvBalloonMessage.Hide);
        else
            kill();
    }
}

void BalloonMessage::exeHide() {}

bool BalloonMessage::isNearPlayerActor(f32 threshold) const {
    if (mPlayerIndex >= 0)
        return isNear(mHostActor, getPlayerActor(mHostActor, mPlayerIndex), threshold);

    return isNearPlayer(mHostActor, threshold);
}

BalloonMessage* createBalloonMessage(const LiveActor* hostActor, const ActorInitInfo& info,
                                     const BalloonMessageInitParam& initParam) {
    return BalloonMessage::create(hostActor, getLayoutInitInfo(info), initParam, true);
}

BalloonMessage* createBalloonMessageNoAutoUpdate(const LiveActor* hostActor,
                                                 const ActorInitInfo& info,
                                                 const BalloonMessageInitParam& initParam) {
    return BalloonMessage::create(hostActor, getLayoutInitInfo(info), initParam, false);
}

BalloonMessage* createBalloonMessage(const LiveActor* hostActor, const ActorInitInfo& info) {
    BalloonMessageInitParam initParam = {.message = "未設定"};

    return BalloonMessage::create(hostActor, getLayoutInitInfo(info), initParam, true);
}

BalloonMessage* createBalloonMessage(const LiveActor* hostActor, const ActorInitInfo& info,
                                     const char* message) {
    BalloonMessageInitParam initParam = {.message = message};

    return BalloonMessage::create(hostActor, getLayoutInitInfo(info), initParam, true);
}
}  // namespace al
