#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
class CameraTicket;
}  // namespace al
class IUsePlayerCollision;
class PlayerInput;

class PlayerJudgeCameraSubjective : public al::HioNode, public IJudge {
public:
    PlayerJudgeCameraSubjective(const al::LiveActor* player, const IUsePlayerCollision* collider,
                                const PlayerInput* input);

    bool isEnableKeepSubjectiveCamera() const;

    bool judge() const override;

    void reset() override {}

    void update() override {}

private:
    const al::LiveActor* mPlayer = nullptr;
    const IUsePlayerCollision* mCollider = nullptr;
    const PlayerInput* mInput = nullptr;
    al::CameraTicket* mTicket = nullptr;
};

static_assert(sizeof(PlayerJudgeCameraSubjective) == 0x28);
