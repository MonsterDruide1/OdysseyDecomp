#include "Player/Player.h"
#include "Library/Controller/InputFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(Player, Wait);
NERVE_IMPL(Player, Run);
NERVE_IMPL(Player, Jump);
NERVE_IMPL(Player, Fall);
NERVE_IMPL(Player, Damage);

struct {
    NERVE_MAKE(Player, Wait);
    NERVE_MAKE(Player, Run);
    NERVE_MAKE(Player, Jump);
    NERVE_MAKE(Player, Fall);
    NERVE_MAKE(Player, Damage);
} NrvPlayer;

}  // namespace

Player::Player(const char* actorName, const char* archiveName, s32 port)
    : al::LiveActor(actorName), mArchiveName(archiveName), mPort(port) {}
void Player::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, al::LiveActor::getName(), nullptr);
    al::invalidateClipping(this);
}
void Player::control() {
    if (al::isPadTriggerL(mPort)) {
        al::setVelocityZero(this);
        al::setTrans(this, {100.0f, 0.0f, 800.0f});
        al::resetPosition(this);
        al::setNerve(this, &NrvPlayer.Fall);
    }
}
void Player::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    al::scaleVelocity(this, 0.7f);

    if (al::isPadTriggerA(mPort))
        al::setNerve(this, &NrvPlayer.Jump);
    else if (al::isNearZero(al::getLeftStick(mPort), 0.001f))
        al::setNerve(this, &NrvPlayer.Run);
}

// NON_MATCHING
void Player::exeRun() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Run");
        mAirTime = 0;
    }
    al::addVelocityToGravity(this, 2.0);
    al::scaleVelocity(this, 0.7);
}
