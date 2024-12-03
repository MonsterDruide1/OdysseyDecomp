#include "Library/MapObj/ConveyerStep.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Project/LiveActor/ConveyerKeyKeeper.h"

namespace {
using namespace al;

NERVE_IMPL(ConveyerStep, Wait)

NERVES_MAKE_STRUCT(ConveyerStep, Wait)
}  // namespace

namespace al {
ConveyerStep::ConveyerStep(const char* name) : LiveActor(name) {}

void ConveyerStep::init(const ActorInitInfo& info) {
    initMapPartsActor(this, info, nullptr);
    initNerve(this, &NrvConveyerStep.Wait, 0);
    onDrawClipping(this);

    makeActorAlive();
}

bool ConveyerStep::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (mHost != nullptr)
        return mHost->receiveMsg(message, other, self);

    return false;
}

void ConveyerStep::setHost(LiveActor* host) {
    mHost = host;
}

void ConveyerStep::setConveyerKeyKeeper(const ConveyerKeyKeeper* conveyerKeyKeeper, f32 coord) {
    mConveyerKeyKeeper = conveyerKeyKeeper;
    mCoord = coord;
}

void ConveyerStep::setTransByCoord(f32 coord, bool isMoving) {
    setTransByCoord(coord, isMoving, false);
}

__attribute__((noinline)) void ConveyerStep::setTransByCoord(f32 coord, bool isMoving,
                                                             bool forceReset) {
    f32 relativeCoord = modf(mCoord + coord, mCoord) + 0.0f;
    s32 index = -1;

    mConveyerKeyKeeper->calcPosAndQuat(getTransPtr(this), getQuatPtr(this), &index, relativeCoord);

    const char* keyHitReactionName = nullptr;
    const char* actionName = nullptr;

    if (index > -1) {
        const ConveyerKey* conveyerKey = mConveyerKeyKeeper->getConveyerKey(index);

        if (tryGetStringArg(&keyHitReactionName, conveyerKey->mPlacementInfo,
                            "KeyHitReactionName") &&
            (mKeyHitReactionName == nullptr ||
             !isEqualString(mKeyHitReactionName, keyHitReactionName)))
            startHitReaction(this, keyHitReactionName);

        if (tryGetStringArg(&actionName, conveyerKey->mPlacementInfo, "ActionName") &&
            (mActionName == nullptr || !isEqualString(mActionName, actionName)))
            startAction(this, actionName);
    }

    mKeyHitReactionName = keyHitReactionName;
    mActionName = actionName;

    if ((isMoving && relativeCoord < mRelativeCoord) ||
        (!isMoving && relativeCoord > mRelativeCoord) || forceReset)
        resetPosition(this);

    if (relativeCoord > mConveyerKeyKeeper->get_34()) {
        if (mIsExist) {
            mIsExist = false;
            if (getModelKeeper() != nullptr && !isHideModel(this))
                hideModel(this);
            if (isExistCollisionParts(this))
                invalidateCollisionParts(this);
        }
    } else if (!mIsExist) {
        mIsExist = true;
        if (getModelKeeper() != nullptr && isHideModel(this))
            showModel(this);
        if (isExistCollisionParts(this))
            validateCollisionParts(this);
    }

    mRelativeCoord = relativeCoord;
}

void ConveyerStep::setTransAndResetByCoord(f32 coord) {
    setTransByCoord(coord, true, true);
}

void ConveyerStep::exeWait() {}
}  // namespace al
