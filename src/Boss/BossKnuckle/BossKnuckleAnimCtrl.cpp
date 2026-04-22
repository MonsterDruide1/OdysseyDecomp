#include "Boss/BossKnuckle/BossKnuckleAnimCtrl.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/BossKnuckle/BossKnuckle.h"

namespace {
NERVE_IMPL(BossKnuckleAnimCtrl, BeforeStart)
NERVE_IMPL(BossKnuckleAnimCtrl, GraspMoveHit)
NERVES_MAKE_NOSTRUCT(BossKnuckleAnimCtrl, BeforeStart, GraspMoveHit)

NERVE_IMPL(BossKnuckleAnimCtrl, Wait)
NERVE_IMPL(BossKnuckleAnimCtrl, AttackGraspSign)
NERVE_IMPL(BossKnuckleAnimCtrl, Sand)
NERVE_IMPL(BossKnuckleAnimCtrl, NumbnessStart)
NERVE_IMPL(BossKnuckleAnimCtrl, AngryStart)
NERVE_IMPL(BossKnuckleAnimCtrl, AngryHighLoop)
NERVE_IMPL(BossKnuckleAnimCtrl, AngryHighOneTime)
NERVE_IMPL(BossKnuckleAnimCtrl, MoveFrontStart)
NERVE_IMPL(BossKnuckleAnimCtrl, MoveFront)
NERVE_IMPL(BossKnuckleAnimCtrl, MoveFrontEnd)
NERVE_IMPL(BossKnuckleAnimCtrl, FearStart)
NERVE_IMPL(BossKnuckleAnimCtrl, Numbness)
NERVE_IMPL(BossKnuckleAnimCtrl, NumbnessEnd)
NERVE_IMPL(BossKnuckleAnimCtrl, Angry)
NERVE_IMPL(BossKnuckleAnimCtrl, AngryEnd)
NERVE_IMPL(BossKnuckleAnimCtrl, Fear)
NERVE_IMPL(BossKnuckleAnimCtrl, FearEnd)
NERVE_IMPL(BossKnuckleAnimCtrl, PlayerDamage)

NERVES_MAKE_NOSTRUCT(BossKnuckleAnimCtrl, Wait, AttackGraspSign, Sand, NumbnessStart, AngryStart,
                     AngryHighLoop, AngryHighOneTime, MoveFrontStart, MoveFront, MoveFrontEnd,
                     FearStart, Numbness, NumbnessEnd, Angry, AngryEnd, Fear, FearEnd, PlayerDamage)
}  // namespace

BossKnuckleAnimCtrl::BossKnuckleAnimCtrl(BossKnuckle* bossKnuckle)
    : al::NerveExecutor("ボスナックルアニメコントロール"), mBossKnuckle(bossKnuckle) {
    initNerve(&BeforeStart, 0);
}

void BossKnuckleAnimCtrl::update() {
    updateNerve();
}

void BossKnuckleAnimCtrl::startWait() {
    al::setNerve(this, &Wait);
}

void BossKnuckleAnimCtrl::startAttackGraspSign() {
    if (isPlayerDamage())
        return;
    if (isNumbness())
        return;

    al::setNerve(this, &AttackGraspSign);
}

void BossKnuckleAnimCtrl::startSand() {
    if (isPlayerDamage())
        return;
    if (isNumbness())
        return;

    al::setNerve(this, &Sand);
}

void BossKnuckleAnimCtrl::startGraspMoveHit() {
    al::setNerve(this, &GraspMoveHit);
}

void BossKnuckleAnimCtrl::startNumbnessIfNotPlaying() {
    if (isPlayerDamage())
        return;
    if (isNumbness())
        return;

    al::setNerve(this, &NumbnessStart);
}

void BossKnuckleAnimCtrl::startAngryIfNotPlaying() {
    if (isAngry())
        return;

    al::setNerve(this, &AngryStart);
}

void BossKnuckleAnimCtrl::startAngryHighLoopIfNotPlaying() {
    if (al::isNerve(this, &AngryHighLoop))
        return;

    al::setNerve(this, &AngryHighLoop);
}

void BossKnuckleAnimCtrl::startAngryHighOneTimeIfNotPlaying() {
    if (al::isNerve(this, &AngryHighOneTime))
        return;

    al::setNerve(this, &AngryHighOneTime);
}

void BossKnuckleAnimCtrl::startMoveFrontIfNotPlaying() {
    if (al::isNerve(this, &MoveFrontStart))
        return;
    if (al::isNerve(this, &MoveFront))
        return;
    if (al::isNerve(this, &MoveFrontEnd))
        return;

    al::setNerve(this, &MoveFrontStart);
}

void BossKnuckleAnimCtrl::startFearIfNotPlaying() {
    if (isFear())
        return;

    al::setNerve(this, &FearStart);
}

void BossKnuckleAnimCtrl::endMoveFrontIfPlaying() {
    if (!al::isNerve(this, &MoveFront))
        return;

    al::setNerve(this, &MoveFrontEnd);
}

void BossKnuckleAnimCtrl::exeNumbnessStart() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "AttackGraspNumbnessStart");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &Numbness);
}

void BossKnuckleAnimCtrl::endNumbnessIfPlaying() {
    if (!al::isNerve(this, &Numbness))
        return;

    al::setNerve(this, &NumbnessEnd);
}

void BossKnuckleAnimCtrl::exeAngryStart() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "AngryStart");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &Angry);
}

void BossKnuckleAnimCtrl::endAngryIfPlaying() {
    if (al::isNerve(this, &Angry)) {
        al::setNerve(this, &AngryEnd);
        return;
    }

    if (al::isNerve(this, &AngryHighLoop)) {
        al::setNerve(this, &AngryEnd);
        return;
    }

    if (!al::isNerve(this, &AngryHighOneTime))
        return;

    al::setNerve(this, &AngryEnd);
}

void BossKnuckleAnimCtrl::exeFearStart() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "FearStart");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &Fear);
}

void BossKnuckleAnimCtrl::endFearIfPlaying() {
    if (!al::isNerve(this, &Fear))
        return;

    al::setNerve(this, &FearEnd);
}

void BossKnuckleAnimCtrl::startPlayerDamage() {
    if (al::isNerve(this, &PlayerDamage))
        return;

    if (al::isNerve(this, &AngryStart) || al::isNerve(this, &Angry) ||
        al::isNerve(this, &AngryEnd) || al::isNerve(this, &AngryHighLoop) ||
        al::isNerve(this, &AngryHighOneTime)) {
        mPlayerDamageReturnNerve = &Angry;
    } else if (al::isNerve(this, &NumbnessStart) || al::isNerve(this, &Numbness) ||
               al::isNerve(this, &NumbnessEnd)) {
        mPlayerDamageReturnNerve = &Numbness;
    } else {
        if (!al::isNerve(this, &FearStart) && !al::isNerve(this, &Fear) &&
            !al::isNerve(this, &FearEnd)) {
            mPlayerDamageReturnNerve = &Wait;
            al::setNerve(this, &PlayerDamage);
            return;
        }

        mPlayerDamageReturnNerve = &Fear;
    }

    al::setNerve(this, &PlayerDamage);
}

bool BossKnuckleAnimCtrl::isPlayerDamage() {
    return al::isNerve(this, &PlayerDamage);
}

bool BossKnuckleAnimCtrl::isNumbness() {
    return al::isNerve(this, &NumbnessStart) || al::isNerve(this, &Numbness) ||
           al::isNerve(this, &NumbnessEnd);
}

bool BossKnuckleAnimCtrl::isAngry() {
    return al::isNerve(this, &AngryStart) || al::isNerve(this, &Angry) ||
           al::isNerve(this, &AngryEnd) || al::isNerve(this, &AngryHighLoop) ||
           al::isNerve(this, &AngryHighOneTime);
}

bool BossKnuckleAnimCtrl::isFear() {
    return al::isNerve(this, &FearStart) || al::isNerve(this, &Fear) || al::isNerve(this, &FearEnd);
}

void BossKnuckleAnimCtrl::exeBeforeStart() {}

void BossKnuckleAnimCtrl::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(mBossKnuckle, "Wait");
        return;
    }
}

void BossKnuckleAnimCtrl::exeAttackGraspSign() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(mBossKnuckle, "AttackGraspSign");
        return;
    }
}

void BossKnuckleAnimCtrl::exeSand() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(mBossKnuckle, "AttackGraspSand");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &Wait);
}

void BossKnuckleAnimCtrl::exeNumbness() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(mBossKnuckle, "AttackGraspNumbness");
        return;
    }
}

void BossKnuckleAnimCtrl::exeNumbnessEnd() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "AttackGraspNumbnessEnd");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &Wait);
}

void BossKnuckleAnimCtrl::exeGraspMoveHit() {
    if (al::isFirstStep(this)) {
        al::startAction(mBossKnuckle, "GraspMoveHit");
        return;
    }
}

void BossKnuckleAnimCtrl::exeAngry() {
    if (al::isFirstStep(this)) {
        al::startAction(mBossKnuckle, "Angry");
        return;
    }
}

void BossKnuckleAnimCtrl::exeAngryEnd() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "AngryEnd");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &Wait);
}

void BossKnuckleAnimCtrl::exeAngryHighLoop() {
    if (al::isFirstStep(this)) {
        al::startAction(mBossKnuckle, "AngryHigh1");
        return;
    }
}

void BossKnuckleAnimCtrl::exeAngryHighOneTime() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "AngryHigh2");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &Angry);
}

void BossKnuckleAnimCtrl::exeFear() {
    if (al::isFirstStep(this)) {
        al::startAction(mBossKnuckle, "Fear");
        return;
    }
}

void BossKnuckleAnimCtrl::exeFearEnd() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "FearEnd");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &Wait);
}

void BossKnuckleAnimCtrl::exeMoveFrontStart() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "MoveFrontStart");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &MoveFront);
}

void BossKnuckleAnimCtrl::exeMoveFront() {
    if (al::isFirstStep(this)) {
        al::startAction(mBossKnuckle, "MoveFront");
        return;
    }
}

void BossKnuckleAnimCtrl::exeMoveFrontEnd() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "MoveFrontEnd");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, &Wait);
}

void BossKnuckleAnimCtrl::exePlayerDamage() {
    if (al::isFirstStep(this))
        al::startAction(mBossKnuckle, "PlayerDamage");
    if (al::isActionEnd(mBossKnuckle))
        al::setNerve(this, mPlayerDamageReturnNerve);
}

BossKnuckleAnimCtrl::~BossKnuckleAnimCtrl() = default;
