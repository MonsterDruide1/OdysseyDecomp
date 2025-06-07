#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

class PlayerConst;
class PlayerInput;
class IUsePlayerHeightCheck;
class IPlayerModelChanger;

class PlayerJudgeStartHipDrop : public al::HioNode, public IJudge {
public:
    PlayerJudgeStartHipDrop(const PlayerConst* pConst, const PlayerInput* input,
                            const IUsePlayerHeightCheck* heightCheck,
                            const IPlayerModelChanger* modelChanger);

    void reset() override {}

    void update() override {}

    bool judge() const override;

private:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerHeightCheck* mHeightCheck;
    const IPlayerModelChanger* mModelChanger;
};

static_assert(sizeof(PlayerJudgeStartHipDrop) == 0x28);
