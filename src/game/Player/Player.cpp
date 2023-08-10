#include "game/Player/Player.h"
#include "al/Library/Controller/InputFunction.h"
#include "al/Library/LiveActor/ActorActionFunction.h"
#include "al/Library/LiveActor/ActorClippingFunction.h"
#include "al/Library/LiveActor/ActorInitInfo.h"
#include "al/Library/LiveActor/ActorMovementFunction.h"
#include "al/Library/LiveActor/ActorPoseKeeper.h"
#include "al/Library/Math/MathUtil.h"
#include "al/Library/Nerve/NerveSetupUtil.h"
#include "al/Library/Nerve/NerveUtil.h"

namespace {
MAKE_NERVE(Player, Wait);
MAKE_NERVE(Player, Run);
MAKE_NERVE(Player, Jump);
MAKE_NERVE(Player, Fall);
MAKE_NERVE(Player, Damage);
}  // namespace

Player::Player(const char* actorName, const char* archiveName, s32 port) : al::LiveActor(actorName), mArchiveName(archiveName), mPort(port) {}
void Player::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, al::LiveActor::getName(), nullptr);
    al::invalidateClipping(this);
}
void Player::control() {
    if (al::isPadTriggerL(mPort)) {
        al::setVelocityZero(this);
        al::setTrans(this, {100.0f, 0.0f, 800.0f});
        al::resetPosition(this);
        al::setNerve(this, &PlayerNrvFall::sInstance);
    }
}
void Player::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    al::scaleVelocity(this, 0.7f);

    if (al::isPadTriggerA(mPort))
        al::setNerve(this, &PlayerNrvJump::sInstance);
    else if (al::isNearZero(al::getLeftStick(mPort), 0.001f))
        al::setNerve(this, &PlayerNrvRun::sInstance);
}
void Player::exeRun() {}
