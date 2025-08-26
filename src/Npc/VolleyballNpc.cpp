#include "Npc/VolleyballNpc.h"

#include <basis/seadTypes.h>
#include <math/seadMathCalcCommon.h>

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaShape.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Math/MathUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Item/Shine.h"
#include "MapObj/CapMessageShowInfo.h"
#include "MapObj/VolleyballBase.h"
#include "Npc/NpcEventStateScare.h"
#include "Npc/TalkNpcCap.h"
#include "Npc/VolleyballBall.h"
#include "Npc/VolleyballLayout.h"
#include "System/GameDataUtil.h"
#include "Util/ItemUtil.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(VolleyballNpc, Wait);
NERVE_IMPL(VolleyballNpc, Scare);
NERVE_IMPL(VolleyballNpc, ReactionCap);
NERVE_IMPL(VolleyballNpc, ReactionTrample);
NERVE_IMPL(VolleyballNpc, Miss);
NERVE_IMPL(VolleyballNpc, WaitRetry);
NERVE_IMPL(VolleyballNpc, CapMessageWaitForThrough);
NERVE_IMPL(VolleyballNpc, LetsPlay);
NERVE_IMPL(VolleyballNpc, CapMessage);
NERVE_IMPL(VolleyballNpc, AttackWait);
NERVE_IMPL(VolleyballNpc, WaitAfterGame);
NERVE_IMPL(VolleyballNpc, Attack);
NERVE_IMPL(VolleyballNpc, WaitAttackSign);
NERVE_IMPL(VolleyballNpc, AttackSign);
NERVE_IMPL_(VolleyballNpc, AttackWithNewBall, Attack);

NERVES_MAKE_STRUCT(VolleyballNpc, Wait, Scare, ReactionCap, ReactionTrample, Miss, WaitRetry,
                   CapMessageWaitForThrough, LetsPlay, CapMessage, AttackWait, WaitAfterGame,
                   Attack, WaitAttackSign);

NERVES_MAKE_NOSTRUCT(VolleyballNpc, AttackSign, AttackWithNewBall);
}  // namespace

static const sead::Vector3f gVector = {0.0f, 150.0f, 0.0f};
static const NpcEventStateScareActionParam gStateScare("ScaredStart", "Scared", "ScaredEnd");

VolleyballNpc::VolleyballNpc(const char* name) : al::LiveActor(name) {}

void VolleyballNpc::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "SeaMan", "Volleyball");
    al::startMtpAnim(this, "Body1");
    TalkNpcCap::createForVolleyballNpc(this, initInfo);
    mEventFlowExecutor = rs::initEventFlowSuffix(this, initInfo, "Volleyball", nullptr, nullptr);
    al::MessageTagDataHolder* messageHolder = al::initMessageTagDataHolder(1);
    al::registerMessageTagDataScore(messageHolder, "Score", &mCurrentScore);
    rs::initEventMessageTagDataHolder(mEventFlowExecutor, messageHolder);
    mBestScore = rs::getVolleyballBestCount(this);
    mDayScore = rs::getVolleyballDayCount(this);
    mShineClear = rs::tryInitLinkShine(initInfo, "ShineActor", 0);
    mShineSuperClear = rs::tryInitLinkShine(initInfo, "ShineActor", 1);

    al::AreaObjGroup* gameAreaGroup = al::createLinkAreaGroup(
        this, initInfo, "VolleyballArea", "バレー開始エリア", "バレー開始エリア");
    if (gameAreaGroup && gameAreaGroup->getSize() == 1)
        mGameArea = gameAreaGroup->getAreaObj(0);
    else
        makeActorDead();

    al::AreaObjGroup* ballAreaGroup =
        al::createLinkAreaGroup(this, initInfo, "BallArea", "ボール範囲", "ボール範囲");
    if (ballAreaGroup && ballAreaGroup->getSize() == 1)
        mBallArea = ballAreaGroup->getAreaObj(0);
    else
        makeActorDead();

    al::AreaObjGroup* layoutAreaGroup =
        al::createLinkAreaGroup(this, initInfo, "LayoutArea", "レイアウト範囲", "レイアウト範囲");
    if (layoutAreaGroup && layoutAreaGroup->getSize() == 1)
        mLayoutArea = layoutAreaGroup->getAreaObj(0);
    else
        makeActorDead();

    al::getArg(&mLevelUpCount, initInfo, "LevelUpCount");
    al::getArg(&mLevelUpSpeed, initInfo, "LevelUpSpeed");
    al::getArg(&mMaxSpeed, initInfo, "MaxSpeed");
    al::getArg(&mInitSpeed, initInfo, "InitSpeed");
    al::getArg(&mBallNumMax, initInfo, "BallNumMax");
    al::getArg(&mLevelUpBallCount, initInfo, "LevelUpBallCount");
    al::getArg(&mClearCount, initInfo, "ClearCount");
    al::getArg(&mSuperClearCount, initInfo, "SuperClearCount");

    al::DeriveActorGroup<VolleyballBall>* ballGroup =
        new al::DeriveActorGroup<VolleyballBall>("バレーボールのボール", mBallNumMax);
    mBallGroup = ballGroup;
    for (s32 i = 0; i < ballGroup->getMaxActorCount(); i++) {
        VolleyballBall* ball = new VolleyballBall("バレーボールのボール");
        al::initCreateActorNoPlacementInfo(ball, initInfo);
        ballGroup->registerActor(ball);
    }

    ballGroup = mBallGroup;
    for (s32 i = 0; i < ballGroup->getActorCount(); i++) {
        ballGroup->getDeriveActor(i)->setNpc(this);
        ballGroup = mBallGroup;
    }

    mBase = new VolleyballBase("土台");
    al::initLinksActor(mBase, initInfo, "Base", 0);

    mLayout = new VolleyballLayout(this, initInfo);

    al::calcFrontDir(&mFrontDir, this);

    if (mShineClear && mShineClear->isGot()) {
        if (mShineSuperClear && mShineSuperClear->isGot())
            rs::requestSwitchTalkNpcEventVolleyBall(this, 2);
        else
            rs::requestSwitchTalkNpcEventVolleyBall(this, 1);
    }

    al::initNerve(this, &NrvVolleyballNpc.Wait, 1);
    mNpcEventStateScare = new NpcEventStateScare(this, &gStateScare);
    al::initNerveState(this, mNpcEventStateScare, &NrvVolleyballNpc.Scare, "怖がり");
    al::startAction(this, "VolleyballWait");
    makeActorAlive();
}

void VolleyballNpc::control() {
    if (!al::isNerve(this, &NrvVolleyballNpc.ReactionCap) &&
        !al::isNerve(this, &NrvVolleyballNpc.ReactionTrample)) {
        al::turnFrontToPlayer(this, 1.5f);
    }

    mLayout->updateNerve();
    if (rs::isPlayerHack(this) && !al::isNerve(this, &NrvVolleyballNpc.Wait) &&
        !al::isNerve(this, &NrvVolleyballNpc.Scare)) {
        mLayout->end();
        endGame();
        if (!al::isNerve(this, &NrvVolleyballNpc.ReactionCap) &&
            !al::isNerve(this, &NrvVolleyballNpc.ReactionTrample)) {
            al::setNerve(this, &NrvVolleyballNpc.Wait);
        }
        return;
    }

    if (0 < _19c) {
        _19c -= 1;
        rs::updateEventFlow(mEventFlowExecutor);
    }
}

void VolleyballNpc::endGame() {
    for (s32 i = 0; i < mBallGroup->getActorCount(); i++)
        mBallGroup->getDeriveActor(i)->endGame();
    rs::resetEventBalloonFilter(this);
    al::tryOffStageSwitch(this, "SwitchInGameOnOff");
    al::endBgmSituation(this, "Volleyball", false);
}

bool VolleyballNpc::receiveEvent(const al::EventFlowEventData* event) {
    if (al::isEventName(event, "StartGame")) {
        startGame(true);
        return true;
    }
    return false;
}

void VolleyballNpc::startGame(bool startEvent) {
    mBall = nullptr;
    for (s32 i = 0; i < mBallGroup->getActorCount(); i++) {
        VolleyballBall* ball = mBallGroup->getDeriveActor(i);
        if (ball->isRetryEnd())
            mBall = ball;
        ball->reset();
    }

    mSpeed = mInitSpeed;
    _150 = 0;
    mCurrentScore = 0;
    _174 = 1;
    if (mBall == nullptr)
        mBall = (VolleyballBall*)mBallGroup->tryFindDeadActor();
    al::invalidateClipping(this);
    _19c = 60;
    rs::setEventBalloonFilterOnlyMiniGame(this);
    al::tryOnStageSwitch(this, "SwitchInGameOnOff");
    al::startBgmSituation(this, "Volleyball", false, true);
    al::setNerve(this, &AttackSign);

    if (startEvent)
        rs::startEventFlow(mEventFlowExecutor, "GameStart");
}

void VolleyballNpc::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEye(self)) {
        rs::sendMsgEventFlowScareCheck(other, self, mEventFlowExecutor);
        return;
    }
    if (rs::sendMsgPushToPlayer(other, self))
        return;
    al::sendMsgPush(other, self);
}

bool VolleyballNpc::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardTargetMarker(message))
        return true;

    if (al::isNerve(this, &NrvVolleyballNpc.Wait) || al::isNerve(this, &NrvVolleyballNpc.Scare) ||
        al::isNerve(this, &NrvVolleyballNpc.ReactionCap) ||
        al::isNerve(this, &NrvVolleyballNpc.ReactionTrample)) {
        if (rs::isMsgCapReflect(message) || rs::isMsgSeedReflect(message) ||
            rs::isMsgHosuiAttack(message)) {
            rs::requestHitReactionToAttacker(message, self, other);
            if (!al::isNerve(this, &NrvVolleyballNpc.ReactionCap) ||
                al::isGreaterEqualStep(this, 30)) {
                al::setNerve(this, &NrvVolleyballNpc.ReactionCap);
            }
            return true;
        }

        if (al::isMsgPlayerTrampleReflect(message) || rs::isMsgHosuiTrampleReflect(message)) {
            rs::requestHitReactionToAttacker(message, self, other);
            al::setNerve(this, &NrvVolleyballNpc.ReactionTrample);
            return true;
        }
    }

    if (!rs::isMsgNpcCapReactionAll(message))
        return false;
    rs::requestHitReactionToAttacker(message, self, other);
    return true;
}

void VolleyballNpc::addSuccessCount() {
    mCurrentScore = sead::Mathi::min(mCurrentScore + 1, 99999);

    if (mCurrentScore == 1)
        mLayout->start();

    if (mCurrentScore == mClearCount) {
        al::tryOnStageSwitch(this, "SwitchClearOn");
        if (mShineSuperClear->isGot())
            rs::requestSwitchTalkNpcEventVolleyBall(this, 2);
        else
            rs::requestSwitchTalkNpcEventVolleyBall(this, 1);

        if (mShineClear != nullptr)
            rs::appearPopupShine(mShineClear);
    }

    if (mCurrentScore == mSuperClearCount) {
        al::tryOnStageSwitch(this, "SwitchSuperClearOn");
        rs::requestSwitchTalkNpcEventVolleyBall(this, 2);
        if (mShineSuperClear != nullptr)
            rs::appearPopupShine(mShineSuperClear);
        al::startHitReaction(this, "2個目の目標達成");
    }

    if (199 < mCurrentScore && mCurrentScore % 100 == 0)
        al::startHitReaction(this, "超クリア");
}

bool VolleyballNpc::isEnableAppearLayout() const {
    return al::isInAreaObjPlayerAnyOne(this, mLayoutArea);
}

bool VolleyballNpc::isEnterGameArea() const {
    return al::isInAreaObjPlayerAnyOne(this, mGameArea);
}

bool VolleyballNpc::isEndMiss() const {
    return al::isNerve(this, &NrvVolleyballNpc.Miss);
}

bool VolleyballNpc::isInGame() const {
    return !al::isNerve(this, &NrvVolleyballNpc.Wait) && !isEndMiss() &&
           !al::isNerve(this, &NrvVolleyballNpc.Scare) &&
           !al::isNerve(this, &NrvVolleyballNpc.WaitRetry);
}

void VolleyballNpc::startSmashReaction() {
    mBase->startReaction();
}

void VolleyballNpc::exeWait() {
    if (al::isFirstStep(this)) {
        for (s32 i = 0; i < mBallGroup->getActorCount(); i++)
            mBallGroup->getDeriveActor(i)->reset();
        rs::startEventFlow(mEventFlowExecutor, "Talk");
        al::tryStartActionIfNotPlaying(this, "VolleyballWait");
    }

    if (mLayout->isWaiting())
        al::validateClipping(this);

    if (!rs::isPlayerHack(this) && isEnterGameArea()) {
        if (_1a0)
            al::setNerve(this, &NrvVolleyballNpc.CapMessageWaitForThrough);
        else
            al::setNerve(this, &NrvVolleyballNpc.LetsPlay);
        return;
    }

    if (mNpcEventStateScare->tryStart(mEventFlowExecutor)) {
        al::setNerve(this, &NrvVolleyballNpc.Scare);
        return;
    }
    rs::updateEventFlow(mEventFlowExecutor);
}

void VolleyballNpc::exeCapMessageWaitForThrough() {
    if (al::isFirstStep(this))
        al::tryOnStageSwitch(this, "SwitchInGameOnOff");

    if (!al::isGreaterEqualStep(this, 30)) {
        if (!isEnterGameArea())
            al::setNerve(this, &NrvVolleyballNpc.Wait);
        return;
    }
    al::setNerve(this, &NrvVolleyballNpc.CapMessage);
}

void VolleyballNpc::exeCapMessage() {
    if (al::isFirstStep(this))
        rs::showCapMessage(this, "VolleyballStart", 70, 0);

    if (al::isGreaterEqualStep(this, 110) && !rs::isActiveCapMessage(this))
        al::setNerve(this, &NrvVolleyballNpc.LetsPlay);
}

void VolleyballNpc::exeLetsPlay() {
    if (al::isFirstStep(this)) {
        rs::startEventFlow(mEventFlowExecutor, "GameStart");
        al::tryOnStageSwitch(this, "SwitchInGameOnOff");
    }

    if (al::isGreaterEqualStep(this, 60)) {
        _1a0 = false;
        startGame(false);
        return;
    }

    rs::updateEventFlow(mEventFlowExecutor);
}

void VolleyballNpc::exeWaitAfterGame() {
    if (al::isGreaterEqualStep(this, 60) || isEnterGameArea())
        al::setNerve(this, &NrvVolleyballNpc.Wait);
}

void VolleyballNpc::exeScare() {
    if (al::isFirstStep(this))
        rs::startEventFlow(mEventFlowExecutor, "Scare");

    rs::updateEventFlow(mEventFlowExecutor);

    if (al::updateNerveState(this))
        al::setNerve(this, &NrvVolleyballNpc.Wait);
}

void VolleyballNpc::exeWaitAttackSign() {
    rs::updateEventFlow(mEventFlowExecutor);

    if (al::isGreaterEqualStep(this, 45))
        al::setNerve(this, &AttackSign);
}

void VolleyballNpc::exeAttackSign() {
    if (al::isFirstStep(this)) {
        mSpeed = mInitSpeed;
        al::startAction(this, "VolleyballAttackSign");
        mBall->toss(al::getTrans(this) + gVector, al::getTrans(this) + gVector);
        al::startMtpAnimAndSetFrameAndStop(mBall, "Color", _174 - 1);
    }

    if (mBall->isTossEnd())
        al::setNerve(this, &AttackWithNewBall);
    else
        tryChangeMiss();
}

bool VolleyballNpc::tryChangeMiss() {
    for (s32 i = 0; i < mBallGroup->getActorCount(); i++) {
        if (!al::isDead(mBallGroup->getDeriveActor(i)) && mBallGroup->getDeriveActor(i)->isMiss()) {
            if (mLayout->isInGame())
                al::startHitReaction(this, "終了");
            al::setNerve(this, &NrvVolleyballNpc.Miss);
            return true;
        }
    }
    return false;
}

void VolleyballNpc::exeAttack() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "VolleyballAttack");
        sead::Vector3f targetPos = sead::Vector3f::zero;
        calcTargetTrans(&targetPos);

        if (0 < mCurrentScore) {
            if (mCurrentScore == (mCurrentScore / mLevelUpCount) * mLevelUpCount) {
                f32 prevSpeed = mSpeed;
                mSpeed = sead::Mathf::min(mMaxSpeed, mSpeed + mLevelUpSpeed);
                if (!al::isNear(prevSpeed, mSpeed, 0.001f)) {
                    al::startHitReaction(this, "スピードアップ");
                    rs::startEventFlow(mEventFlowExecutor, "SpeedUp");
                    _19c = 60;
                }
            }
        }

        mBall->attack(al::getTrans(this) + gVector, targetPos, mSpeed);
    }

    if (isActionEnd(this)) {
        al::setNerve(this, &NrvVolleyballNpc.AttackWait);
        return;
    }

    if (tryChangeMiss() || tryChangeAttack())
        return;

    tryChangeAttackSign();
    return;
}

void VolleyballNpc::calcTargetTrans(sead::Vector3f* out) const {
    sead::Vector3f randomTrans = sead::Vector3f::zero;
    randomTrans.x = al::getRandom(-500.0f, 500.0f);
    randomTrans.z = al::getRandom(-500.0f, 500.0f);

    al::AreaObj* obj = mBallArea ?: mGameArea;
    obj->getAreaShape()->calcWorldPos(out, randomTrans);
    out->y += 500.0f;

    al::Triangle triangle;

    alCollisionUtil::getFirstPolyOnArrow(this, out, &triangle, *out, -500.0f * sead::Vector3f::ey,
                                         nullptr, nullptr);
    out->y += 40.0f;
}

bool VolleyballNpc::tryChangeAttack() {
    al::DeriveActorGroup<VolleyballBall>* ballGroup = mBallGroup;
    for (s32 i = 0; i < ballGroup->getActorCount(); i++) {
        bool isEnd = ballGroup->getDeriveActor(i)->isReturnEnd();
        ballGroup = mBallGroup;
        if (isEnd) {
            mBall = ballGroup->getDeriveActor(i);
            al::setNerve(this, &NrvVolleyballNpc.Attack);
            return true;
        }
    }
    return false;
}

bool VolleyballNpc::tryChangeAttackSign() {
    if (_174 >= mBallNumMax || mCurrentScore < mLevelUpBallCount * _174)
        return false;

    al::DeriveActorGroup<VolleyballBall>* ballGroup = mBallGroup;
    VolleyballBall* ball = nullptr;
    for (s32 i = 0; i < ballGroup->getActorCount(); i++) {
        if (!ballGroup->getDeriveActor(i)->isActive()) {
            ball = ballGroup->getDeriveActor(i);
            break;
        }
    }
    mBall = ball;
    _174++;
    al::setNerve(this, &NrvVolleyballNpc.WaitAttackSign);
    return true;
}

void VolleyballNpc::exeAttackWait() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "VolleyballWait");

    if (al::isGreaterEqualStep(this, 30) && isEnterGameArea())
        mLayout->start();

    if (tryChangeAttack())
        return;

    if (!tryChangeAttackSign())
        tryChangeMiss();
}

void VolleyballNpc::exeMiss() {
    if (!isEnterGameArea()) {
        al::tryOffStageSwitch(this, "SwitchInGameOnOff");
        mLayout->end();
        rs::resetEventBalloonFilter(this);
        al::endBgmSituation(this, "Volleyball", false);

        if (mCurrentScore > mBestScore) {
            mBestScore = mCurrentScore;
            rs::setVolleyballBestCount(this, mCurrentScore);
        }

        rs::setVolleyballUpdateScoreFlag(this);
        al::setNerve(this, &NrvVolleyballNpc.WaitAfterGame);
        return;
    }

    if (al::isFirstStep(this)) {
        mLayout->result();
        if (mCurrentScore > mDayScore) {
            mDayScore = mCurrentScore;
            mLayout->setBestToday(mDayScore);
            rs::setVolleyballDayCount(this, mDayScore);
        }

        if (mCurrentScore > mBestScore) {
            mBestScore = mCurrentScore;
            mLayout->setBest(mCurrentScore);
            rs::setVolleyballBestCount(this, mBestScore);
        }

        rs::setVolleyballUpdateScoreFlag(this);
        if (mCurrentScore >= mSuperClearCount) {
            rs::startEventFlow(mEventFlowExecutor, "SuperClear");
            al::startAction(this, "VolleyballClappingHands");
        } else if (mCurrentScore >= mClearCount) {
            rs::startEventFlow(mEventFlowExecutor, "Clear");
            al::startAction(this, "VolleyballClappingHands");
        } else {
            rs::startEventFlow(mEventFlowExecutor, "Miss");
            al::startAction(this, "VolleyballAngry");
        }
        mCurrentScore = 0;
        endGame();
    }
    rs::updateEventFlow(mEventFlowExecutor);

    for (s32 i = 0; i < mBallGroup->getActorCount(); i++) {
        if (mBallGroup->getDeriveActor(i)->isRetry()) {
            mLayout->start();
            al::setNerve(this, &NrvVolleyballNpc.WaitRetry);
            return;
        }
    }
}

void VolleyballNpc::exeWaitRetry() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(this, "VolleyballWait");
        al::tryOnStageSwitch(this, "SwitchInGameOnOff");
    }

    if (al::isStep(this, 60))
        rs::startEventFlow(mEventFlowExecutor, "GameStart");

    if (al::isGreaterEqualStep(this, 60))
        rs::updateEventFlow(mEventFlowExecutor);

    if (al::isLessEqualStep(this, 60))
        return;

    for (s32 i = 0; i < mBallGroup->getActorCount(); i++)
        if (!mBallGroup->getDeriveActor(i)->isRetryEnd())
            return;
    startGame(true);
    al::setNerve(this, &NrvVolleyballNpc.Attack);
}

void VolleyballNpc::exeReactionCap() {
    if (al::isFirstStep(this))
        al::startAction(this, "ReactionCap");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvVolleyballNpc.Wait);
}

void VolleyballNpc::exeReactionTrample() {
    if (al::isFirstStep(this))
        al::startAction(this, "VolleyballReaction");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvVolleyballNpc.Wait);
}
