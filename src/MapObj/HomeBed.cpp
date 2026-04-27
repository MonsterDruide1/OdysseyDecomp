#include "MapObj/HomeBed.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/ChairStateBind.h"
#include "MapObj/FurnitureStateWait.h"
#include "Util/PlayerPuppetFunction.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(HomeBed, Wait);
NERVE_IMPL(HomeBed, SleepStart);
NERVE_IMPL(HomeBed, StandUp);
NERVE_IMPL(HomeBed, SnoozeStart);
NERVE_IMPL(HomeBed, WaitNoSleep);
NERVE_IMPL(HomeBed, BindJump);
NERVE_IMPL(HomeBed, Snooze);
NERVE_IMPL(HomeBed, Sleep);

NERVES_MAKE_STRUCT(HomeBed, Wait, SleepStart, StandUp, SnoozeStart, WaitNoSleep, BindJump, Snooze,
                   Sleep);
}  // namespace

HomeBed::HomeBed(const char* name) : al::LiveActor(name) {}

void HomeBed::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &NrvHomeBed.Wait, 3);

    mStateWait = new FurnitureStateWait(this, FurnitureType::Bed);
    al::initNerveState(this, mStateWait, &NrvHomeBed.Wait, "座るまで");

    mStateBindSitDown = new ChairStateBindSitDown(this, &mPlayerPuppet, "BedSleepStart",
                                                  "BedSleepStart", "BedSleep");
    al::initNerveState(this, mStateBindSitDown, &NrvHomeBed.SleepStart, "寝るまでの状態");
    mStateBindSitDown->setEnableStartInterpolation(false);

    mStateBindStandUp = new ChairStateBindStandUp(this, &mPlayerPuppet);
    al::initNerveState(this, mStateBindStandUp, &NrvHomeBed.StandUp, "立つ");

    makeActorAlive();
}

bool HomeBed::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    IUsePlayerPuppet** playerPuppet = &mPlayerPuppet;

    if (mPlayerPuppet == nullptr) {
        if (al::isMsgBindStart(message))
            return rs::isPlayerNoInput(this);

        if (al::isMsgBindInit(message)) {
            if (!rs::isPlayerNoInput(this))
                return false;

            mPlayerPuppet = rs::startPuppet(self, other);
            mStateBindSitDown->setUseLongStartStep(true);
            al::setNerve(this, &NrvHomeBed.SnoozeStart);
            return true;
        }

        if (*playerPuppet == nullptr)
            return false;
    }

    if (rs::tryReceiveBindCancelMsgAndPuppetNull(playerPuppet, message)) {
        al::setNerve(this, &NrvHomeBed.Wait);
        return true;
    }

    return false;
}

void HomeBed::exeWait() {
    al::updateNerveState(this);

    if (mStateWait->isEnableBindRequest())
        rs::requestBindPlayer(this, al::getHitSensor(this, "Collision"));
}

void HomeBed::exeSnoozeStart() {
    if (al::isFirstStep(this))
        rs::startPuppetAction(mPlayerPuppet, "BedSnoozeStart");

    if (rs::isPuppetMoveStickOn(mPlayerPuppet)) {
        rs::endBindAndPuppetNull(&mPlayerPuppet);
        al::setNerve(this, &NrvHomeBed.WaitNoSleep);
        return;
    }

    if (rs::tryEndBindCapThrowOnChair(&mPlayerPuppet)) {
        al::setNerve(this, &NrvHomeBed.WaitNoSleep);
        return;
    }

    if (rs::tryBindJumpOnChair(mPlayerPuppet)) {
        al::setNerve(this, &NrvHomeBed.BindJump);
        return;
    }

    if (rs::isPuppetActionEnd(mPlayerPuppet))
        al::setNerve(this, &NrvHomeBed.Snooze);
}

void HomeBed::exeSnooze() {
    if (al::isFirstStep(this))
        rs::startPuppetAction(mPlayerPuppet, "BedSnooze");

    if (rs::isPuppetMoveStickOn(mPlayerPuppet)) {
        rs::endBindAndPuppetNull(&mPlayerPuppet);
        al::setNerve(this, &NrvHomeBed.WaitNoSleep);
        return;
    }

    if (rs::tryEndBindCapThrowOnChair(&mPlayerPuppet)) {
        al::setNerve(this, &NrvHomeBed.WaitNoSleep);
        return;
    }

    if (rs::tryBindJumpOnChair(mPlayerPuppet)) {
        al::setNerve(this, &NrvHomeBed.BindJump);
        return;
    }

    if (al::isGreaterEqualStep(this, 180))
        al::setNerve(this, &NrvHomeBed.SleepStart);
}

void HomeBed::exeSleepStart() {
    if (al::updateNerveState(this)) {
        s32 result = mStateBindSitDown->getResult();

        if (result != 2) {
            if (result == 1) {
                al::setNerve(this, &NrvHomeBed.WaitNoSleep);
                return;
            }

            al::setNerve(this, &NrvHomeBed.Sleep);
            return;
        }

        al::setNerve(this, &NrvHomeBed.BindJump);
    }
}

void HomeBed::exeSleep() {
    IUsePlayerPuppet** playerPuppet = &mPlayerPuppet;

    if (rs::tryEndBindCapThrowOnChair(playerPuppet))
        al::setNerve(this, &NrvHomeBed.WaitNoSleep);
    else if (rs::tryBindJumpOnChair(*playerPuppet))
        al::setNerve(this, &NrvHomeBed.BindJump);
    else if (rs::tryBindStandUpOnChair(*playerPuppet))
        al::setNerve(this, &NrvHomeBed.StandUp);
}

void HomeBed::exeStandUp() {
    if (al::updateNerveState(this)) {
        s32 result = mStateBindStandUp->getResult();

        if (result != 2) {
            if (result == 1) {
                al::setNerve(this, &NrvHomeBed.WaitNoSleep);
                return;
            }

            rs::endBindAndPuppetNull(&mPlayerPuppet);
            al::setNerve(this, &NrvHomeBed.WaitNoSleep);
            return;
        }

        al::setNerve(this, &NrvHomeBed.BindJump);
    }
}

void HomeBed::exeBindJump() {
    rs::startPuppetAction(mPlayerPuppet, "Jump");

    sead::Vector3f endTrans;
    endTrans.x = 0.0f;
    endTrans.y = 25.0f;
    endTrans.z = 0.0f;

    rs::endBindJumpAndPuppetNull(&mPlayerPuppet, endTrans, 0);
    al::setNerve(this, &NrvHomeBed.WaitNoSleep);
}

void HomeBed::exeWaitNoSleep() {
    if (al::isGreaterEqualStep(this, 120))
        al::setNerve(this, &NrvHomeBed.Wait);
}

bool HomeBed::tryEndBind() {
    return false;
}
