#include "Demo/StageEventDemoBase.h"

#include "Library/Base/StringUtil.h"
#include "Library/Demo/DemoActorFunction.h"
#include "Library/Demo/DemoActorHolder.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Demo/StageEventDemoFunction.h"
#include "System/GameDataUtil.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_IMPL(StageEventDemo, Wait)
NERVE_IMPL(StageEventDemo, Demo)
NERVE_IMPL(StageEventDemo, End)
NERVES_MAKE_NOSTRUCT(StageEventDemo, Wait, Demo, End)
}  // namespace

StageEventDemo::StageEventDemo(const char* name) : StageEventDemoBase(name) {}

void StageEventDemo::init(const al::ActorInitInfo& info) {
    if (al::isObjectName(info, "StageEventDemoNoSave"))
        mIsSaveDemo = false;

    StageEventDemoFunction::initStageEventDemoBase(this, info);
    al::initNerve(this, &Wait, 0);
    al::getStringArg(&mDemoStageName, info, "DemoStageName");

    if (al::isEqualString(mDemoStageName, "DemoStartWorldWaterfallStage"))
        mIsEnableSkipDemo = true;

    using StartFunctor = al::FunctorV0M<StageEventDemo*, void (StageEventDemo::*)()>;
    if (al::listenStageSwitchOnStart(this, StartFunctor(this, &StageEventDemo::startDemo)))
        mIsStageStartDemo = false;

    mSaveObjInfo = rs::createSaveObjInfoWriteSaveData(info);
    if (mIsSaveDemo && rs::isOnSaveObjInfo(mSaveObjInfo)) {
        mIsStageStartDemo = false;
        mIsEnableStartDemo = false;
    } else {
        mDemoActorHolder =
            al::alDemoFunction::createDemoActorHolder(mDemoStageName, info, nullptr, 1, false);
        mAddDemoInfo = al::registDemoRequesterToAddDemoInfo(this, info, 0);
    }

    makeActorDead();
}

void StageEventDemo::startDemo() {
    if (!mIsEnableStartDemo)
        return;

    if (!al::isNerve(this, &Wait)) {
        isStageStartDemo();
        return;
    }

    al::LiveActor::appear();

    if (isStageStartDemo()) {
        if (!rs::requestStartDemoNormalWithCinemaFrame(this))
            return;

        if (mIsEnableSkipDemo)
            rs::requestValidateDemoSkip(this, this);
    } else {
        rs::requestStartDemoNormal(this, false);
    }

    al::addDemoActorFromDemoActorHolder(this, mDemoActorHolder);
    al::addDemoActorFromAddDemoInfo(this, mAddDemoInfo);
    mDemoActorHolder->startSequence();
    al::setNerve(this, &Demo);
}

void StageEventDemo::endDemo() {
    mIsDemoLastStep = true;
    al::setNerve(this, &End);
}

bool StageEventDemo::isDemoLastStep() const {
    return mIsDemoLastStep;
}

bool StageEventDemo::isEndDemo() const {
    return al::isDead(this);
}

bool StageEventDemo::isFirstDemo() const {
    if (!mIsSaveDemo)
        return false;

    return !rs::isOnSaveObjInfo(mSaveObjInfo);
}

bool StageEventDemo::isEnableSkipDemo() const {
    return mIsEnableSkipDemo;
}

void StageEventDemo::skipDemo() {
    endDemoCore();
}

void StageEventDemo::endDemoCore() {
    if (isStageStartDemo())
        rs::requestEndDemoNormalWithCinemaFrame(this);
    else
        rs::requestEndDemoNormal(this);

    if (mIsSaveDemo)
        rs::onSaveObjInfo(mSaveObjInfo);

    mIsDemoLastStep = false;
    mDemoActorHolder->kill();
    al::tryOnStageSwitch(this, "SwitchDemoEndOn");
    kill();
}

void StageEventDemo::updateOnlyDemoGraphics() {
    if (mDemoActorHolder != nullptr && mDemoActorHolder->isAlive())
        mDemoActorHolder->updateGraphics();
}

void StageEventDemo::exeWait() {}

void StageEventDemo::exeDemo() {
    al::isFirstStep(this);
    mDemoActorHolder->updateSequence();

    if (mDemoActorHolder->isEndSequence())
        endDemo();
}

void StageEventDemo::exeEnd() {
    if (al::isFirstStep(this))
        endDemoCore();
}

bool StageEventDemo::isStageStartDemo() const {
    return mIsStageStartDemo;
}

__attribute__((noinline)) StageEventDemoBase::StageEventDemoBase(const char* name)
    : al::LiveActor(name) {}
