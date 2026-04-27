#pragma once

#include "Library/Nerve/NerveExecutor.h"

class BossKnuckle;

class BossKnuckleAnimCtrl : public al::NerveExecutor {
public:
    BossKnuckleAnimCtrl(BossKnuckle* bossKnuckle);

    void update();
    void startWait();
    void startAttackGraspSign();
    bool isPlayerDamage();
    bool isNumbness();
    void startSand();
    void startGraspMoveHit();
    void startNumbnessIfNotPlaying();
    void startAngryIfNotPlaying();
    bool isAngry();
    void startAngryHighLoopIfNotPlaying();
    void startAngryHighOneTimeIfNotPlaying();
    void startMoveFrontIfNotPlaying();
    void startFearIfNotPlaying();
    bool isFear();
    void endNumbnessIfPlaying();
    void endAngryIfPlaying();
    void endFearIfPlaying();
    void endMoveFrontIfPlaying();
    void startPlayerDamage();
    void exeBeforeStart();
    void exeWait();
    void exeAttackGraspSign();
    void exeSand();
    void exeNumbnessStart();
    void exeNumbness();
    void exeNumbnessEnd();
    void exeGraspMoveHit();
    void exeAngryStart();
    void exeAngry();
    void exeAngryEnd();
    void exeAngryHighLoop();
    void exeAngryHighOneTime();
    void exeFearStart();
    void exeFear();
    void exeFearEnd();
    void exeMoveFrontStart();
    void exeMoveFront();
    void exeMoveFrontEnd();
    void exePlayerDamage();
    ~BossKnuckleAnimCtrl();

private:
    BossKnuckle* mBossKnuckle = nullptr;
    const al::Nerve* mPlayerDamageReturnNerve = nullptr;
};

static_assert(sizeof(BossKnuckleAnimCtrl) == 0x20);
