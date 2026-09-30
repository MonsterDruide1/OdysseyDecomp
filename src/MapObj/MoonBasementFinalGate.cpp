#include "MapObj/MoonBasementFinalGate.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/BreakModel.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "MapObj/CollisionAnimCtrl.h"
#include "Npc/PeachOnKoopaAnimRequester.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(MoonBasementFinalGate, BreakPillar)
NERVE_IMPL(MoonBasementFinalGate, Wait)
NERVE_IMPL(MoonBasementFinalGate, BreakLastAfter)
NERVE_IMPL(MoonBasementFinalGate, Break)
NERVE_IMPL(MoonBasementFinalGate, BreakLast)

NERVES_MAKE_NOSTRUCT(MoonBasementFinalGate, BreakPillar)
NERVES_MAKE_STRUCT(MoonBasementFinalGate, Wait, BreakLastAfter, Break, BreakLast)

const char* sBreakPillarActions[] = {"WaitInit", "BreakPillar1", "BreakPillar2", "BreakPillar3",
                                     "BreakPillar4"};
const char* sBreakLastAfterActions[] = {"BreakLastAfter1", "BreakLastAfter2", "BreakLastAfter3",
                                        "BreakLastAfter4", "BreakLastAfter5", "BreakLastAfter6",
                                        "BreakLastAfter7", "BreakLastAfter8", "BreakLastAfter9",
                                        "BreakLastAfter9", "BreakLastAfter9", "BreakLastAfter9"};
const char* sBreakAfterActions[] = {"BreakAfter1", "BreakAfter2", "BreakAfter3", "BreakAfter4",
                                    "BreakAfter5"};
const char* sBreakPillarAfterActions[] = {"WaitInit", "BreakPillarAfter1", "BreakPillarAfter2",
                                          "BreakPillarAfter3", "BreakPillarAfter4"};
const char* sBreakPillarReactions[] = {"WaitInit", "柱ヒット1", "柱ヒット2", "柱ヒット3",
                                       "柱ヒット4"};
const char* sBreakActions[] = {"Break1", "Break2", "Break3", "Break4", "Break5"};
const char* sBreakReactions[] = {"ひび割れ1", "ひび割れ2", "ひび割れ3", "ひび割れ4", "ひび割れ5"};
const char* sBreakLastActions[] = {"BreakLast1", "BreakLast2", "BreakLast3", "BreakLast4",
                                   "BreakLast5", "BreakLast6", "BreakLast7", "BreakLast8",
                                   "BreakLast9", "BreakLast9", "BreakLast9", "BreakLastFinal"};
const char* sBreakLastReactions[] = {"ひび割れラスト1",   "ひび割れラスト2",   "ひび割れラスト3",
                                     "ひび割れラスト4",   "ひび割れラスト5",   "ひび割れラスト6",
                                     "ひび割れラスト7",   "ひび割れラスト8",   "ひび割れラスト9",
                                     "ひび割れラスト9-2", "ひび割れラスト9-3", "とどめ開始"};
const char* sCollisionNames[] = {"BreakPillar1", "BreakPillar2", "BreakPillar3",
                                 "BreakPillar4", "Break1",       "Break2",
                                 "Break3",       "Break4",       "Break5"};
}  // namespace

MoonBasementFinalGate::MoonBasementFinalGate(const char* name) : al::LiveActor(name) {}

void MoonBasementFinalGate::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &NrvMoonBasementFinalGate.Wait, 0);

    mBreakModel = al::createBreakModel(this, info, "崩落ステージの最後の岩壊れモデル",
                                       "MoonBasementFinalGateBreak", nullptr, nullptr, "Break");

    mCollisionAnimCtrl = new CollisionAnimCtrl(this, info, sCollisionNames, 9);

    if (al::tryGetBoolArgOrFalse(info, "IsValidBreakLastObjectCamera"))
        mBreakLastCameraTicket = al::initObjectCamera(this, info, "BreakLast", nullptr);

    al::invalidateHitSensor(this, "AttackBreak");
    makeActorAlive();
}

void MoonBasementFinalGate::kill() {
    al::tryOnSwitchDeadOn(this);
    mBreakModel->appear();
    al::LiveActor::kill();
    mCollisionAnimCtrl->killAll();
}

void MoonBasementFinalGate::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorPlayerAttack(self))
        rs::sendMsgMoonBasementBreakShockwaveMeteor(other, self);
}

bool MoonBasementFinalGate::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                       al::HitSensor* self) {
    if (al::isNerve(this, &NrvMoonBasementFinalGate.BreakLastAfter))
        return false;

    if (!rs::isMsgKoopaHackPunchCollide(message) && !al::isMsgExplosionCollide(message))
        return false;

    if (mBreakPillarIndex < 4)
        return false;

    s32 breakAfterIndex = mBreakAfterIndex + 1;
    if (breakAfterIndex <= 4) {
        mBreakAfterIndex = breakAfterIndex;
        al::invalidateClipping(this);
        al::setNerve(this, &NrvMoonBasementFinalGate.Break);
        return true;
    }

    s32 breakLastIndex = mBreakLastIndex + 1;
    if (breakLastIndex <= 11) {
        mBreakLastIndex = breakLastIndex;
        al::invalidateClipping(this);
        al::setNerve(this, &NrvMoonBasementFinalGate.BreakLast);
        return true;
    }

    return false;
}

void MoonBasementFinalGate::startBreakPillar(s32 breakPillarIndex) {
    if (breakPillarIndex >= 2)
        for (s32 i = 1; i < breakPillarIndex; i++)
            mCollisionAnimCtrl->startBreak(sBreakPillarActions[i]);

    mBreakPillarIndex = breakPillarIndex;
    al::invalidateClipping(this);
    al::setNerve(this, &BreakPillar);
}

void MoonBasementFinalGate::exeWait() {
    if (!al::isFirstStep(this))
        return;

    const char* actionName = "WaitInit";
    if ((u32)mBreakLastIndex <= 11)
        actionName = sBreakLastAfterActions[mBreakLastIndex];
    else if ((u32)mBreakAfterIndex <= 4)
        actionName = sBreakAfterActions[mBreakAfterIndex];
    else if ((u32)(mBreakPillarIndex - 1) <= 3)
        actionName = sBreakPillarAfterActions[mBreakPillarIndex];

    al::startAction(this, actionName);
    al::validateClipping(this);
}

void MoonBasementFinalGate::exeBreakPillar() {
    if (al::isFirstStep(this)) {
        al::startAction(this, sBreakPillarActions[mBreakPillarIndex]);
        al::startHitReaction(this, sBreakPillarReactions[mBreakPillarIndex]);
    }

    if (al::isActionEnd(this)) {
        mCollisionAnimCtrl->startBreak(sBreakPillarActions[mBreakPillarIndex]);
        al::setNerve(this, &NrvMoonBasementFinalGate.Wait);
    }
}

void MoonBasementFinalGate::exeBreak() {
    if (al::isFirstStep(this)) {
        al::startAction(this, sBreakActions[mBreakAfterIndex]);
        al::startHitReaction(this, sBreakReactions[mBreakAfterIndex]);
    }

    if (al::isActionEnd(this)) {
        if (mBreakAfterIndex >= 4)
            al::tryOnStageSwitch(this, "SwitchBreakEndOn");

        mCollisionAnimCtrl->startBreak(sBreakActions[mBreakAfterIndex]);
        al::setNerve(this, &NrvMoonBasementFinalGate.Wait);
    }
}

void MoonBasementFinalGate::exeBreakLast() {
    s32 breakLastIndex = mBreakLastIndex;

    do {
        if (al::isFirstStep(this)) {
            al::startAction(this, sBreakLastActions[mBreakLastIndex]);
            al::startHitReaction(this, sBreakLastReactions[mBreakLastIndex]);

            if (breakLastIndex > 10 || (al::isStep(this, 2), false))
                al::validateHitSensor(this, "AttackBreak");
            else
                break;
        }

        s32 isBreakStep = al::isStep(this, 2);
        if (breakLastIndex >= 11) {
            if (isBreakStep)
                al::invalidateHitSensor(this, "AttackBreak");
        }
    } while (false);

    if (!al::isActionEnd(this))
        return;

    if (breakLastIndex >= 11) {
        al::startHitReaction(this, "とどめ終了");
        rs::requestStartPeachOnKoopaHitReactionBreakGateFinal(this);
        al::tryOnStageSwitch(this, "SwitchBreakFinalOn");

        if (mBreakLastCameraTicket != nullptr) {
            al::hideModelIfShow(this);
            al::invalidateCollisionParts(this);
            al::setNerve(this, &NrvMoonBasementFinalGate.BreakLastAfter);
            return;
        }

        kill();
        return;
    }

    al::setNerve(this, &NrvMoonBasementFinalGate.Wait);
}

void MoonBasementFinalGate::exeBreakLastAfter() {
    if (al::isFirstStep(this))
        al::startCamera(this, mBreakLastCameraTicket, -1);

    if (al::isGreaterEqualStep(this, 150)) {
        rs::requestStartPeachOnKoopaAnimSuccess(this);
        al::endCamera(this, mBreakLastCameraTicket, -1, false);
        kill();
    }
}
