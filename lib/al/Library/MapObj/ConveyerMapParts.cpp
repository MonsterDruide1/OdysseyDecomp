#include "Library/MapObj/ConveyerMapParts.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/MapObj/ConveyerStep.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"
#include "Project/LiveActor/ConveyerKeyKeeper.h"

namespace {
using namespace al;

NERVE_IMPL(ConveyerMapParts, Move)
NERVE_IMPL(ConveyerMapParts, StandBy)

NERVES_MAKE_STRUCT(ConveyerMapParts, Move, StandBy)
}  // namespace

namespace al {
inline void registerConveyerSteps(DeriveActorGroup<ConveyerStep>* conveyerStepGroup,
                                  const ActorInitInfo& info) {
    for (s32 i = 0; i < conveyerStepGroup->getMaxActorCount(); i++) {
        ConveyerStep* conveyerStep = new ConveyerStep("コンベア足場");
        initCreateActorWithPlacementInfo(conveyerStep, info);
        conveyerStepGroup->registerActor(conveyerStep);
    }
}

ConveyerMapParts::ConveyerMapParts(const char* name) : LiveActor(name) {}

void ConveyerMapParts::init(const ActorInitInfo& info) {
    using ConveyerMapPartsFunctor = FunctorV0M<ConveyerMapParts*, void (ConveyerMapParts::*)()>;

    initActorSceneInfo(this, info);
    initExecutorMapObjMovement(this, info);
    initActorPoseTQSV(this);
    tryGetQuatPtr(this);
    initActorSRT(this, info);
    initActorClipping(this, info);
    initGroupClipping(this, info);
    initNerve(this, &NrvConveyerMapParts.Move, 0);
    initStageSwitch(this, info);

    mConveyerKeyKeeper = new ConveyerKeyKeeper();
    mConveyerKeyKeeper->init(info);

    tryGetArg(&mMoveSpeed, info, "MoveSpeed");
    tryGetArg(&mPartsInterval, info, "PartsInterval");
    tryGetArg(&mIsRideOnlyMove, info, "IsRideOnlyMove");

    if (mPartsInterval < 10.0f)
        mPartsInterval = 10.0f;

    f32 totalMoveDistance = mConveyerKeyKeeper->getTotalMoveDistance();
    if (mConveyerKeyKeeper->getConveyerKeyCount() > 1)
        isNearZero(totalMoveDistance, 0.001f);

    s32 groupCount = (s32)(totalMoveDistance / mPartsInterval) + 1;
    mMaxCoord = mPartsInterval * (f32)groupCount;

    f32 startRate = 0.0f;
    tryGetArg(&startRate, info, "StartRate");

    f32 rate = mPartsInterval * startRate;
    mOffsetCoord = modf(rate + mPartsInterval, mPartsInterval) + 0.0f;

    mConveyerStepGroup = new DeriveActorGroup<ConveyerStep>("コンベア足場リスト", groupCount);
    registerConveyerSteps(mConveyerStepGroup, info);

    for (s32 i = 0; i < groupCount; i++) {
        ConveyerStep* conveyerStep = mConveyerStepGroup->getDeriveActor(i);
        conveyerStep->setHost(this);
        conveyerStep->setConveyerKeyKeeper(mConveyerKeyKeeper, mMaxCoord);
        conveyerStep->setTransAndResetByCoord((f32)i * mPartsInterval + mOffsetCoord);
    }

    f32 clippingRadius = 0.0f;
    mConveyerKeyKeeper->calcClippingSphere(&mClippingTrans, &clippingRadius,
                                           getClippingRadius(mConveyerStepGroup->getActor(0)));
    setClippingInfo(this, clippingRadius, &mClippingTrans);

    bool isListenStartOnOff =
        listenStageSwitchOnOffStart(this, ConveyerMapPartsFunctor(this, &ConveyerMapParts::start),
                                    ConveyerMapPartsFunctor(this, &ConveyerMapParts::stop));
    if (isListenStartOnOff)
        setNerve(this, &NrvConveyerMapParts.StandBy);

    makeActorAlive();
}

void ConveyerMapParts::start() {
    if (!isNerve(this, &NrvConveyerMapParts.StandBy))
        return;

    setNerve(this, &NrvConveyerMapParts.Move);
}

void ConveyerMapParts::stop() {
    if (!isNerve(this, &NrvConveyerMapParts.Move))
        return;

    setNerve(this, &NrvConveyerMapParts.StandBy);
}

bool ConveyerMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgFloorTouch(message)) {
        mAddRideActiveFrames = 2;

        return true;
    }

    return false;
}

void ConveyerMapParts::control() {
    if (mAddRideActiveFrames > 0) {
        if (mRideActiveFrames < mMaxRideActiveFrames)
            mRideActiveFrames++;
        else
            mRideActiveFrames = mMaxRideActiveFrames;

        mAddRideActiveFrames--;

        return;
    }

    if (mRideActiveFrames != 0)
        mRideActiveFrames--;
}

void ConveyerMapParts::startClipped() {
    LiveActor::startClipped();

    for (s32 i = 0; i < mConveyerStepGroup->getActorCount(); i++)
        offDrawClipping(mConveyerStepGroup->getActor(i));
}

void ConveyerMapParts::endClipped() {
    LiveActor::endClipped();

    for (s32 i = 0; i < mConveyerStepGroup->getActorCount(); i++)
        onDrawClipping(mConveyerStepGroup->getActor(i));
}

void ConveyerMapParts::exeStandBy() {}

void ConveyerMapParts::exeMove() {
    if (!mIsRideOnlyMove || mRideActiveFrames >= 1) {
        f32 speedFactor =
            mIsRideOnlyMove ? (f32)mRideActiveFrames / (f32)mMaxRideActiveFrames : 1.0f;
        mOffsetCoord = modf(mOffsetCoord + speedFactor * mMoveSpeed + mMaxCoord, mMaxCoord) + 0.0f;

        bool isForwards = mMoveSpeed >= 0.0f;
        s32 actorCount = mConveyerStepGroup->getActorCount();
        for (s32 i = 0; i < actorCount; i++)
            mConveyerStepGroup->getDeriveActor(i)->setTransByCoord(
                mPartsInterval * (f32)i + mOffsetCoord, isForwards);
    }
}
}  // namespace al
