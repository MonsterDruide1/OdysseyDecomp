#pragma once

#include <basis/seadTypes.h>

class HackCap;

class HackCapJudgePreInputSeparateThrow {
public:
    HackCapJudgePreInputSeparateThrow(HackCap* hackCap);

    void update();
    bool judge() const;
    bool isEnablePreInput() const;

private:
    HackCap* mHackCap;
    s32 mPreInputFrames;
    bool mIsPreInput;
};
