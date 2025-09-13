#include "Player/PlayerJumpMessageRequest.h"

PlayerJumpMessageRequest::PlayerJumpMessageRequest() = default;

void PlayerJumpMessageRequest::clear() {
    jumpType = PlayerJumpType::Standard;
    jumpPower = 0.0f;
    extendFrame = 0;
    turnJumpAngle = {0.0f, 0.0f, 0.0f};
    actorTrans = {0.0f, 0.0f, 0.0f};
    isSpinClockwise = false;
    isEnableStandUp = false;
}
