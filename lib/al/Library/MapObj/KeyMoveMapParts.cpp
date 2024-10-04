#include "Library/MapObj/KeyMoveMapParts.h"

#include "ChildStep.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/Fluid/RippleCtrl.h"
#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(KeyMoveMapParts, StandBy)
NERVE_ACTION_IMPL(KeyMoveMapParts, Delay)
NERVE_ACTION_IMPL(KeyMoveMapParts, Wait)
NERVE_ACTION_IMPL(KeyMoveMapParts, MoveSign)
NERVE_ACTION_IMPL(KeyMoveMapParts, Move)
NERVE_ACTION_IMPL(KeyMoveMapParts, StopSign)
NERVE_ACTION_IMPL(KeyMoveMapParts, Stop)

NERVE_ACTIONS_MAKE_STRUCT(KeyMoveMapParts, StandBy, Delay, Wait, MoveSign, Move, StopSign, Stop)
}  // namespace

namespace al {
KeyMoveMapParts::KeyMoveMapParts(const char* name) : LiveActor(name) {}

void KeyMoveMapParts::init(const ActorInitInfo& info) {
    using KeyMoveMapPartsFunctor = FunctorV0M<KeyMoveMapParts*, void (KeyMoveMapParts::*)()>;

    initNerveAction(this, "Wait", &NrvKeyMoveMapParts.mCollector, 0);

    const char* suffix = nullptr;
    tryGetStringArg(&suffix, info, "SuffixName");
    initMapPartsActor(this, info, suffix);

    tryGetQuatPtr(this);

    mKeyPoseKeeper = createKeyPoseKeeper(info);

    registerAreaHostMtx(this, info);

    setKeyMoveClippingInfo(this, &_128, mKeyPoseKeeper);

    tryGetArg(&mIsFloorTouchStart, info, "IsFloorTouchStart");
    tryGetArg(&mIsHipDropStart, info, "IsHipDropStart");
    tryGetArg(&mIsStopKill, info, "IsStopKill");
    tryGetArg(&mDelayTime, info, "DelayTime");

    if (mDelayTime > 0)
        startNerveAction(this, "Delay");

    bool isListenStart =
        listenStageSwitchOnStart(this, KeyMoveMapPartsFunctor(this, &KeyMoveMapParts::start));
    s32 keyPoseCount = getKeyPoseCount(mKeyPoseKeeper);
    u8 tmp;
    if (keyPoseCount < 2 || (tmp = mIsFloorTouchStart, (tmp & 0xff) != 0) ||
        (isListenStart | (0xff < tmp)))
        startNerveAction(this, "StandBy");

    mSwitchKeepOnAreaGroup = tryCreateSwitchKeepOnAreaGroup(this, info);
    mSwitchOnAreaGroup = tryCreateSwitchOnAreaGroup(this, info);

    trySyncStageSwitchAppear(this);
    tryListenStageSwitchKill(this);
    listenStageSwitchOn(this, "SwitchStop", KeyMoveMapPartsFunctor(this, &KeyMoveMapParts::stop));
    listenStageSwitchOn(this, "SwitchReset",
                        KeyMoveMapPartsFunctor(this, &KeyMoveMapParts::appearAndSetStart));

    registActorToDemoInfo(this, info);

    tryInitSubActorKeeperChildStep(this, info);
    createChildStep(info, this, true);
    if (!isExistModel(this))
        return;

    mRippleCtrl = RippleCtrl::tryCreate(this);
    if (mRippleCtrl == nullptr)
        return;

    mRippleCtrl->init(info);
}
}  // namespace al
