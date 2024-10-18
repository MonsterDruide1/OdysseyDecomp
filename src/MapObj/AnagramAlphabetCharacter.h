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
    AnagramAlphabetCharacter(const char* name);

    void init(const al::ActorInitInfo& info);
    void attackSensor(al::HitSensor* source, al::HitSensor* target);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* target, al::HitSensor* source);
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

    inline void endHack();

private:
    CapTargetInfo* mCapTargetInfo = nullptr;
    sead::Matrix34f* mPoseMatrix = nullptr;
    AnagramAlphabet* mParent = nullptr;
    IUsePlayerHack* mHackerParent = nullptr;
    CapTargetParts* mCapTargetParts = nullptr;
    HackerJudgeNormalFall* mHackerJudgeNormalFall = nullptr;
    HackerJudgeStartRun* mHackerJudgeStartRun = nullptr;
    PlayerHackStartShaderCtrl* mPlayerHackStartShaderCtrl = nullptr;
    s32 mSwingTimer = 0;
};
