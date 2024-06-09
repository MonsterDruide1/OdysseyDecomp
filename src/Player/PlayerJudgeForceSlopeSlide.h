#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class PlayerConst;

class PlayerJudgeForceSlopeSlide : public al::HioNode, public IJudge {
public:
    PlayerJudgeForceSlopeSlide(const al::LiveActor* player, const PlayerConst* pConst,
                               const IUsePlayerCollision* collider);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollider;
    bool mIsJudge = false;
};
