#include "Npc/FlyObject.h"

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Rail/RailUtil.h"

#include "MapObj/CapMessageShowInfo.h"
#include "MapObj/FukankunZoomTargetFunction.h"
#include "Util/ItemUtil.h"

namespace {
NERVE_IMPL(FlyObject, Fly)
NERVE_IMPL(FlyObject, Disappear)

NERVES_MAKE_NOSTRUCT(FlyObject, Fly, Disappear)
}  // namespace

WaveMovementController::WaveMovementController() = default;

f32 WaveMovementController::calcUpdatedYCoord(const sead::Vector3f& trans) {
    if (mVerticalSpeed == 0.0f || mVerticalAmplitude == 0.0f)
        return trans.y;

    f32 nextAngle = mVerticalSpeed * (1.0f / 60.0f) + mVerticalAngle;
    if (nextAngle > sead::Mathf::pi2())
        nextAngle = 0.0f;

    mVerticalAngle = nextAngle;
    return trans.y + sead::Mathf::sin(mVerticalAngle) * mVerticalAmplitude;
}

void Stationary::setUp(al::LiveActor* actor) {
    mTrans = al::getTrans(actor);
}

void Stationary::move(al::LiveActor* actor, f32 speed) {
    al::setTransY(actor, getWaveController()->calcUpdatedYCoord(mTrans));
}

void OnRails::setUp(al::LiveActor* actor) {
    al::setSyncRailToNearestRailControlPoint(actor);

    mMoveSyncRail = al::isLoopRail(actor) ? al::moveSyncRailLoop : al::moveSyncRail;
}

void OnRails::move(al::LiveActor* actor, f32 speed) {
    mMoveSyncRail(actor, speed);
    al::setTransY(actor, getWaveController()->calcUpdatedYCoord(al::getTrans(actor)));

    sead::Vector3f railDir = al::getRailDir(actor);
    const sead::Vector3f& up = -al::getGravity(actor);

    sead::Vector3f front = railDir - railDir.dot(up) * up;
    if (al::tryNormalizeOrZero(&front))
        al::setFront(actor, front);

    if (al::isRailReachedEnd(actor) && !al::isLoopRail(actor))
        al::setNerve(actor, &Disappear);
}

FukanKunInteractionEmpty::FukanKunInteractionEmpty() {}

void FukanKunInteractionEmpty::init(FlyObject* flyObject, const al::ActorInitInfo& info) {}

void FukanKunInteractionEmpty::setUp(FlyObject* flyObject) {}

void FukanKunInteractionEmpty::control(FlyObject* flyObject) {}

al::MessageSystem* FukanKunInteractionEmpty::getMessageSystem() const {
    return nullptr;
}

FukanKunInteractionBase::FukanKunInteractionBase(s32 displayTime) : mDisplayTime(displayTime) {}

void FukanKunInteractionBase::init(FlyObject* flyObject, const al::ActorInitInfo& info) {
    FukankunZoomTargetFunction::declareUseFukankunZoomTargetActor(flyObject);
}

void FukanKunInteractionBase::setUp(FlyObject* flyObject) {
    FukankunZoomTargetFunction::registerFukankunZoomTargetActor(flyObject, 0, sead::Vector3f::zero,
                                                                nullptr);
}

void FukanKunInteractionBase::control(FlyObject* flyObject) {
    if (mHasInteraction && mDisplayTime < FukankunZoomTargetFunction::getWatchCount(flyObject)) {
        interact(flyObject);
        mHasInteraction = false;
    }
}

FukanKunMessageHolder::FukanKunMessageHolder() : FukanKunInteractionBase(180) {}

void FukanKunMessageHolder::init(FlyObject* flyObject, const al::ActorInitInfo& info) {
    mMessageSystem = al::getLayoutInitInfo(info).getMessageSystem();
    mCapMsg = al::StringTmp<64>("CapMsg_%s", al::createPlacementId(info)->getId());
    FukankunZoomTargetFunction::declareUseFukankunZoomTargetActor(flyObject);
}

al::MessageSystem* FukanKunMessageHolder::getMessageSystem() const {
    return mMessageSystem;
}

void FukanKunMessageHolder::interact(FlyObject* flyObject) {
    rs::tryShowCapMessageFromCurrentStageMsg(flyObject, mCapMsg.cstr(), 90, 0);
}

FukanKunShineHolder::FukanKunShineHolder() : FukanKunInteractionBase(120) {}

void FukanKunShineHolder::init(FlyObject* flyObject, const al::ActorInitInfo& info) {
    mShine = rs::tryInitLinkShine(info, "ShineActor", 0);
    FukankunZoomTargetFunction::declareUseFukankunZoomTargetActor(flyObject);
}

void FukanKunShineHolder::interact(FlyObject* flyObject) {
    rs::appearPopupShine(mShine, flyObject);
}

FlyObject::FlyObject(const char* name) : al::LiveActor(name) {}

inline FukanKunInteractionEmpty* createFukanKunInteraction(const al::ActorInitInfo& info) {
    FukanKunInteractionType interactionType = FukanKunInteractionType::None;
    if (al::tryGetArg((s32*)&interactionType, info, "FukanKunInteractionType")) {
        switch (interactionType) {
        case FukanKunInteractionType::None:
            return new FukanKunInteractionEmpty();
        case FukanKunInteractionType::Message:
            return new FukanKunMessageHolder();
        case FukanKunInteractionType::Shine:
            return new FukanKunShineHolder();
        default:
            return nullptr;
        }
    }
    return nullptr;
}

// NON_MATCHING: Bad load order https://decomp.me/scratch/4Q3he
void FlyObject::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    if (al::isExistRail(this))
        mBehavior = new OnRails();
    else
        mBehavior = new Stationary();

    al::tryGetArg(&mMovementSpeed, info, "MovementSpeed");
    al::tryGetArg(mBehavior->getWaveController()->getVerticalSpeedPtr(), info, "VerticalSpeed");
    al::tryGetArg(mBehavior->getWaveController()->getVerticalAmplitudePtr(), info,
                  "VerticalAmplitude");

    FukanKunInteractionEmpty* interaction = createFukanKunInteraction(info);
    if (interaction) {
        mFukanKunInteraction = interaction;
    } else {
        bool isFukanKunMessageNeed = false;
        al::tryGetArg(&isFukanKunMessageNeed, info, "IsFukanKunMessageNeed");
        if (isFukanKunMessageNeed)
            mFukanKunInteraction = new FukanKunMessageHolder();
        else
            mFukanKunInteraction = new FukanKunInteractionEmpty();
    }

    mFukanKunInteraction->init(this, info);

    al::initNerve(this, &Fly, 0);
    al::invalidateClipping(this);
    al::trySyncStageSwitchAppear(this);
}

void FlyObject::initAfterPlacement() {
    mFukanKunInteraction->setUp(this);
    mBehavior->setUp(this);
}

void FlyObject::control() {
    mFukanKunInteraction->control(this);
}

al::MessageSystem* FlyObject::getMessageSystem() const {
    return mFukanKunInteraction->getMessageSystem();
}

void FlyObject::exeFly() {
    if (al::isFirstStep(this))
        al::startAction(this, "Fly");
    mBehavior->move(this, mMovementSpeed);
}

void FlyObject::exeDisappear() {
    makeActorDead();
}
