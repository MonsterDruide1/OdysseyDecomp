#include "Player/PlayerJudgeFailureCameraSubjective.h"

#include "Player/PlayerInput.h"
#include "Util/JudgeUtil.h"

PlayerJudgeFailureCameraSubjective::PlayerJudgeFailureCameraSubjective(
    const PlayerInput* input, const IJudge* inCameraNerveJudge)
    : mInput(input), mInCameraNerveJudge(inCameraNerveJudge) {}

bool PlayerJudgeFailureCameraSubjective::judge() const {
    return mInput->isTriggerCameraSubjective() && !rs::isJudge(mInCameraNerveJudge);
}
