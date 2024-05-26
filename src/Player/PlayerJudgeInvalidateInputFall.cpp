#include "Player/PlayerJudgeInvalidateInputFall.h"

#include <basis/seadTypes.h>

PlayerJudgeInvalidateInputFall::PlayerJudgeInvalidateInputFall(const al::LiveActor* actor,
                                                               const PlayerAreaChecker* areaChecker,
                                                               const IUsePlayerCollision* collision) : mActor(actor),
                                                                                                      mAreaChecker(areaChecker),
                                                                                                      mCollision(collision) {}
void PlayerJudgeInvalidateInputFall::reset() {
    CRASH
}
void PlayerJudgeInvalidateInputFall::update() {
    WARN_UNIMPL;
    return;
}
bool PlayerJudgeInvalidateInputFall::judge() const {
    return mInvalidateInputFallArea != nullptr;
}
