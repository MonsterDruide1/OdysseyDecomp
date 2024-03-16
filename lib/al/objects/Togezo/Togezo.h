#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/async/FunctorV0M.h"
#include "al/player/PlayerActorHakoniwa.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/MathUtil.h"
#include "al/util/NerveUtil.h"

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <heap/seadHeap.h>
#include <math/seadVector.h>

namespace al {
sead::Vector3f* getVelocity(const al::LiveActor*);
Collider* getActorCollider(const al::LiveActor*);
bool isNoCollide(const al::LiveActor*);
bool isFallOrDamageCodeNextMove(const al::LiveActor*, const sead::Vector3f&, float, float);

class Collider {
public:
    sead::Vector3f collide(const sead::Vector3f&);
    void onInvalidate();
};
}  // namespace al

class ExternalForceKeeper;
class EnemyStateWander;

class Togezo : public al::LiveActor {
public:
    Togezo(char const* name);
    void init(al::ActorInitInfo const& info);
    void listenAppear();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target);
    void attackSensor(al::HitSensor* source, al::HitSensor* target);
    void control();
    void updateCollider();

    void FUNC_18F290();  // unknown symbol

    void exeWait(void);
    void exeWander(void);
    void exeTurn(void);
    void exeFind(void);
    void exeChase(void);
    void exeFall(void);
    void exeLand(void);
    void exeAttack(void);
    void exeCapHit(void);
    void exeBlowDown(void);

    ExternalForceKeeper* forceKeeper = nullptr;          // 0x108
    EnemyStateWander* state = nullptr;                   // 0x110
    sead::Vector3f capPos = sead::Vector3f::zero;        // 0x118
    unsigned int unkUInt = 0;                            // 0x124
    int airTime = 0;                                     // 0x128
    sead::Vector3f futurePos = sead::Vector3f::zero;     // 0x12C
    sead::Vector3f groundNormal = sead::Vector3f::zero;  // 0x138
    unsigned int unkInt = 0;                             // 0x144
};

namespace {
NERVE_HEADER(Togezo, Wait)
NERVE_HEADER(Togezo, Wander)
NERVE_HEADER(Togezo, Turn)
NERVE_HEADER(Togezo, Find)
NERVE_HEADER(Togezo, Chase)
NERVE_HEADER(Togezo, Fall)
NERVE_HEADER(Togezo, Land)
NERVE_HEADER(Togezo, Attack)
NERVE_HEADER(Togezo, CapHit)
NERVE_HEADER(Togezo, BlowDown)
}  // namespace
