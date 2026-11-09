#include "Npc/HomeCap.h"

#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Npc/HomeCapStateMoveChair.h"
#include "Npc/HomeCapStateMoveSouvenir.h"
#include "Util/CapManHeroDemoUtil.h"

namespace {
NERVE_IMPL(HomeCap, Hide);
NERVE_IMPL(HomeCap, MoveOtherChair);
NERVE_IMPL(HomeCap, MoveSouvenir);
NERVE_IMPL(HomeCap, WanderSign);
NERVE_IMPL(HomeCap, NoMoveSouvenirSleepWait);

NERVES_MAKE_STRUCT(HomeCap, Hide, MoveOtherChair, MoveSouvenir, WanderSign,
                   NoMoveSouvenirSleepWait);

NERVE_IMPL(HomeCap, WaitOtherChair);
NERVE_IMPL(HomeCap, SleepWait);
NERVE_IMPL(HomeCap, SleepStart);
NERVE_IMPL(HomeCap, Sleep);
NERVE_IMPL(HomeCap, SleepEnd);

NERVES_MAKE_NOSTRUCT(HomeCap, WaitOtherChair, SleepWait, SleepStart, Sleep, SleepEnd);

const sead::Vector3f sAimChairOffset = {0.0f, 100.0f, 30.0f};
}  // namespace

namespace HomeCapFunction {
const sead::Vector3f* getAimChairOffset() {
    return &sAimChairOffset;
}
}  // namespace HomeCapFunction

HomeCap::HomeCap(const char* name, HomeChair* chair,
                 const sead::PtrArray<HomeCapMovePoint>& points)
    : al::LiveActor(name), mChair(chair), mMovePoints(points) {}

void HomeCap::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "CapManHero", "Home");
    al::initNerve(this, &NrvHomeCap.Hide, 2);
    mStateMoveChair = new HomeCapStateMoveChair(this);
    al::initNerveState(this, mStateMoveChair, &NrvHomeCap.MoveOtherChair, "椅子に移動");
    mStateMoveSouvenir = new HomeCapStateMoveSouvenir(this, mMovePoints);
    al::initNerveState(this, mStateMoveSouvenir, &NrvHomeCap.MoveSouvenir, "お土産移動");
    al::initJointControllerKeeper(this, 1);
    CapManHeroDemoUtil::initCapManHeroTailJointController(this);
    makeActorDead();
}

void HomeCap::appearMoveOtherChair(al::LiveActor* actor) {
    al::LiveActor::appear();
    CapManHeroDemoUtil::startCapManHeroCommonSettingAfterShowModel(this);
    mStateMoveChair->appearMoveOtherChair(actor);
    mStateMoveSouvenir->setReturnChair(actor);
    al::setNerve(this, &NrvHomeCap.MoveOtherChair);
}

void HomeCap::tryKillReturnHead() {
    if (al::isAlive(this)) {
        al::startHitReaction(this, "消滅");
        kill();
    }
}

void HomeCap::exeHide() {}

void HomeCap::exeMoveOtherChair() {
    al::updateNerveStateAndNextNerve(this, &WaitOtherChair);
}

void HomeCap::exeWaitOtherChair() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HomeWait");
    }
    if (al::isGreaterEqualStep(this, 0x2c6)) {
        bool isMoveSouvenir = mStateMoveSouvenir->tryMoveSouvenir();
        if (isMoveSouvenir) {
            al::setNerve(this, &NrvHomeCap.WanderSign);
        } else {
            al::setNerve(this, &NrvHomeCap.NoMoveSouvenirSleepWait);
        }
    }
}

void HomeCap::exeWanderSign() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "SouvenirMoveSign");
    }
    if (al::isActionEnd(this)) {
        al::setNerve(this, &NrvHomeCap.MoveSouvenir);
    }
}

void HomeCap::exeMoveSouvenir() {
    if (al::updateNerveState(this)) {
        al::setNerve(this, &SleepWait);
    }
}

void HomeCap::exeSleepWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HomeWait");
    }
    if (al::isGreaterEqualStep(this, 0x258)) {
        al::setNerve(this, &SleepStart);
    }
}

void HomeCap::exeNoMoveSouvenirSleepWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HomeWait");
    }
    if (al::isGreaterEqualStep(this, 0x960)) {
        al::setNerve(this, &SleepStart);
    }
}

void HomeCap::exeSleepStart() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HomeSleepStart");
    }
    if (al::isActionEnd(this)) {
        al::setNerve(this, &Sleep);
    }
}

void HomeCap::exeSleep() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HomeSleep");
    }
    if (al::isGreaterEqualStep(this, 0x258)) {
        al::setNerve(this, &SleepEnd);
    }
}

void HomeCap::exeSleepEnd() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HomeSleepEnd");
    }
    if (al::isActionEnd(this)) {
        al::setNerve(this, &WaitOtherChair);
    }
}
