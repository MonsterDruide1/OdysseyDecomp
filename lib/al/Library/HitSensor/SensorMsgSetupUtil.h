#pragma once

#include <prim/seadRuntimeTypeInfo.h>

#include "Library/Base/Macros.h"
#include "Library/LiveActor/ActorSensorFunction.h"

namespace al {
class HitSensor;
class ComboCounter;

class SensorMsg {
    SEAD_RTTI_BASE(SensorMsg)

public:
    virtual ~SensorMsg() = default;
};
}  // namespace al

/*

Declares a SensorMsg class
Creating a SensorMsg class called SensorMsgTest:
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
Creating a SensorMsg class called SensorMsgTest2 that holds a string referenced as `mStr` or `pStr`:
    SENSOR_MSG_WITH_DATA(Test2, (const char*, Str));

*/

#define SENSOR_MSG_WITH_DATA(Type, ...)                                                            \
    class SensorMsg##Type : public al::SensorMsg {                                                 \
        SEAD_RTTI_OVERRIDE(SensorMsg##Type, al::SensorMsg)                                         \
    public:                                                                                        \
        inline SensorMsg##Type(PARAM_LIST(__VA_ARGS__)) {                                          \
            SET_MEMBER_PARAM_MULTI(__VA_ARGS__);                                                   \
        }                                                                                          \
                                                                                                   \
        DECL_GET_MULTI(__VA_ARGS__)                                                                \
                                                                                                   \
        virtual ~SensorMsg##Type() = default;                                                      \
                                                                                                   \
    private:                                                                                       \
        DECL_MEMBER_VAR_MULTI(__VA_ARGS__);                                                        \
    };

/*

Same as above, but allows adding a custom constructor. The second argument is a list of member
variables the message should store and the third argument is a list of arguments to the constructor.
This is especially useful for implementing existing SensorMsgs that store vectors because the ctor
for them needs to call `set()` on the vector:

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(MyVecMsg, ((sead::Vector3f, Vec)), ((const sead::Vector3f&, Vec)))
{ mVec.set(pVec);
};

*/

#define SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(Type, SensorMsgParams, CtorParams)                        \
    class SensorMsg##Type : public al::SensorMsg {                                                 \
        SEAD_RTTI_OVERRIDE(SensorMsg##Type, al::SensorMsg)                                         \
                                                                                                   \
    public:                                                                                        \
        inline SensorMsg##Type(PARAM_LIST CtorParams);                                             \
                                                                                                   \
        DECL_GET_MULTI CtorParams;                                                                 \
                                                                                                   \
        virtual ~SensorMsg##Type() = default;                                                      \
                                                                                                   \
    private:                                                                                       \
        DECL_MEMBER_VAR_MULTI SensorMsgParams;                                                     \
    };                                                                                             \
    inline SensorMsg##Type::SensorMsg##Type(PARAM_LIST CtorParams)

/*

Same as above, but returns the actual stored field types from the getters
instead of the constructor parameter types. Useful when the stored data
types differ from the constructor argument types

*/

#define SENSOR_MSG_WITH_DATA_CUSTOM_CTOR_DIRECT_GETTERS(Type, SensorMsgParams, CtorParams)         \
    class SensorMsg##Type : public al::SensorMsg {                                                 \
        SEAD_RTTI_OVERRIDE(SensorMsg##Type, al::SensorMsg)                                         \
                                                                                                   \
    public:                                                                                        \
        inline SensorMsg##Type(PARAM_LIST CtorParams);                                             \
                                                                                                   \
        DECL_GET_MULTI SensorMsgParams;                                                            \
                                                                                                   \
        virtual ~SensorMsg##Type() = default;                                                      \
                                                                                                   \
    private:                                                                                       \
        DECL_MEMBER_VAR_MULTI SensorMsgParams;                                                     \
    };                                                                                             \
    inline SensorMsg##Type::SensorMsg##Type(PARAM_LIST CtorParams)

// Use this in the edge cases where there's no macro to implement a specific type of isMsg
#define MSG_TYPE_CHECK_(MsgVar, Type) sead::IsDerivedFrom<SensorMsg##Type>(MsgVar)

/*

Implements an isMsg function that checkes if the given message is one of multiple message types.
Implementing a function called isMsgTestAll that checks if the given message is of type
SensorMsgTest or SensorMsgTest2: IS_MSG_MULTIPLE_IMPL(TestAll, Test, Test2);

*/

#define IS_MSG_MULTIPLE_IMPL(Name, ...)                                                            \
    bool isMsg##Name(const al::SensorMsg* msg) {                                                   \
        return FOR_EACH_DELIM(MSG_TYPE_CHECK_, LOGICAL_OR, msg, __VA_ARGS__);                      \
    }

/*

Implements an isMsg function that checks if the given message is of a specific message type.
Creating a function called isMsgX that checks if the message is of type SensorMsgTest2:
    IS_MSG_IMPL_(X, Test2);

*/

#define IS_MSG_IMPL_(Name, Type)                                                                   \
    bool isMsg##Name(const al::SensorMsg* msg) {                                                   \
        return MSG_TYPE_CHECK_(msg, Type);                                                         \
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

// Same as above, but a shorter version
#define SEND_MSG_IMPL(Name) SEND_MSG_IMPL_(Name, Name)

/*

Implements a sendMsg function that sends a message of the given type to the first sensor of the
actor passed in.
Creating a function called sendMsgX that sends a SensorMsgTest2 to the first sensor
of the target actor:
    SEND_MSG_TO_ACTOR_IMPL(X, Test2);

*/

#define SEND_MSG_TO_ACTOR_IMPL_(Name, Type)                                                        \
    bool sendMsg##Name(al::LiveActor* actor) {                                                     \
        SensorMsg##Type msg;                                                                       \
        return alActorSensorFunction::sendMsgToActorUnusedSensor(msg, actor);                      \
    }

#define SEND_MSG_TO_ACTOR_IMPL(Name) SEND_MSG_TO_ACTOR_IMPL_(Name, Name)

// Same as SEND_MSG_TO_ACTOR_IMPL but also includes data like the macro above
#define SEND_MSG_DATA_TO_ACTOR_IMPL(Name, DataType, DataName)                                      \
    bool sendMsg##Name(al::LiveActor* actor, DataType DataName) {                                  \
        SensorMsg##Name msg(DataName);                                                             \
        return alActorSensorFunction::sendMsgToActorUnusedSensor(msg, actor);                      \
    }

/*

Implements a sendMsg function that takes in any number data variables and sends a message of the
given message type with the data. Creating a function called sendMsgX that takes a const char* and
and a s32 and sends a SensorMsgTest2 with that string and s32: SEND_MSG_DATA_IMPL(X, Test2, (const
char*, String), (s32, Number));

*/

#define SEND_MSG_DATA_IMPL_(Name, Type, ...)                                                       \
    bool sendMsg##Name(al::HitSensor* receiver, al::HitSensor* sender, PARAM_LIST(__VA_ARGS__)) {  \
        SensorMsg##Type msg(CALL_PARAM_LIST(__VA_ARGS__));                                         \
        return alActorSensorFunction::sendMsgSensorToSensor(msg, sender, receiver);                \
    }

// Same as above, but a shorter version
#define SEND_MSG_DATA_IMPL(Name, ...) SEND_MSG_DATA_IMPL_(Name, Name, __VA_ARGS__)

// Shorter macros for messages that store a ComboCounter (There are 31 of them in al alone)
#define SENSOR_MSG_COMBO(Name) SENSOR_MSG_WITH_DATA(Name, (al::ComboCounter*, ComboCounter))
#define SEND_MSG_COMBO_IMPL_(Name, Type)                                                           \
    SEND_MSG_DATA_IMPL_(Name, Type, (al::ComboCounter*, ComboCounter))
#define SEND_MSG_COMBO_IMPL(Name) SEND_MSG_DATA_IMPL_(Name, Name, (al::ComboCounter*, ComboCounter))
