#pragma once

#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

class Car;

class CarWatcher : public al::LiveActor {
public:
    CarWatcher(const char* name);

    void init(const al::ActorInitInfo& info) override;

private:
    sead::PtrArray<Car> mCars{};
};

static_assert(sizeof(CarWatcher) == 0x118);
