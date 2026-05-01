#include "Camera/EventKeyMoveCameraObjWithDemo.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/Obj/KeyMoveCameraObj.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Util/DemoUtil.h"

EventKeyMoveCameraObjWithDemo::EventKeyMoveCameraObjWithDemo(const char* name)
    : EventKeyMoveCameraObjNoDemo(name, WithDemoCtorTag()) {}

void EventKeyMoveCameraObjWithDemo::init(const al::ActorInitInfo& info) {
    EventKeyMoveCameraObjNoDemo::init(info);
    al::tryGetStringArg(&mDemoName, info, "DemoName");
}

void EventKeyMoveCameraObjWithDemo::control() {
    s32 step = mStep;
    al::KeyMoveCameraObj** keyMoveCameraObj;

    if (step >= 1) {
        keyMoveCameraObj = &mKeyMoveCameraObj;
    } else {
        if (rs::isActiveDemo(this)) {
            keyMoveCameraObj = &mKeyMoveCameraObj;
        } else {
            rs::requestStartDemoNormal(this, false);
            keyMoveCameraObj = &mKeyMoveCameraObj;
            rs::addDemoActor(*keyMoveCameraObj, false);

            if (mDemoName)
                rs::setDemoInfoDemoName(this, mDemoName);
        }

        step = mStep;
        if (step <= 0) {
            (*keyMoveCameraObj)->appear();
            step = mStep;
        }
    }

    mStep = step + 1;

    if (al::isDead(*keyMoveCameraObj))
        kill();
}

void EventKeyMoveCameraObjWithDemo::kill() {
    al::LiveActor::kill();
    al::tryOnSwitchDeadOn(this);
    mIsPlayed = !mIsPlayManyTimes;
    rs::requestEndDemoNormal(this);
}
