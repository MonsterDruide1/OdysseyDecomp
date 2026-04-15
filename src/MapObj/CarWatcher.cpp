#include "MapObj/CarWatcher.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "MapObj/Car.h"

CarWatcher::CarWatcher(const char* name) : al::LiveActor(name) {}

void CarWatcher::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initExecutorUpdate(this, info, "監視オブジェ");
    al::initActorClipping(this, info);
    al::invalidateClipping(this);

    s32 linkChildNum = al::calcLinkChildNum(info, "LinkCar");
    mCars.allocBuffer(linkChildNum, nullptr, 8);

    for (s32 i = 0; i < linkChildNum; i++) {
        Car* car = new Car("車");
        al::initLinksActor(car, info, "LinkCar", i);
        car->setWatcherArray(&mCars);
        mCars.pushBack(car);
    }

    makeActorDead();
}
