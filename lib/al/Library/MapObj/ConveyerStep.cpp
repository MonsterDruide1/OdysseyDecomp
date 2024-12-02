#include "Library/MapObj/ConveyerStep.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"

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

void ConveyerStep::setConveyerKeyKeeper(const ConveyerKeyKeeper* conveyerKeyKeeper, f32 param_2) {
    mConveyerKeyKeeper = conveyerKeyKeeper;
    _12c = param_2;
}

void ConveyerStep::setTransByCoord(f32 param_1, bool param_2) {
    setTransByCoord(param_1, param_2, false);
}

inline void test(ConveyerStep* conveyerStep, f32 mod) __attribute__((always_inline)) {
    if (mod <= conveyerStep->getConveyerKeyKeeper()->get_34()) {
        if (!conveyerStep->get_130()) {
            conveyerStep->set_130(true);
            if (conveyerStep->getModelKeeper() != nullptr && isHideModel(conveyerStep))
                showModel(conveyerStep);
            if (isExistCollisionParts(conveyerStep))
                validateCollisionParts(conveyerStep);
        }
    } else if (conveyerStep->get_130()) {
        conveyerStep->set_130(false);
        if (conveyerStep->getModelKeeper() != nullptr && !isHideModel(conveyerStep))
            hideModel(conveyerStep);
        if (isExistCollisionParts(conveyerStep))
            invalidateCollisionParts(conveyerStep);
    }

    conveyerStep->set_128(mod);
}

void ConveyerStep::setTransByCoord(f32 param_1, bool param_2, bool param_3)
    __attribute__((noinline)) {
    f32 mod = modf(_12c + param_1, _12c) + 0.0f;
    s32 index = -1;

    mConveyerKeyKeeper->calcPosAndQuat(getTransPtr(this), getQuatPtr(this), &index, mod);

    const char* keyHitReactionName = nullptr;
    const char* actionName = nullptr;

    if (index > -1) {
        const ConveyerKey conveyerKey = mConveyerKeyKeeper->getConveyerKey(index);

        if (tryGetStringArg(&keyHitReactionName, *conveyerKey.mPlacementInfo,
                            "KeyHitReactionName") &&
            (mKeyHitReactionName == nullptr ||
             !isEqualString(mKeyHitReactionName, keyHitReactionName)))
            startHitReaction(this, keyHitReactionName);

        if (tryGetStringArg(&actionName, *conveyerKey.mPlacementInfo, "ActionName") &&
            (mActionName == nullptr || !isEqualString(mActionName, actionName)))
            startHitReaction(this, actionName);
    }

    mKeyHitReactionName = keyHitReactionName;
    mActionName = actionName;

    if (param_3 || (!param_2 && _128 >= mod) || (param_2 && _128 <= mod))
        resetPosition(this);

    test(this, mod);
}

void ConveyerStep::setTransAndResetByCoord(f32 param_1) {
    setTransByCoord(param_1, true, true);
}

void ConveyerStep::exeWait() {}
}  // namespace al
