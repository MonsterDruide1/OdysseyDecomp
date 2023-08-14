#pragma once

#include "Library/LiveActor/LiveActor.h"

class HackerJudgeNormalFall;
class HackerJudgeStartRun;
class CapTargetInfo;
class AnagramAlphabet;
class IUsePlayerHack;
class CapTargetParts;
class PlayerHackStartShaderCtrl;

class AnagramAlphabetCharacter : public al::LiveActor {
public:
    AnagramAlphabetCharacter(const char*);

    void init(const al::ActorInitInfo&);
    void attackSensor(al::HitSensor*, al::HitSensor*);
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    void setComplete();
    void killCapTarget();

    void exeWait();
    void exeWaitHack();
    void exeWaitHackStart();
    void exeHackStart();
    void exeHackWait();
    void exeHackMove();
    void exeHackFall();
    void exeHackEnd();
    void exeHackGoal();
    void exeSet();
    void exeComplete();

private:
    CapTargetInfo* mCapTargetInfo;
    sead::Matrix34f* unkMtx;
    AnagramAlphabet* mParent;
    IUsePlayerHack* mHackerParent;
    CapTargetParts* mCapTargetParts;
    HackerJudgeNormalFall* mHackerJudgeNormalFall;
    HackerJudgeStartRun* mHackerJudgeStartRun;
    PlayerHackStartShaderCtrl* mPlayerHackStartShaderCtrl;
    s32 mSwingTimer;
};
