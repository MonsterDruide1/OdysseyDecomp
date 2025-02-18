#include "MapObj/FurnitureStateWait.h"

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(FurnitureStateWait, Wait);
NERVE_IMPL(FurnitureStateWait, BindRequest);

NERVES_MAKE_STRUCT(FurnitureStateWait, Wait, BindRequest);
}  // namespace

FurnitureStateWait::FurnitureStateWait(al::LiveActor* actor, FurnitureType furniture)
    : al::ActorStateBase("待機", actor), mFurnitureType(furniture) {
    initNerve(&NrvFurnitureStateWait.Wait, 0);
}

void FurnitureStateWait::appear() {
    setDead(false);
    al::setNerve(this, &NrvFurnitureStateWait.Wait);
    mIsPlayerNotOnGround = !rs::isPlayerOnGround(mActor);
    mPlayerState = PlayerFurnitureState::Unbound;
}

bool FurnitureStateWait::isEnableBindRequest() const {
    return al::isNerve(this, &NrvFurnitureStateWait.BindRequest);
}

void FurnitureStateWait::exeWait() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this))
        mPlayerPos = rs::getPlayerPos(actor);

    bool isPlayerNoInput = rs::isPlayerNoInput(actor);
    FurnitureType furniture = mFurnitureType;
    bool isPlayerOnSomething = false;

    if (rs::isPlayerNoInput(actor)) {
        if (furniture == FurnitureType::Chair)
            isPlayerOnSomething = rs::isPlayerOnChairActor(actor);
        if (furniture == FurnitureType::Bed)
            isPlayerOnSomething = rs::isPlayerOnBedActor(actor);
    }

    bool isPlayerOnGround = rs::isPlayerOnGround(actor);
    const sead::Vector3f& newPlayerPos = rs::getPlayerPos(actor);
    bool isPlayerStationary = al::isNearZero(mPlayerPos - newPlayerPos, 0.001f);
    mPlayerPos = newPlayerPos;

    if (!isPlayerOnSomething || !isPlayerNoInput || !isPlayerStationary) {
        mIsPlayerNotOnGround = !isPlayerOnGround;
        al::setNerve(this, &NrvFurnitureStateWait.Wait);
        return;
    }
    if (mIsPlayerNotOnGround) {
        sead::Vector3f frontDir = sead::Vector3f::zero;
        rs::calcPlayerFrontDir(&frontDir, actor);
        sead::Vector3f plane = sead::Vector3f::zero;
        al::calcFrontDir(&plane, actor);
        f32 angle = al::calcAngleOnPlaneDegree(frontDir, plane, sead::Vector3f::ey);

        if (90.0f < sead::Mathf::abs(angle))
            mPlayerState = PlayerFurnitureState::AirNotFacing;
        else
            mPlayerState = PlayerFurnitureState::AirFacing;
        mIsPlayerNotOnGround = false;
        return;
    }

    s32 waitTime = (mFurnitureType == FurnitureType::Chair) ? 30 : 120;
    if (al::isGreaterEqualStep(this, waitTime)) {
        mPlayerState = PlayerFurnitureState::Bound;
        al::setNerve(this, &NrvFurnitureStateWait.BindRequest);
    }
}

void FurnitureStateWait::exeBindRequest() {
    al::LiveActor* actor = mActor;
    FurnitureType furniture = mFurnitureType;

    if (!rs::isPlayerNoInput(actor)) {
        al::setNerve(this, &NrvFurnitureStateWait.Wait);
        return;
    }

    bool isPlayerOnSomething = false;
    if (furniture == FurnitureType::Bed)
        isPlayerOnSomething = rs::isPlayerOnBedActor(actor);
    if (furniture == FurnitureType::Chair)
        isPlayerOnSomething = rs::isPlayerOnChairActor(actor);
    if (!isPlayerOnSomething)
        al::setNerve(this, &NrvFurnitureStateWait.Wait);
}
