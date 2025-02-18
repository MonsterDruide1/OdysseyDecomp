#include "Library/MapObj/VisibleSwitchMapParts.h"

#include "Library/Collision/PartsConnector.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_IMPL(VisibleSwitchMapParts, Show)
NERVE_IMPL(VisibleSwitchMapParts, Hide)
NERVE_IMPL(VisibleSwitchMapParts, Disappear)
NERVE_IMPL(VisibleSwitchMapParts, DisappearDither)
NERVE_IMPL(VisibleSwitchMapParts, Appear)
NERVE_IMPL(VisibleSwitchMapParts, AppearDither)

NERVES_MAKE_STRUCT(VisibleSwitchMapParts, Show, Hide, Disappear, DisappearDither, Appear,
                   AppearDither)
}  // namespace

namespace al {
VisibleSwitchMapParts::VisibleSwitchMapParts(const char* name) : LiveActor(name) {}

void VisibleSwitchMapParts::init(const ActorInitInfo& info) {
    using VisibleSwitchMapPartsFunctor =
        FunctorV0M<al::VisibleSwitchMapParts*, void (al::VisibleSwitchMapParts::*)()>;

    initMapPartsActor(this, info, nullptr);
    initNerve(this, &NrvVisibleSwitchMapParts.Show, 0);

    makeActorAlive();

    if (isValidStageSwitch(this, "SwitchAppear") && isValidStageSwitch(this, "SwitchDisappear"))
        return;

    if (isValidStageSwitch(this, "SwitchDisappear")) {
        listenStageSwitchOnOff(
            this, "SwitchDisappear",
            VisibleSwitchMapPartsFunctor(this, &VisibleSwitchMapParts::startDisappear),
            VisibleSwitchMapPartsFunctor(this, &VisibleSwitchMapParts::startAppear));
    } else if (isValidStageSwitch(this, "SwitchAppear")) {
        listenStageSwitchOnOff(
            this, "SwitchAppear",
            VisibleSwitchMapPartsFunctor(this, &VisibleSwitchMapParts::startAppear),
            VisibleSwitchMapPartsFunctor(this, &VisibleSwitchMapParts::startDisappear));
        setNerve(this, &NrvVisibleSwitchMapParts.Hide);
    } else {
        return;
    }

    listenStageSwitchOn(
        this, "SwitchSuddenDisappear",
        VisibleSwitchMapPartsFunctor(this, &VisibleSwitchMapParts::startSuddenDisappear));

    tryGetArg(&mConnectMapCheckLength, info, "ConnectMapCheckLength");
    tryGetArg((s32*)&mConnectMapDirType, info, "ConnectMapDirType");
    tryGetArg(&mDitherFrame, info, "DitherFrame");
    tryGetArg(&mIsUsingAlphaMask, info, "UseAlphaMask");

    if (mConnectMapDirType != ConnectMapDirType::Invalid)
        mMtxConnector = createMtxConnector(this);
}

void VisibleSwitchMapParts::startDisappear() {
    if (isNerve(this, &NrvVisibleSwitchMapParts.Disappear) ||
        isNerve(this, &NrvVisibleSwitchMapParts.DisappearDither) ||
        isNerve(this, &NrvVisibleSwitchMapParts.Hide))
        return;

    if (mIsUsingAlphaMask)
        setNerve(this, &NrvVisibleSwitchMapParts.DisappearDither);
    else
        setNerve(this, &NrvVisibleSwitchMapParts.Disappear);
}

void VisibleSwitchMapParts::startAppear() {
    if (isNerve(this, &NrvVisibleSwitchMapParts.Appear) ||
        isNerve(this, &NrvVisibleSwitchMapParts.AppearDither))
        return;

    if (mIsUsingAlphaMask)
        setNerve(this, &NrvVisibleSwitchMapParts.AppearDither);
    else
        setNerve(this, &NrvVisibleSwitchMapParts.Appear);
}

void VisibleSwitchMapParts::startSuddenDisappear() {
    if (isNerve(this, &NrvVisibleSwitchMapParts.Hide))
        return;

    if (mIsUsingAlphaMask)
        setModelAlphaMask(this, 0.0f);

    setNerve(this, &NrvVisibleSwitchMapParts.Hide);
}

void VisibleSwitchMapParts::initAfterPlacement() {
    if (mMtxConnector == nullptr)
        return;

    sead::Vector3f dir;
    switch (mConnectMapDirType) {
    case ConnectMapDirType::Side:
        calcSideDir(&dir, this);

        break;
    case ConnectMapDirType::Up:
        calcUpDir(&dir, this);

        break;
    case ConnectMapDirType::Front:
        calcFrontDir(&dir, this);

        break;
    default:
        break;
    }

    if (mConnectMapDirType == ConnectMapDirType::NegSide ||
        mConnectMapDirType == ConnectMapDirType::NegUp ||
        mConnectMapDirType == ConnectMapDirType::NegFront)
        dir = -dir;

    attachMtxConnectorToCollision(mMtxConnector, this, getTrans(this),
                                  mConnectMapCheckLength * dir);
}

void VisibleSwitchMapParts::control() {
    if (mMtxConnector == nullptr)
        return;

    connectPoseQT(this, mMtxConnector);
}

bool VisibleSwitchMapParts::receiveMsg(const SensorMsg* message, HitSensor* other,
                                       HitSensor* self) {
    if (isMsgScreenPointInvalidCollisionParts(message) &&
        isNerve(this, &NrvVisibleSwitchMapParts.Hide))
        return true;

    return false;
}

void VisibleSwitchMapParts::exeShow() {
    if (isFirstStep(this))
        tryStartAction(this, "Wait");
}

void VisibleSwitchMapParts::exeDisappear() {
    if (isFirstStep(this)) {
        if (!tryStartAction(this, "Disappear")) {
            setNerve(this, &NrvVisibleSwitchMapParts.Hide);

            return;
        }
    }

    if (isActionEnd(this))
        setNerve(this, &NrvVisibleSwitchMapParts.Hide);
}

void VisibleSwitchMapParts::exeDisappearDither() {
    setModelAlphaMask(this, 1.0f - calcNerveRate(this, mDitherFrame));

    if (isGreaterEqualStep(this, mDitherFrame))
        setNerve(this, &NrvVisibleSwitchMapParts.Hide);
}

void VisibleSwitchMapParts::exeHide() {
    if (isFirstStep(this))
        hideModelIfShow(this);
}

void VisibleSwitchMapParts::exeAppear() {
    if (isFirstStep(this)) {
        showModelIfHide(this);

        if (!tryStartAction(this, "Appear")) {
            setNerve(this, &NrvVisibleSwitchMapParts.Show);

            return;
        }
    }

    if (isActionEnd(this))
        setNerve(this, &NrvVisibleSwitchMapParts.Show);
}

void VisibleSwitchMapParts::exeAppearDither() {
    if (isFirstStep(this))
        showModelIfHide(this);

    setModelAlphaMask(this, calcNerveRate(this, mDitherFrame));

    if (isGreaterEqualStep(this, mDitherFrame))
        setNerve(this, &NrvVisibleSwitchMapParts.Show);
}
}  // namespace al
