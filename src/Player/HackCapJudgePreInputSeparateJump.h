#pragma once

#include <basis/seadTypes.h>

class HackCap;

class HackCapJudgePreInputSeparateJump {
public:
    HackCapJudgePreInputSeparateJump(HackCap* hackCap);

    void update();
    bool judge() const;

private:
    HackCap* mHackCap;
    s32 mPreInputFrames;
    bool mIsPreInput;
};
