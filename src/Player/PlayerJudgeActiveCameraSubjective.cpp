#include "Player/PlayerJudgeActiveCameraSubjective.h"

#include "Player/PlayerStateCameraSubjective.h"

PlayerJudgeActiveCameraSubjective::PlayerJudgeActiveCameraSubjective(
    const PlayerStateCameraSubjective* state)
    : mState(state) {}

bool PlayerJudgeActiveCameraSubjective::judge() const {
    return mState->isStateActiveCamera();
}
