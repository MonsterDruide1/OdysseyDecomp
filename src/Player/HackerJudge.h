#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}

class IUsePlayerCollision;
class IUsePlayerHack;

class HackerJudge : public al::HioNode, public IJudge {
public:
    HackerJudge(IUsePlayerHack** parent) { mHacker = parent; };

    void reset() override;
    void update() override;
    bool judge() const override;

    IUsePlayerHack** getHacker() const { return mHacker; };

private:
    IUsePlayerHack** mHacker;
};

class HackerJudgeNormalFall : public HackerJudge {
public:
    HackerJudgeNormalFall(const al::LiveActor* parent, s32 unk);
    void reset() override;
    void update() override;
    bool judge() const override;

    void setPlayerCollision(IUsePlayerCollision* playerCollision) {
        mPlayerCollision = playerCollision;
    }

private:
    s32 field_10;
    s32 field_14;
    IUsePlayerCollision* mPlayerCollision;
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

    void setPlayerCollision(IUsePlayerCollision* playerCollision) {
        mPlayerCollision = playerCollision;
    }

private:
    al::LiveActor* mParent;
    s32 field_18;
    IUsePlayerCollision* mPlayerCollision;
    f32 field_28;
};
