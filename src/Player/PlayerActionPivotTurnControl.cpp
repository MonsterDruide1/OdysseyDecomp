#include "Player/PlayerActionPivotTurnControl.h"

PlayerActionPivotTurnControl::PlayerActionPivotTurnControl(al::LiveActor* player,
                                                           PlayerConst const* pConst,
                                                           PlayerInput const* input,
                                                           IUsePlayerCollision const* collider,
                                                           float gravity)
    : mPlayer(player), mConst(pConst), mInput(input), mCollider(collider), mGravity(gravity) {}
void PlayerActionPivotTurnControl::reset() {
    CRASH
}
void PlayerActionPivotTurnControl::update() {
    CRASH
}
void PlayerActionPivotTurnControl::calcMoveDirection(sead::Vector3<float>*,
                                                     sead::Vector3<float> const&) {
    CRASH
}
