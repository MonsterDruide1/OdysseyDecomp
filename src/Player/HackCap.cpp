#include "Player/HackCap.h"

#include "Player/IUsePlayerCollision.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Project/Controller/PadRumbleKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/CapTargetInfo.h"
#include "Player/HackCapThrowParam.h"
#include "Player/PlayerAreaChecker.h"
#include "Player/PlayerCapActionHistory.h"
#include "Player/PlayerExternalVelocity.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerJointControlKeeper.h"
#include "Player/PlayerPushReceiver.h"
#include "Player/PlayerWallActionHistory.h"

namespace {
NERVE_IMPL(HackCap, Catch);
NERVE_IMPL(HackCap, ThrowStay);
NERVE_IMPL(HackCap, Hide);
NERVE_IMPL(HackCap, Return);
NERVE_IMPL(HackCap, LockOn);
NERVE_IMPL(HackCap, Rebound);
NERVE_IMPL(HackCap, Hack);
NERVE_IMPL(HackCap, SpinAttack);
NERVE_IMPL(HackCap, ThrowStart);
NERVE_IMPL(HackCap, Rescue);
NERVE_IMPL_(HackCap, TrampleReturn, Trample);
NERVE_IMPL(HackCap, ThrowTornado);
NERVE_IMPL(HackCap, ThrowSpiral);
NERVE_IMPL(HackCap, ThrowRolling);
NERVE_IMPL(HackCap, Throw);
NERVE_IMPL(HackCap, ThrowBrake);
NERVE_IMPL_(HackCap, Rethrow, Throw);
NERVE_IMPL(HackCap, ThrowAppend);
NERVE_IMPL(HackCap, ThrowRollingBrake);
NERVE_IMPL(HackCap, Trample);
NERVE_IMPL(HackCap, TrampleLockOn);
NERVE_IMPL(HackCap, Blow);

NERVES_MAKE_STRUCT(HackCap, Catch, ThrowStay, Hide, Return, LockOn, Rebound, Hack, SpinAttack,
                   ThrowStart, Rescue, TrampleReturn, ThrowTornado, ThrowSpiral, ThrowRolling,
                   Throw, ThrowBrake, Rethrow, ThrowAppend, ThrowRollingBrake, Trample,
                   TrampleLockOn, Blow);
}  // namespace

void HackCap::syncHackDamageVisibility(bool visible) {
    mIsHackDamageVisible = visible;
}


bool HackCap::isSpinAttack() const {
    return al::isNerve(this, &NrvHackCap.SpinAttack);
}

void HackCap::recordCapJump(PlayerWallActionHistory* history) {
    // NON_MATCHING
}

void HackCap::startPuppet() {
    mPuppetFlags = 1;
    mIsHidePuppetCapSilhouette = false;
}

void HackCap::hidePuppetCapSilhouette() {
    mIsHidePuppetCapSilhouette = true;
}

void HackCap::showPuppetCapSilhouette() {
    mIsHidePuppetCapSilhouette = false;
}

void HackCap::endHackShineGetDemo() {
    // NON_MATCHING
}

s32 HackCap::getPadRumblePort() const {
    return mPadRumbleKeeper->getPort();
}

bool HackCap::isHackInvalidSeparatePlay() const {
    return mCapTargetInfo1 && mCapTargetInfo1->_7e;
}

void HackCap::endHackThrow() {
    al::setNerve(this, &NrvHackCap.Catch);
}
