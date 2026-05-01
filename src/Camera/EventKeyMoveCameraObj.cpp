#include "Camera/EventKeyMoveCameraObj.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Obj/KeyMoveCameraObj.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Util/DemoUtil.h"

EventKeyMoveCameraObjNoDemo::EventKeyMoveCameraObjNoDemo(const char* name) : al::LiveActor(name) {}

void EventKeyMoveCameraObjNoDemo::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTFSV(this);
    al::initActorClipping(this, info);
    al::invalidateClipping(this);
    al::initExecutorWatchObj(this, info);
    al::initStageSwitch(this, info);

    mKeyMoveCameraObj = new al::KeyMoveCameraObj("キー移動カメラオブジェ");
    al::initLinksActor(mKeyMoveCameraObj, info, "NextKey", 0);
    mKeyMoveCameraObj->makeActorDead();

    al::tryGetArg(&mIsPlayManyTimes, info, "IsPlayManyTimes");
    al::tryListenStageSwitchAppear(this);
}

void EventKeyMoveCameraObjNoDemo::makeActorAlive() {
    if (!al::isAlive(this) && !mIsPlayed) {
        al::LiveActor::makeActorAlive();
        mStep = 0;
    }
}

void EventKeyMoveCameraObjNoDemo::kill() {
    al::LiveActor::kill();
    al::tryOnSwitchDeadOn(this);
    mIsPlayed = !mIsPlayManyTimes;
}

void EventKeyMoveCameraObjNoDemo::control() {
    if (mStep <= 0)
        mKeyMoveCameraObj->appear();

    mStep++;

    if (al::isDead(mKeyMoveCameraObj))
        kill();
}

EventKeyMoveCameraObjWithDemo::EventKeyMoveCameraObjWithDemo(const char* name)
    : EventKeyMoveCameraObjNoDemo(name) {}

void EventKeyMoveCameraObjWithDemo::init(const al::ActorInitInfo& info) {
    EventKeyMoveCameraObjNoDemo::init(info);
    al::tryGetStringArg(&mDemoName, info, "DemoName");
}

void EventKeyMoveCameraObjWithDemo::control() {
    if (getStep() < 1 && !rs::isActiveDemo(this)) {
        rs::requestStartDemoNormal(this, false);
        rs::addDemoActor(getKeyMoveCameraObj(), false);

        if (mDemoName)
            rs::setDemoInfoDemoName(this, mDemoName);
    }

    EventKeyMoveCameraObjNoDemo::control();
}

void EventKeyMoveCameraObjWithDemo::kill() {
    EventKeyMoveCameraObjNoDemo::kill();
    rs::requestEndDemoNormal(this);
}
