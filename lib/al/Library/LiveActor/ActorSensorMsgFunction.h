#pragma once

#include <math/seadVector.h>

// TODO: This defines the class but the sead decomp doesn't have anything inside the RTTI functions,
// causing the functions in the vtable to be exported
#define SENSOR_MSG(Type)                                                                           \
    class SensorMsg##Type : public al::SensorMsg {                                                 \
        SEAD_RTTI_OVERRIDE(SensorMsg##Type, al::SensorMsg)                                         \
    };                                                                                             \
    bool isMsg##Type(const al::SensorMsg* msg) {                                                   \
        return SensorMsg##Type::checkDerivedRuntimeTypeInfoStatic(msg->getRuntimeTypeInfo());      \
    }

namespace al {
class SensorMsg {
    SEAD_RTTI_BASE(SensorMsg);
};

class HitSensor;
class ComboCounter;
class LiveActor;
class SensorMsg;

}  // namespace al
