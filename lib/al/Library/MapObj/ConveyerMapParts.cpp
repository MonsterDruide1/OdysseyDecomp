#include "Library/MapObj/ConveyerMapParts.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/MapObj/ConveyerStep.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
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
ConveyerMapParts::ConveyerMapParts(const char* name) : LiveActor(name) {}

// NON_MATCHING
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

    f32 fVar10 = mConveyerKeyKeeper->get_34();
    if (mConveyerKeyKeeper->getConveyerKeyCount() > 1)
        isNearZero(fVar10, 0.001f);

    s32 groupCount = (s32)(fVar10 / mPartsInterval) + 1;
    mMaxCoord = mPartsInterval * (f32)groupCount;

    f32 startRate = 0.0f;
    tryGetArg(&startRate, info, "StartRate");

    mOffsetCoord = modf(mPartsInterval * startRate + mPartsInterval, mPartsInterval) + 0.0f;

    mConveyerSteps = new DeriveActorGroup<ConveyerStep>("コンベア足場リスト", groupCount);
    s32 maxActorCount = mConveyerSteps->getMaxActorCount();
    for (s32 i = 0; i < maxActorCount; i++) {
        ConveyerStep* conveyerStep = new ConveyerStep("コンベア足場");
        initCreateActorWithPlacementInfo(conveyerStep, info);
        mConveyerSteps->registerActor(conveyerStep);
    }

    for (s32 i = 0; i < groupCount; i++) {
        ConveyerStep* conveyerStep = mConveyerSteps->getDeriveActor(i);
        conveyerStep->setHost(this);
        conveyerStep->setConveyerKeyKeeper(mConveyerKeyKeeper, mMaxCoord);
        conveyerStep->setTransAndResetByCoord((f32)i * mPartsInterval + mOffsetCoord);
    }

    f32 clippingRadius = 0.0f;
    mConveyerKeyKeeper->calcClippingSphere(&mClippingTrans, &clippingRadius,
                                           getClippingRadius(mConveyerSteps->getActor(0)));
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
        _134 = 2;

        return true;
    }

    return false;
}

void ConveyerMapParts::control() {
    if (_134 > 0) {
        if (_138 < _13c)
            _138++;
        else
            _138 = _13c;

        _134--;

        return;
    }

    if (_138 != 0)
        _138--;
}

void ConveyerMapParts::startClipped() {
    LiveActor::startClipped();

    for (s32 i = 0; i < mConveyerSteps->getActorCount(); i++)
        offDrawClipping(mConveyerSteps->getActor(i));
}

void ConveyerMapParts::endClipped() {
    LiveActor::endClipped();

    for (s32 i = 0; i < mConveyerSteps->getActorCount(); i++)
        onDrawClipping(mConveyerSteps->getActor(i));
}

void ConveyerMapParts::exeStandBy() {}

void ConveyerMapParts::exeMove() {
    if (!mIsRideOnlyMove || _138 >= 1) {
        f32 fVar3 = !mIsRideOnlyMove ? 1.0f : (f32)_138 / (f32)_13c;
        mOffsetCoord = modf(mOffsetCoord + fVar3 * mMoveSpeed + mMaxCoord, mMaxCoord) + 0.0f;

        bool isForwards = mMoveSpeed >= 0.0f;
        s32 actorCount = mConveyerSteps->getActorCount();
        for (s32 i = 0; i < actorCount; i++)
            mConveyerSteps->getDeriveActor(i)->setTransByCoord(
                mPartsInterval * (f32)i + mOffsetCoord, isForwards);
    }
}
}  // namespace al
