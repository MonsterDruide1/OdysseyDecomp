#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

class IUsePlayerHack;

class HackerJudge : public al::HioNode, public IJudge {
public:
    HackerJudge(IUsePlayerHack** parent);

    void reset() override = 0;
    void update() override = 0;
    bool judge() const override = 0;

    IUsePlayerHack** getHacker() const { return mHacker; };

private:
    IUsePlayerHack** mHacker;
};
