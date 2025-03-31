#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
class AreaObj;
}  // namespace al

class PlayerAreaChecker;
class IUsePlayerCollision;

class PlayerJudgeInvalidateInputFall : public IJudge {
public:
    PlayerJudgeInvalidateInputFall(const al::LiveActor* actor, const PlayerAreaChecker* areaChecker,
                                   const IUsePlayerCollision* collision);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mActor = nullptr;
    const PlayerAreaChecker* mAreaChecker = nullptr;
    const IUsePlayerCollision* mCollision = nullptr;
    const al::AreaObj* mInvalidateInputFallArea = nullptr;
};
