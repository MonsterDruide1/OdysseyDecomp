#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class Resource;

enum class ActorParamType : s32 {
    None = -1,
    S32 = 0,
    F32 = 1,
    Move = 2,
    Jump = 3,
    Sight = 4,
    Rebound = 5,
};

struct ActorParamS32 {
    s32 value;
};

static_assert(sizeof(ActorParamS32) == 0x4);

struct ActorParamF32 {
    f32 value;
};

static_assert(sizeof(ActorParamF32) == 0x4);

struct ActorParamMove {
    f32 moveAccel;
    f32 gravity;
    f32 moveFriction;
    f32 turnSpeedDegree;
};

static_assert(sizeof(ActorParamMove) == 0x10);

struct ActorParamJump {
    f32 speedFront;
    f32 speedUp;
};

static_assert(sizeof(ActorParamJump) == 0x8);

struct ActorParamSight {
    f32 distance;
    f32 degreeH;
    f32 degreeV;
};

static_assert(sizeof(ActorParamSight) == 0xc);

struct ActorParamRebound {
    f32 reboundRate;
    f32 speedMinToRebound;
    f32 frictionH;
};

static_assert(sizeof(ActorParamRebound) == 0xc);

struct ActorParamInfo {
    ActorParamInfo();

    const char* name = nullptr;
    ActorParamType type = ActorParamType::None;

    union {
        ActorParamS32 paramS32 = {.value = 0};
        ActorParamF32 paramF32;
        ActorParamMove* paramMove;
        ActorParamJump* paramJump;
        ActorParamSight* paramSight;
        ActorParamRebound* paramRebound;
    };
};

static_assert(sizeof(ActorParamInfo) == 0x18);

class ActorParamHolder {
public:
    static const char* getYamlName();
    static ActorParamHolder* tryCreate(LiveActor* actor, const Resource* resource,
                                       const char* suffix);

    // NOTE: for all of these: if the name and type of `findParam` do not align,
    // it will just re-cast the memory and potentially return garbage/crash

    ActorParamS32* findParamS32(const char* name) const;
    ActorParamF32* findParamF32(const char* name) const;
    ActorParamMove* findParamMove(const char* name) const;
    ActorParamJump* findParamJump(const char* name) const;
    ActorParamSight* findParamSight(const char* name) const;
    ActorParamRebound* findParamRebound(const char* name) const;

private:
    ActorParamHolder(LiveActor* actor, const Resource* resource, const char* suffix);
    ActorParamInfo* tryFindParamInfoByName(const char* name) const;

    s32 mSize = 0;
    ActorParamInfo* mInfoArray = nullptr;
};

static_assert(sizeof(ActorParamHolder) == 0x10);

}  // namespace al
