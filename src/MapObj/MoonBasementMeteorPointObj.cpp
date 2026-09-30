#include "MapObj/MoonBasementMeteorPointObj.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Boss/Koopa/KoopaHackStopCtrl.h"
#include "MapObj/MoonBasementFallObj.h"

void executeWaitNerveImpl(MoonBasementMeteorPointObj* actor);

namespace {
class MoonBasementMeteorPointObjNrvWait : public al::Nerve {
public:
    void execute(al::NerveKeeper* keeper) const override {
        executeWaitNerveImpl(keeper->getParent<MoonBasementMeteorPointObj>());
    }
};

NERVE_IMPL(MoonBasementMeteorPointObj, Stop)
NERVE_IMPL(MoonBasementMeteorPointObj, Delay)

NERVES_MAKE_NOSTRUCT(MoonBasementMeteorPointObj, Stop)

struct {
    MoonBasementMeteorPointObjNrvWait Wait;
    MoonBasementMeteorPointObjNrvDelay Delay;
} NrvMoonBasementMeteorPointObj;
}  // namespace

MoonBasementMeteorPointObj::MoonBasementMeteorPointObj(const char* name) : al::LiveActor(name) {
    mFallObj = nullptr;
    mIsUseChangeTrans = false;
    mIntervalStep = 0;
    mChangeTrans.z = 0.0f;
    mDelayStep = 0;
    mChangeTrans.x = 0.0f;
    mChangeTrans.y = 0.0f;
}

inline void executeWaitNerveImpl(MoonBasementMeteorPointObj* actor) {
    if (al::isFirstStep(actor) && actor->mFallObj->isEnableStartFall()) {
        if (actor->mIsUseChangeTrans)
            al::setTrans(actor->mFallObj, actor->mChangeTrans);
        else
            al::setTrans(actor->mFallObj, al::getTrans(actor));

        actor->mFallObj->appear();
    }

    al::setNerveAtGreaterEqualStep(actor, &NrvMoonBasementMeteorPointObj.Wait,
                                   actor->mIntervalStep);
}

void MoonBasementMeteorPointObj::init(const al::ActorInitInfo& info) {
    using MoonBasementMeteorPointObjFunctor =
        al::FunctorV0M<MoonBasementMeteorPointObj*, void (MoonBasementMeteorPointObj::*)()>;

    al::IUseStageSwitch* stageSwitchUser;

    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::invalidateClipping(this);
    al::initExecutorWatchObj(this, info);
    al::initStageSwitch(this, info);
    al::initNerve(this, &NrvMoonBasementMeteorPointObj.Wait, 0);

    mFallObj = new MoonBasementFallObj("崩落落石");
    al::initCreateActorWithPlacementInfo(mFallObj, info);

    al::getArg(&mIntervalStep, info, "IntervalStep");
    if (al::tryGetArg(&mDelayStep, info, "DelayStep") && mDelayStep >= 1)
        al::setNerve(this, &NrvMoonBasementMeteorPointObj.Delay);

    if (al::isExistLinkChild(info, "FallChangePosition", 0)) {
        al::getChildTrans(&mChangeTrans, info, "FallChangePosition");

        stageSwitchUser = this;
        al::listenStageSwitchOnOff(
            stageSwitchUser, "FallChangePositionSwitch",
            MoonBasementMeteorPointObjFunctor(this, &MoonBasementMeteorPointObj::onChangeTrans),
            MoonBasementMeteorPointObjFunctor(this, &MoonBasementMeteorPointObj::offChangeTrans));
    } else {
        stageSwitchUser = this;
    }

    const MoonBasementMeteorPointObjFunctor onSwitchStartFunctor(
        this, &MoonBasementMeteorPointObj::onSwitchStart);
    const MoonBasementMeteorPointObjFunctor offSwitchStartFunctor(
        this, &MoonBasementMeteorPointObj::offSwitchStart);

    al::listenStageSwitchOnOffStart(stageSwitchUser, onSwitchStartFunctor, offSwitchStartFunctor);
    al::trySyncStageSwitchAppearAndKill(this);
}

void MoonBasementMeteorPointObj::onChangeTrans() {
    mIsUseChangeTrans = true;
}

void MoonBasementMeteorPointObj::offChangeTrans() {
    mIsUseChangeTrans = false;
}

void MoonBasementMeteorPointObj::onSwitchStart() {
    al::invalidateClipping(this);

    if (mDelayStep >= 1) {
        al::setNerve(this, &NrvMoonBasementMeteorPointObj.Delay);
        return;
    }

    al::setNerve(this, &NrvMoonBasementMeteorPointObj.Wait);
}

void MoonBasementMeteorPointObj::offSwitchStart() {
    al::validateClipping(this);
    al::setNerve(this, &Stop);
}

void MoonBasementMeteorPointObj::initAfterPlacement() {
    if (!al::isValidSwitchStart(this))
        return;

    if (al::isOnSwitchStart(this))
        return;

    al::validateClipping(this);
    al::setNerve(this, &Stop);
}

void MoonBasementMeteorPointObj::movement() {
    if (!KoopaHackFunction::isStopKoopaHack(this))
        al::LiveActor::movement();
}

void MoonBasementMeteorPointObj::exeDelay() {
    al::setNerveAtGreaterEqualStep(this, &NrvMoonBasementMeteorPointObj.Wait, mDelayStep);
}

void MoonBasementMeteorPointObj::exeWait() {
    executeWaitNerveImpl(this);
}

void MoonBasementMeteorPointObj::exeStop() {}
