#include "Player/PlayerJudgeCameraSubjective.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/LiveActor.h"

#include "Player/PlayerInput.h"
#include "Util/CameraUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/ScenePlayerCameraFunction.h"

PlayerJudgeCameraSubjective::PlayerJudgeCameraSubjective(const al::LiveActor* player,
                                                         const IUsePlayerCollision* collider,
                                                         const PlayerInput* input)
    : mPlayer(player), mCollider(collider), mInput(input) {
    mTicket = al::initSubjectiveCameraNoSave(player, "Subjective");

    al::validateSnapShotCameraRoll(mTicket);
    al::validateSnapShotCameraZoomFovy(mTicket);
}

bool PlayerJudgeCameraSubjective::isEnableKeepSubjectiveCamera() const {
    return PlayerCameraFunction::checkNoCollisionForPlayerSubjectiveCamera(mPlayer, mTicket, false);
}

bool PlayerJudgeCameraSubjective::judge() const {
    return !rs::isPlayer2D(mPlayer) && rs::isEnableStartSubjectiveCamera(mPlayer) &&
           rs::isCollidedGround(mCollider) && !rs::isCollisionCodeSandSink(mCollider) &&
           mInput->isTriggerCameraSubjective() &&
           PlayerCameraFunction::checkNoCollisionForPlayerSubjectiveCamera(mPlayer, mTicket, true);
}
