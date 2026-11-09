#include "Boss/Koopa/Koopa.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"
#include "Npc/PeachOnKoopaShoulder.h"
#include "Util/PlayerHackInputFunction.h"

namespace {
NERVE_IMPL(Koopa, Swoon);
NERVE_IMPL(Koopa, Start);
NERVE_IMPL(Koopa, DemoHackFirst);
NERVE_END_IMPL(Koopa, 2D);
NERVE_IMPL(Koopa, Move);
NERVE_IMPL(Koopa, Jump);
NERVE_IMPL(Koopa, Fall);
NERVE_IMPL(Koopa, JumpEnd);
NERVE_IMPL(Koopa, JumpStart);
NERVE_IMPL(Koopa, Bind);
NERVE_IMPL(Koopa, SwoonReaction);
NERVE_IMPL(Koopa, BindHackStart);
NERVE_IMPL(Koopa, DeadFall);
NERVE_IMPL(Koopa, Dead);
NERVE_IMPL(Koopa, TurnPoint);
NERVE_IMPL(Koopa, Stop);
NERVE_IMPL(Koopa, MoveBrake);
NERVE_END_IMPL(Koopa, Wait);
NERVE_IMPL_(Koopa, MoveBrakeTurnR, MoveBrakeTurn);
NERVE_IMPL_(Koopa, MoveBrakeTurnL, MoveBrakeTurn);

// Note: the struct is written out by hand instead of NERVES_MAKE_STRUCT because
// NERVE_MAKE would declare a member named `2D`, which is not a valid identifier.
struct {
    KoopaNrvSwoon Swoon;
    KoopaNrvStart Start;
    KoopaNrvDemoHackFirst DemoHackFirst;
    KoopaNrv2D _2D;
    KoopaNrvMove Move;
    KoopaNrvJump Jump;
    KoopaNrvFall Fall;
    KoopaNrvJumpEnd JumpEnd;
    KoopaNrvJumpStart JumpStart;
    KoopaNrvBind Bind;
    KoopaNrvSwoonReaction SwoonReaction;
    KoopaNrvBindHackStart BindHackStart;
    KoopaNrvDeadFall DeadFall;
    KoopaNrvDead Dead;
    KoopaNrvTurnPoint TurnPoint;
    KoopaNrvStop Stop;
    KoopaNrvMoveBrake MoveBrake;
    KoopaNrvWait Wait;
    KoopaNrvMoveBrakeTurnR MoveBrakeTurnR;
    KoopaNrvMoveBrakeTurnL MoveBrakeTurnL;
} NrvKoopa;
}  // namespace

bool Koopa::isTriggerSwing() const {
    return rs::isTriggerHackSwing(mPlayerHack);
}

bool Koopa::isBindWait() const {
    return al::isNerve(this, &NrvKoopa.Bind);
}

void Koopa::exeSwoon() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Swoon");
        al::setVelocityZero(this);
    }
}

void Koopa::exeSwoonReaction() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "SwoonReaction");
    }
    al::setNerveAtActionEnd(this, &NrvKoopa.Swoon);
}

void Koopa::exeDeadFall() {
    f32 t = al::normalize(mMoveCounter->_8, 0, mMoveCounter->_4 * 10);
    f32 eased = al::easeOut(t);
    al::addVelocityToGravity(this, al::lerpValue(1.0f, 4.0f, eased));
    al::scaleVelocityHV(this, 0.0f, 0.975f);
}

void Koopa::end2D() {
    al::startSe(this, "Out2DArea");
    alSeFunction::endSituation(this, "2D3D", -1);
    alSeFunction::startSituation(this, "2D3Dの外", -1);
    al::endBgmSituation(this, "In2DArea", true);
}

void Koopa::endWait() {
    if (mPeachOnKoopaShoulder) {
        mPeachOnKoopaShoulder->invalidateAppearTiaraEyes();
    }
}
