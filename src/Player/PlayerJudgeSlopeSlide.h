#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class IPlayerModelChanger;

class PlayerJudgeSlopeSlide : public IJudge {
public:
    PlayerJudgeSlopeSlide(const al::LiveActor* player, const PlayerConst* pConst,
                          const PlayerInput* input, const IUsePlayerCollision* collider,
                          const IPlayerModelChanger* modelChanger);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollider;
    const IPlayerModelChanger* mModelChanger;
    bool mIsJudge = false;
};
