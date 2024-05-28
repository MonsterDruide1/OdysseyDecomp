#pragma once

#include <math/seadVector.h>

#include "Library/Execute/ExecuteDirector.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {
class FootPrintServer;

void registerExecutorFunctor(char const*,al::ExecuteDirector *,al::FunctorBase const&);

sead::Vector3f* getTransPtr(al::LiveActor*);

class WaterSurfaceFinder {
public:
    WaterSurfaceFinder(al::LiveActor const*);
private:
    void* size[0x38/8];
};
class FootPrintHolder {
public:
    FootPrintHolder(al::LiveActor *,char const*,al::HitSensor *,al::FootPrintServer *);
public:
    void* size[0x58/8];
};


}
