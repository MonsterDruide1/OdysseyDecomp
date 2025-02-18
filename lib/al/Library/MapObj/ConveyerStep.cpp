#include "Library/MapObj/ConveyerStep.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
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
    mMaxCoord = coord;
}

void ConveyerStep::setTransByCoord(f32 coord, bool isForwards) {
    setTransByCoord(coord, isForwards, false);
}

void ConveyerStep::setTransByCoord(f32 coord, bool isForwards, bool isForceReset) {
    f32 newCoord = modf(mMaxCoord + coord, mMaxCoord) + 0.0f;
    s32 index = -1;

    mConveyerKeyKeeper->calcPosAndQuat(getTransPtr(this), getQuatPtr(this), &index, newCoord);

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

    if ((isForwards && newCoord < mCurrentCoord) || (!isForwards && newCoord > mCurrentCoord) ||
        isForceReset)
        resetPosition(this);

    if (newCoord > mConveyerKeyKeeper->getTotalMoveDistance()) {
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

    mCurrentCoord = newCoord;
}

void ConveyerStep::setTransAndResetByCoord(f32 coord) {
    setTransByCoord(coord, true, true);
}

void ConveyerStep::exeWait() {}
}  // namespace al
