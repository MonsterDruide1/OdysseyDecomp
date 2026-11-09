#include "Npc/Frog.h"

#include "Enemy/EnemyCap.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Fluid/JointRippleGenerator.h"
#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MatrixUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Npc/FrogParam.h"
#include "Player/IUsePlayerHack.h"
#include "Util/PlayerHackFunction.h"

namespace rs {
void tryRegisterSphinxQuizRouteKillSensorAfterPlacement(al::HitSensor* sensor);
}

namespace {
NERVE_HOST_TYPE_IMPL(Frog, Wait);
NERVE_HOST_TYPE_IMPL(Frog, Revive);
NERVE_HOST_TYPE_IMPL(Frog, Wander);
NERVE_HOST_TYPE_IMPL(Frog, Reset);
NERVE_HOST_TYPE_IMPL(Frog, Swoon);
NERVE_HOST_TYPE_IMPL(Frog, DemoHackFirst);
NERVE_HOST_TYPE_IMPL(Frog, Appear);
NERVE_HOST_TYPE_IMPL(Frog, WaitHackStartSwoon);
NERVE_HOST_TYPE_IMPL(Frog, WaitHackStart);
NERVE_HOST_TYPE_IMPL(Frog, Die);
NERVE_HOST_TYPE_IMPL(Frog, Trampled);
NERVE_HOST_TYPE_IMPL(Frog, DamageCap);
NERVE_HOST_TYPE_IMPL(Frog, HackStart);
NERVE_HOST_TYPE_IMPL(Frog, ReviveAppear);

class HostTypeNrvHack : public al::Nerve {
public:
    void execute(al::NerveKeeper* keeper) const override {
        keeper->getParent<Frog>()->exeHack();
    }

    void executeOnEnd(al::NerveKeeper* keeper) const override {
        keeper->getParent<Frog>()->endHack();
    }
};

NERVES_MAKE_STRUCT(HostType, Wait, Hack, Revive, Wander, Reset, Swoon, DemoHackFirst, Appear,
                   WaitHackStartSwoon, WaitHackStart, Die, Trampled, DamageCap, HackStart,
                   ReviveAppear);
}  // namespace

// Returns true when the ground found below the frog is at or above the water
// surface (i.e. the frog is not in water). Name is a guess.
bool isGroundAboveWaterSurface(const Frog* frog, const al::WaterSurfaceFinder* finder) {
    sead::Vector3f startPos = al::getTrans(frog) - al::getGravity(frog) * 50.0f;
    sead::Vector3f direction = al::getGravity(frog) * 240.0f;
    sead::Vector3f hitPos;
    if (!alCollisionUtil::getFirstPolyOnArrow(frog, &hitPos, nullptr, startPos, direction, nullptr,
                                              nullptr))
        return false;
    if (!finder->isFoundSurface())
        return true;
    sead::Vector3f surfacePos = finder->getSurfacePosition();
    sead::Vector3f diff = hitPos - surfacePos;
    if (diff.dot(al::getGravity(frog)) > 0.0f)
        return false;
    return true;
}

void Frog::initAfterPlacement() {
    rs::tryRegisterSphinxQuizRouteKillSensorAfterPlacement(al::getHitSensor(this, "Body"));
}

void Frog::updateCollider() {
    if (mPlayerHack && rs::isActiveHackStartDemo(mPlayerHack))
        return;
    al::LiveActor::updateCollider();
}

void Frog::die() {
    if (al::isNerve(this, &NrvHostType.Revive) || al::isNerve(this, &NrvHostType.Reset))
        return;
    al::startHitReaction(this, "死亡");
    al::setNerve(this, &NrvHostType.Die);
}

bool Frog::isOnWaterSurface() const {
    if (!mWaterSurfaceFinder->isFoundSurface())
        return false;
    return !isGroundAboveWaterSurface(this, mWaterSurfaceFinder);
}

bool Frog::isSwimActionPlaying() const {
    if (al::isActionPlaying(this, "SwimWait"))
        return true;
    if (al::isActionPlaying(this, "SwimSwoonStart"))
        return true;
    if (al::isActionPlaying(this, "SwimSwoonStartLand"))
        return true;
    if (al::isActionPlaying(this, "SwimSwoon"))
        return true;
    if (al::isActionPlaying(this, "SwimSwoonEnd"))
        return true;
    if (al::isActionPlaying(this, "SwimHackStart"))
        return true;
    return al::isActionPlaying(this, "SwimReaction");
}

void Frog::updateVelocity() {
    if (al::isOnGround(this, 3)) {
        al::scaleVelocity(this, *mFrogParam->mFrictionNormal);
        al::addVelocityToGravityFittedGround(this, *mFrogParam->mGravity, 3);
    } else {
        al::scaleVelocity(this, *mFrogParam->mFrictionAir);
        al::addVelocityToGravityLimit(this, *mFrogParam->mGravity, 35.0f);
    }
}

void Frog::updateSurfaceTransform() {
    if (mWaterSurfaceFinder->isFoundSurface()) {
        sead::Vector3f surfacePos = mWaterSurfaceFinder->getSurfacePosition();
        sead::Vector3f surfaceNormal = mWaterSurfaceFinder->getSurfaceNormal();
        al::makeMtxUpNoSupportPos(&mSurfaceMtx, surfaceNormal, surfacePos);
    }
}

void Frog::surfaceJump() {
    if (mWaterSurfaceFinder->isFoundSurface()) {
        sead::Vector3f surfacePos = mWaterSurfaceFinder->getSurfacePosition();
        sead::Vector3f surfaceNormal = mWaterSurfaceFinder->getSurfaceNormal();
        al::makeMtxUpNoSupportPos(&mSurfaceMtx, surfaceNormal, surfacePos);
    }
    mSurfaceJumpCount = 15;
}

void Frog::onDynamics() {
    if (mJointSpringControllerHolder)
        mJointSpringControllerHolder->onControlAll();
}

void Frog::offDynamics() {
    if (mJointSpringControllerHolder)
        mJointSpringControllerHolder->offControlAll();
}

void Frog::endHack() {
    al::endBgmSituation(this, "CaptureFrog", false);
}

void Frog::exeAppear() {
    if (al::isFirstStep(this)) {
        rs::tryAppearEnemyCap(mEnemyCap);
        al::startAction(this, "AppearStart");
    }
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvHostType.Wait);
}

void Frog::exeWander() {
    al::updateNerveState(this);
}

void Frog::exeDamageCap() {
    if (al::isFirstStep(this))
        al::startAction(this, "DamageCap");
    updateVelocity();
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvHostType.Wait);
}

void Frog::exeWaitHackStart() {
    if (rs::isHackStartDemoEnterMario(mPlayerHack)) {
        al::setNerve(this, &NrvHostType.HackStart);
        return;
    }
    if (isOnWaterSurface())
        al::setVelocityZeroV(this);
}

void Frog::exeDemoHackFirst() {
    if (al::updateNerveStateAndNextNerve(this, &NrvHostType.Hack)) {
        al::startAction(this, "HackOn");
        al::invalidateShadowMask(this);
        al::offDepthShadowModel(this);
        al::validateDepthShadowMap(this);
    }
}

void Frog::exeHack() {
    if (al::isFirstStep(this)) {
        mJointRippleGenerator1->reset();
        mJointRippleGenerator2->reset();
        al::startBgmSituation(this, "CaptureFrog", false, true);
    }
    mJointRippleGenerator1->updateAndGenerate();
    mJointRippleGenerator2->updateAndGenerate();
    if (al::updateNerveStateAndNextNerve(this, &NrvHostType.Swoon))
        al::setVelocityJump(this, *mFrogParam->mHackEndJumpSpeed);
}

void Frog::exeRevive() {
    if (al::updateNerveStateAndNextNerve(this, &NrvHostType.ReviveAppear))
        rs::tryAppearEnemyCap(mEnemyCap);
}

void Frog::exeReviveAppear() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::startAction(this, "AppearStart");
    }
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvHostType.Wait);
}

void Frog::exeReset() {
    if (al::updateNerveStateAndNextNerve(this, &NrvHostType.Wait))
        rs::tryAppearEnemyCap(mEnemyCap);
}

void Frog::exeDie() {
    // Inverted ternary: the original computes Revive (+0x10) into x10 and Reset
    // (+0x20) into x9, selecting with `csel ..., eq` (i.e. on !mIsRevive).
    al::setNerve(this, !mIsRevive ? static_cast<const al::Nerve*>(&NrvHostType.Reset)
                                  : static_cast<const al::Nerve*>(&NrvHostType.Revive));
}
