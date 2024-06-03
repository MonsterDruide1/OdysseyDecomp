#include "Player/PlayerJumpMessageRequest.h"

PlayerJumpMessageRequest::PlayerJumpMessageRequest() {}

void PlayerJumpMessageRequest::clear() {
    mJumpType = PlayerJumpType::Standard;
    mJumpPower = 0.0f;
    mExtendFrame = 0;
    mTurnJumpAngle = {0.0f, 0.0f, 0.0f};
    mActorTrans = {0.0f, 0.0f, 0.0f};
    mIsSpinClockwise = false;
    mIsEnableStandUp = false;
}
