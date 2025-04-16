#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class AnagramAlphabet;
class CapTargetInfo;
class CapTargetParts;
class HackerJudgeNormalFall;
class HackerJudgeStartRun;
class IUsePlayerHack;
class PlayerHackStartShaderCtrl;

class AnagramAlphabetCharacter : public al::LiveActor {
public:
    AnagramAlphabetCharacter(const char* name);

    void init(const al::ActorInitInfo& info);
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
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

    inline bool isHack();
    inline void endHack();
    inline void endHackDir(const sead::Vector3f& dir);

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
