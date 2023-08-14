#pragma once

#include "Library/HostIO/HioNode.h"
#include "Player/IJudge.h"

class IUsePlayerHack;
namespace al {
class LiveActor;
}

class HackerJudge : public al::HioNode, public IJudge {
    IUsePlayerHack** mHacker;

public:
    HackerJudge(IUsePlayerHack** parent) { mHacker = parent; };
    void reset() override;
    void update() override;
    bool judge() const override;

    IUsePlayerHack** getHacker() const { return mHacker; };
};

class HackerJudgeNormalFall : public HackerJudge {
public:
    HackerJudgeNormalFall(const al::LiveActor* parent, s32 unk);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    s32 field_10;
    s32 field_14;
    al::LiveActor* mParent;
};

class HackerJudgeStartJump : public HackerJudge {
public:
    HackerJudgeStartJump(IUsePlayerHack**);
    void reset() override;
    void update() override;
    bool judge() const override;
};

class HackerJudgeStartRun : public HackerJudge {
public:
    HackerJudgeStartRun(const al::LiveActor*, IUsePlayerHack**);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    al::LiveActor* mParent;
    s32 field_18;
    al::LiveActor* field_20;
    f32 field_28;
};
