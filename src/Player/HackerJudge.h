#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

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
