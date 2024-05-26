#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
class AreaObj;
}

class PlayerAreaChecker;
class IUsePlayerCollision;

class PlayerJudgeInvalidateInputFall : public IJudge {
public:
    PlayerJudgeInvalidateInputFall(const al::LiveActor*, const PlayerAreaChecker*,
                                   const IUsePlayerCollision*);
    void reset() override;
    void update() override;
    bool judge() const override;

public:
    const al::LiveActor* mActor;
    const PlayerAreaChecker* mAreaChecker;
    const IUsePlayerCollision* mCollision;
    al::AreaObj* mInvalidateInputFallArea = nullptr;
};
