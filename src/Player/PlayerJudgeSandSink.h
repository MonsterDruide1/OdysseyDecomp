#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

class IUsePlayerCollision;
class PlayerSandSinkAffect;

class PlayerJudgeSandSink : public al::HioNode, public IJudge {
public:
    PlayerJudgeSandSink(const IUsePlayerCollision* collider,
                        const PlayerSandSinkAffect* sandSinkAffect);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const IUsePlayerCollision* mCollider;
    const PlayerSandSinkAffect* mSandSinkAffect;
};

static_assert(sizeof(PlayerJudgeSandSink) == 0x18);
