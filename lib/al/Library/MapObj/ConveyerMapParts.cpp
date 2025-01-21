#include "Library/MapObj/ConveyerMapParts.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
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

    s32 iVar7 = (s32)(fVar10 / mPartsInterval);
    s32 groupCount = iVar7 + 1;
    _130 = mPartsInterval * (f32)groupCount;

    f32 startRate = 0.0f;
    tryGetArg(&startRate, info, "StartRate");

    _124 = modf(mPartsInterval * startRate + mPartsInterval, mPartsInterval) + 0.0f;

    mConveyerSteps = new DeriveActorGroup<ConveyerStep>("コンベア足場リスト", groupCount);
    for (s32 i = 0; i < mConveyerSteps->getMaxActorCount(); i++) {
        ConveyerStep* conveyerStep = new ConveyerStep("コンベア足場");
        initCreateActorWithPlacementInfo(conveyerStep, info);
        mConveyerSteps->registerActor(conveyerStep);
    }

    for (s32 i = 0; i < iVar7 - 1; i++) {
        ConveyerStep* conveyerStep = mConveyerSteps->getDeriveActor(i);
        conveyerStep->setHost(this);
        conveyerStep->setConveyerKeyKeeper(mConveyerKeyKeeper, _130);
        conveyerStep->setTransAndResetByCoord((f32)i * mPartsInterval + _124);
    }

    f32 local_68 = 0.0f;
    mConveyerKeyKeeper->calcClippingSphere(&_118, &local_68,
                                           getClippingRadius(mConveyerSteps->getActor(0)));
    setClippingInfo(this, local_68, &_118);

    bool isListenStartOnOff =
        listenStageSwitchOnOffStart(this, ConveyerMapPartsFunctor(this, &ConveyerMapParts::start),
                                    ConveyerMapPartsFunctor(this, &ConveyerMapParts::stop));
    if (isListenStartOnOff)
        setNerve(this, &NrvConveyerMapParts.StandBy);

    makeActorAlive();
}
}  // namespace al
