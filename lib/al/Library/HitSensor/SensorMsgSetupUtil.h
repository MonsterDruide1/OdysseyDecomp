#pragma once

#include <prim/seadRuntimeTypeInfo.h>

#include "Library/Base/Macros.h"
#include "Library/LiveActor/ActorSensorFunction.h"

namespace al {
class HitSensor;
class ComboCounter;

class SensorMsg {
    SEAD_RTTI_BASE(SensorMsg);

public:
    virtual ~SensorMsg() = default;
};
}  // namespace al

/*

Declares a SensorMsg class
Creating a SensorMsg class called SenosorMsgTest:
    SENSOR_MSG(Test);

*/

#define SENSOR_MSG(Type)                                                                           \
    class SensorMsg##Type : public al::SensorMsg {                                                 \
        SEAD_RTTI_OVERRIDE(SensorMsg##Type, al::SensorMsg)                                         \
    public:                                                                                        \
        inline SensorMsg##Type() = default;                                                        \
        virtual ~SensorMsg##Type() = default;                                                      \
    };

/*

Declares a SensorMsg class with data.
Creating a SensorMsg class called SenosorMsgTest2 that holds a string:
    SENSOR_MSG_WITH_DATA(Test2, (const char*, str));

*/

#define SENSOR_MSG_WITH_DATA(Type, ...)                                                            \
    class SensorMsg##Type : public al::SensorMsg {                                                 \
        SEAD_RTTI_OVERRIDE(SensorMsg##Type, al::SensorMsg)                                         \
    public:                                                                                        \
        inline SensorMsg##Type(PARAM_LIST_END_COMMA(__VA_ARGS__) void* _ = nullptr) {              \
            SET_MEMEBER_PARAM_MULTI(__VA_ARGS__);                                                  \
        }                                                                                          \
                                                                                                   \
        inline void extractData(POINTER_PARAM_LIST_END_COMMA(__VA_ARGS__) void* _ = nullptr) {     \
            SET_OUT_VAR_MEMEBER_MULTI(__VA_ARGS__);                                                \
        }                                                                                          \
                                                                                                   \
        virtual ~SensorMsg##Type() = default;                                                      \
                                                                                                   \
    private:                                                                                       \
        DECL_MEMBER_VAR_MULTI(__VA_ARGS__);                                                        \
    };

/*

Same as above, but allows adding a custom constructor. This is especially useful for implementing
existing SensorMsgs that store vectors because the ctor for them needs to call `set()` on the vector:

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(MyVecMsg, (sead::Vector3f, Vec))
    inline MyVecMsg(const sead::Vector3f& pVec){
        mVec.set(pVec);
    }
};

*/

#define SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(Type, ...) \
    class SensorMsg##Type : public al::SensorMsg {                                                 \
        SEAD_RTTI_OVERRIDE(SensorMsg##Type, al::SensorMsg)                                          \
    private:                                                                                       \
        DECL_MEMBER_VAR_MULTI(__VA_ARGS__);                                                         \
    public:                                                                                        \
        inline void extractData(POINTER_PARAM_LIST_END_COMMA(__VA_ARGS__) void* _ = nullptr) {     \
            SET_OUT_VAR_MEMEBER_MULTI(__VA_ARGS__);                                                \
        }                                                                                          \
                                                                                                   \
        virtual ~SensorMsg##Type() = default;                                                      

// Use this in the edge cases where there's no macro to implement a specific type of isMsg
#define MSG_TYPE_CHECK_(Type, MsgVar) sead::IsDerivedFrom<SensorMsg##Type>(MsgVar)

// Helper macro passed into FOR_EACH, shouldn't be used directly
#define IS_MSG_MULTIPLE_PART_(_, Type) || MSG_TYPE_CHECK_(Type, msg)

/*

Implements an isMsg function that checkes if the given message is one of multiple message types.
Implementing a function called isMsgTestAll that checks if the given message is of type
SensorMsgTest or SensorMsgTest2: IS_MSG_MULTIPLE_IMPL(TestAll, Test, Test2);

*/

#define IS_MSG_MULTIPLE_IMPL(Name, FirstType, ...)                                                 \
    bool isMsg##Name(const al::SensorMsg* msg) {                                                   \
        return sead::IsDerivedFrom<SensorMsg##FirstType>(msg)                                      \
            FOR_EACH(IS_MSG_MULTIPLE_PART_, _, __VA_ARGS__);                                       \
    }

/*

Implements an isMsg function that checks if the given message is of a specific message type.
Creating a function called isMsgX that checks if the message is of type SensorMsgTest2:
    IS_MSG_IMPL_(X, Test2);

*/

#define IS_MSG_IMPL_(Name, Type)                                                                   \
    bool isMsg##Name(const al::SensorMsg* msg) {                                                   \
        return sead::IsDerivedFrom<SensorMsg##Type>(msg);                                          \
    }

/*

Same as above, but the name of the isMsg function and the SensorMsg are the same.
Creating a function called isMsgTest that checks if the message is of type SensorMsgTest:
    IS_MSG_IMPL(Test);

*/

#define IS_MSG_IMPL(Name) IS_MSG_IMPL_(Name, Name)

/*

Implements a sendMsg function that sends a message of the given type.
Creating a function called sendMsgX that sends a SensorMsgTest2:
    SEND_MSG_IMPL_(X, Test2);

*/

#define SEND_MSG_IMPL_(Name, Type)                                                                 \
    bool sendMsg##Name(al::HitSensor* receiver, al::HitSensor* sender) {                           \
        SensorMsg##Type msg;                                                                       \
        return alActorSensorFunction::sendMsgSensorToSensor(msg, sender, receiver);                \
    }

/*

Implements a sendMsg function that sends a message of the given type to the first sensor of the
actor passed in. Creating a function called sendMsgX that sends a SensorMsgTest2 to the first sensor
of the target actor: SEND_MSG_TO_ACTOR_IMPL(X, Test2);

*/

#define SEND_MSG_TO_ACTOR_IMPL_(Name, Type)                                                        \
    bool sendMsg##Name(al::LiveActor* actor) {                                                     \
        SensorMsg##Type msg;                                                                       \
        return alActorSensorFunction::sendMsgToActorUnusedSensor(msg, actor);                      \
    }

#define SEND_MSG_TO_ACTOR_IMPL(Name) SEND_MSG_TO_ACTOR_IMPL_(Name, Name)

/*

Same as above, but the name of the sendMsg function and the SensorMsg are the same.
Creating a function called sendMsgTest that sends a SensorMsgTest:
    SEND_MSG_IMPL(Test);

*/

#define SEND_MSG_IMPL(Name) SEND_MSG_IMPL_(Name, Name)

/*

Implements a sendMsg function that takes in data of a specific type and sends a message of the given
message type with the data. Creating a function called sendMsgX that takes a const char* and sends a
SensorMsgTest2 with that string: SEND_MSG_DATA_IMPL(X, Test2, const char*);

*/

#define SEND_MSG_DATA_IMPL_(Name, Type, DataType)                                                  \
    bool sendMsg##Name(al::HitSensor* receiver, al::HitSensor* sender, DataType data) {            \
        SensorMsg##Type msg(data);                                                                 \
        return alActorSensorFunction::sendMsgSensorToSensor(msg, sender, receiver);                \
    }

// Same as SEND_MSG_TO_ACTOR_IMPL but also includes data like the macro above
#define SEND_MSG_DATA_TO_ACTOR_IMPL(Name, DataType)                                                \
    bool sendMsg##Name(al::LiveActor* actor, DataType data) {                                      \
        SensorMsg##Name msg(data);                                                                 \
        return alActorSensorFunction::sendMsgToActorUnusedSensor(msg, actor);                      \
    }

// Same as above, but a shorter version
#define SEND_MSG_DATA_IMPL(Name, DataType) SEND_MSG_DATA_IMPL_(Name, Name, DataType)

/*

Similar to SEND_MSG_DATA_IMPL, but for sending a message with multiple different data fields.
Creating a function called sendMsgX that takes a const char* and and a s32 and sends a
SensorMsgTest2 with that string and s32: SEND_MSG_DATA_IMPL(X, Test2, const char*, (s32, Number));

NOTE: all fields after the first one need to be pairs of type and name.

*/

#define SEND_MSG_DATA_MULTI_IMPL_(Name, Type, FirstDataType, ...)                                  \
    bool sendMsg##Name(al::HitSensor* receiver, al::HitSensor* sender,                             \
                       FirstDataType pFirstData PARAM_LIST_START_COMMA(__VA_ARGS__)) {             \
        SensorMsg##Type msg(pFirstData CALL_PARAM_LIST_START_COMMA(__VA_ARGS__));                  \
        return alActorSensorFunction::sendMsgSensorToSensor(msg, sender, receiver);                \
    }

// Same as above, but a shorter version
#define SEND_MSG_DATA_MULTI_IMPL(Name, FirstDataType, ...)                                         \
    SEND_MSG_DATA_MULTI_IMPL_(Name, Name, FirstDataType, __VA_ARGS__)

// Shorter macros for messages that store a ComboCounter (There are 31 of them in al alone)
#define SENSOR_MSG_CBC(Name) SENSOR_MSG_WITH_DATA(Name, (al::ComboCounter*, ComboCounter))
#define SEND_MSG_CBC_IMPL_(Name, Type) SEND_MSG_DATA_IMPL_(Name, Type, al::ComboCounter*)
#define SEND_MSG_CBC_IMPL(Name) SEND_MSG_DATA_IMPL_(Name, Name, al::ComboCounter*)
