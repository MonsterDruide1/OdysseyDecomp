#include "Library/LiveActor/ActorParamHolderUtil.h"

#include "Library/LiveActor/ActorParamHolder.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

static ActorParamS32 gParamS32;
static ActorParamF32 gParamF32;
static ActorParamMove gParamMove;
static ActorParamJump gParamJump;
static ActorParamSight gParamSight;
static ActorParamRebound gParamRebound;

ActorParamF32* findActorParamF32(const LiveActor* actor, const char* paramName) {
    if (actor->getActorParamHolder())
        return actor->getActorParamHolder()->findParamF32(paramName);

    return &gParamF32;
}

ActorParamS32* findActorParamS32(const LiveActor* actor, const char* paramName) {
    if (actor->getActorParamHolder())
        return actor->getActorParamHolder()->findParamS32(paramName);

    return &gParamS32;
}

ActorParamMove* findActorParamMove(const LiveActor* actor, const char* paramName) {
    if (actor->getActorParamHolder())
        return actor->getActorParamHolder()->findParamMove(paramName);

    return &gParamMove;
}

ActorParamJump* findActorParamJump(const LiveActor* actor, const char* paramName) {
    if (actor->getActorParamHolder())
        return actor->getActorParamHolder()->findParamJump(paramName);

    return &gParamJump;
}

ActorParamSight* findActorParamSight(const LiveActor* actor, const char* paramName) {
    if (actor->getActorParamHolder())
        return actor->getActorParamHolder()->findParamSight(paramName);

    return &gParamSight;
}

ActorParamRebound* findActorParamRebound(const LiveActor* actor, const char* paramName) {
    if (actor->getActorParamHolder())
        return actor->getActorParamHolder()->findParamRebound(paramName);

    return &gParamRebound;
}

void setActorParamMove(ActorParamMove* paramMove, f32 moveAccel, f32 gravity, f32 moveFriction,
                       f32 turnSpeedDegree) {
    paramMove->moveAccel = moveAccel;
    paramMove->gravity = gravity;
    paramMove->moveFriction = moveFriction;
    paramMove->turnSpeedDegree = turnSpeedDegree;
}

}  // namespace al
