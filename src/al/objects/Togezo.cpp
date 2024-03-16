#include "al/objects/Togezo/Togezo.h"

Togezo::Togezo(const char* name) : al::LiveActor(name) {}

typedef void (Togezo::*functorType)();

/*void Togezo::init(al::ActorInitInfo const& info) {
    al::initActorWithArchiveName(this, info, "Togezo", nullptr);

    if (al::isValidStageSwitch(this, "SwitchStart")) {
        al::initNerve(this, &nrvTogezoWait, 1);
    } else {
        al::initNerve(this, &nrvTogezoWander, 1);
    }

    this->state = new EnemyStateWander(this, "Walk");

    al::initNerveState(this, this->state, &nrvTogezoWander, "徘徊");

    this->forceKeeper = new ExternalForceKeeper();

    if (al::listenStageSwitchOnAppear(
            this, al::FunctorV0M<Togezo*, functorType>(this, &Togezo::listenAppear))) {
        this->makeActorDead();
    } else {
        this->makeActorAlive();
    }
}

void Togezo::listenAppear() {
    this->appear();
}

bool Togezo::receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                        al::HitSensor* target) {}

void Togezo::attackSensor(al::HitSensor* source, al::HitSensor* target) {
    if (!al::isNerve(this, &nrvTogezoBlowDown)) {
        if (!al::sendMsgEnemyAttack(target, source)) {
            rs::sendMsgPushToPlayer(target, source);
            if (al::isNerve(this, &nrvTogezoBlowDown))
                return;
            if (al::isNerve(this, &nrvTogezoBlowDown) || al::isNerve(this, &nrvTogezoAttack) ||
                (!al::sendMsgEnemyAttackNeedle(target, source) &&
                 !al::sendMsgEnemyAttack(target, source))) {
                al::sendMsgPushAndKillVelocityToTarget(this, source, target);
                return;
            }
        }
        al::setNerve(this, &nrvTogezoAttack);
    }
}

// todo: no idea what 0x144 or 0x124 are

void Togezo::control() {
    if (al::isInDeathArea(this) || al::isCollidedFloorCode(this, "DamageFire") ||
        al::isCollidedFloorCode(this, "Needle") || al::isCollidedFloorCode(this, "Poison") ||
        al::isInWaterArea(this)) {
        al::startHitReaction(this, "死亡");
        al::tryAddRippleMiddle(this);
        this->kill();
    } else {
        // this is probably wrong but matches
        int unk = this->unkUInt - 1;
        if(unk > 0) {
            this->unkUInt = unk;
        }

        sead::Vector3f calculatedForce = sead::Vector3f::zero;

        this->forceKeeper->calcForce(&calculatedForce);

        // this mis mismatched pretty much entirely
        
        this->futurePos.y = ((calculatedForce.y * 0.64) + this->futurePos.y) * 0.955;
        this->futurePos.x = ((calculatedForce.x * 0.64) + this->futurePos.x) * 0.955;
        this->futurePos.z = ((calculatedForce.z * 0.64) + this->futurePos.z) * 0.955;

        this->forceKeeper->reset();

        if(al::isNearZero(calculatedForce, 0.001)) {
            this->unkInt = 180;
            al::invalidateClipping(this);
        }

        // this is totally wrong but almost matches
        int unk2 = this->unkInt - 1;
        if(unk2 > 0) {
            this->unkInt = unk2;

            if (unk2 != 0) {
                if(al::isNerve(this, &nrvTogezoWander)) {
                    al::validateClipping(this);
                }
            }
        }
    }
}
*/
void Togezo::updateCollider() {
    sead::Vector3f* velocity = al::getVelocity(this);

    if (al::isNoCollide(this)) {
        sead::Vector3f* transPtr = al::getTransPtr(this);
        *transPtr += *velocity;
        al::Collider* collider = al::getActorCollider(this);
        collider->onInvalidate();
    } else {
        if (al::isFallOrDamageCodeNextMove(this, (*velocity + this->futurePos) * 1.5,
                                           50.0f, 200.0f)) {
            sead::Vector3f* transPtr = al::getTransPtr(this);
            al::Collider* collider = al::getActorCollider(this);
            sead::Vector3f result = collider->collide((*velocity + this->futurePos) * 1.5);
            transPtr->x += result.x;
            transPtr->y += result.y;
            transPtr->z += result.z;
        }else {
            al::Collider* collider = al::getActorCollider(this);
            sead::Vector3f result = collider->collide(
                sead::Vector3f(velocity->x + this->futurePos.x, velocity->y + this->futurePos.y,
                               velocity->z + this->futurePos.z));
            sead::Vector3f* transPtr = al::getTransPtr(this);
            transPtr->x += result.x;
            transPtr->y += result.y;
            transPtr->z += result.z;
        }
    }
}
/*
void Togezo::exeWait(void)  // 0x0
{
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::setVelocityZero(this);
    }
    if (al::isValidStageSwitch(this, "SwitchStart") && al::isOnStageSwitch(this, "SwitchStart")) {
        al::setNerve(this, &nrvTogezoWander);
    }
}

void Togezo::exeWander(void)  // 0x8
{
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Walk");
    }

    al::updateNerveState(this);

    bool isGrounded = al::isOnGround(this, 0);
    bool isNearPlayer = al::isNearPlayer(this, 1000.0f);

    if (isGrounded && isNearPlayer) {
        al::setNerve(this, &nrvTogezoTurn);
    } else if (isGrounded) {
        this->airTime = 0;
        this->groundNormal = *al::getOnGroundNormal(this, 0);
    } else {
        this->airTime++;

        if (this->airTime >= 4) {
            al::setNerve(this, &nrvTogezoFall);
        }
    }
}

// FIXME vector math is non-matching, but seems to be functionally identical.
void Togezo::exeCapHit(void)  // 0x10
{
    sead::Quatf frontUp;

    if (al::isFirstStep(this)) {
        al::startAction(this, "CapHit");
        sead::Vector3f* actorPos = al::getTrans(this);

        sead::Vector3f capDirection =
            sead::Vector3f(actorPos->x - this->capPos.x, 0.0f, actorPos->z - this->capPos.z);

        al::tryNormalizeOrDirZ(&capDirection, capDirection);

        sead::Vector3f forceDir =
            sead::Vector3f(capDirection.x * 20.0f, capDirection.y * 20.0f, capDirection.z * 20.0f);

        al::setVelocity(this, forceDir);

        al::makeQuatUpFront(&frontUp, capDirection, sead::Vector3f::ey);

        this->airTime = 0;

        al::invalidateClipping(this);
    }

    if (al::isActionEnd(this)) {
        if (al::isNearPlayer(this, 1000.0f)) {
            al::setNerve(this, &nrvTogezoFind);
        } else {
            al::setNerve(this, &nrvTogezoWander);
        }
    } else if (al::isOnGround(this, 0)) {
        this->airTime = 0;

        al::addVelocityToGravity(this, 1.0);

        al::scaleVelocity(this, 0.95f);

        sead::Vector3f* velocity = al::getVelocity(this);

        sead::Vector3f unk = sead::Vector3f(velocity->x, 0.0f, velocity->z);

        if (al::tryNormalizeOrZero(&unk, unk)) {
            sead::Vector3f unk2 = sead::Vector3f(unk.x * 10.f, unk.y * 10.f, unk.z * 10.f);

            if (al::isFallOrDamageCodeNextMove(this, unk2, 50.0f, 200.0f)) {
                al::setVelocity(this, sead::Vector3f(unk2.x * 5.0f, unk2.y * 5.0f, unk2.z * 5.0f));
            }
        }

    } else {
        this->airTime++;

        if (this->airTime > 5) {
            al::setNerve(this, &nrvTogezoFall);
        } else {
            al::addVelocityToGravity(this, 1.0);
            al::scaleVelocity(this, 0.98f);
        }
    }
}

void Togezo::exeBlowDown(void)  // 0x18
{
    if (al::isFirstStep(this)) {
        al::startAction(this, "BlowDown");
        al::invalidateClipping(this);
    }

    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.98f);

    if (al::isActionEnd(this)) {
        al::startHitReaction(this, "死亡");
        al::appearItem(this);
        this->kill();
    }
}

void Togezo::exeAttack(void)  // 0x20
{
    if (al::isFirstStep(this)) {
        al::startAction(this, "AttackSuccess");
        al::setVelocityZero(this);
    }

    this->FUNC_18F290();  // theres some sort of function called here thats shared between multiple
                          // different nerves

    if (al::isActionEnd(this)) {
        al::setNerve(this, &nrvTogezoWander);
    }
}

void Togezo::exeTurn(void)  // 0x28
{
    if(al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Turn");
    }
    sead::Vector3f frontDir = sead::Vector3f::zero;
    al::calcFrontDir(&frontDir, this);

    PlayerActorHakoniwa* pActor = al::tryFindNearestPlayerActor(this);
    if(pActor) {
        if(al::isFaceToTargetDegreeH(this, *al::getTrans(pActor), frontDir, 1.0f)) {
            al::setNerve(this, &nrvTogezoFind);
            return;
        }
        al::turnToTarget(this, *al::getTrans(pActor), 3.5f);
    }
    if(!al::isNearPlayer(this, 1300.0f)) {
        al::setNerve(this, &nrvTogezoWander);
        return;
    }
    if(al::isOnGround(this, 0)) {
        this->airTime = 0;
        return;
    }
    al::addVelocityToGravity(this, 1.0f);
    al::scaleVelocity(this, 0.98f);
    if(this->airTime++ >= 4) {
        al::setNerve(this, &nrvTogezoFall);
    }
}

void Togezo::exeFall(void)  // 0x30
{
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::startAction(this, "Fall");
    }

    this->FUNC_18F290();

    if (al::isOnGround(this, 0)) {
        this->airTime = 0;
        al::validateClipping(this);
        al::setNerve(this, &nrvTogezoLand);
    }
}

void Togezo::exeFind(void)  // 0x38
{
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Find");
        this->airTime = 0;
        al::invalidateClipping(this);
    }
    if (al::isOnGround(this, 0) || this->airTime++ >= 4) {
        al::setNerve(this, &nrvTogezoFall);
    } else {
        this->FUNC_18F290();
        if (!al::isActionEnd(this))
            return;
        al::setNerve(this, &nrvTogezoChase);
    }
}

void Togezo::exeChase(void)  // 0x40
{
    if (al::isFirstStep(this)) {
        al::startAction(this, "Run");
        al::invalidateClipping(this);
    }
    if(al::isOnGround(this, 0)) {
        sead::Vector3f* groundNormal = al::getOnGroundNormal(this, 0);
        sead::Vector3f normalXZ;
        normalXZ.z = groundNormal->z;
        normalXZ.x = groundNormal->x;
        al::scaleVelocityDirection(this, normalXZ, 0);
        this->airTime = 0;

        PlayerActorHakoniwa* pActor = al::tryFindNearestPlayerActor(this);
        if(pActor) {
            al::turnToTarget(this, *al::getTrans(pActor), 3.5f);

            sead::Vector3f frontDir = sead::Vector3f::zero;
            al::calcFrontDir(&frontDir, this);
            sead::Vector3f vertical = sead::Vector3f::zero;
            sead::Vector3f horizontal = sead::Vector3f::zero;
            al::separateVectorHV(&vertical, &horizontal, vertical, horizontal);
            al::tryNormalizeOrDirZ(&horizontal, horizontal);
        }
    }
}

void Togezo::exeLand(void)  // 0x48
{
    int* airTimePtr;

    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Land");
        airTimePtr = &this->airTime;
        this->airTime = 0;
    } else {
        airTimePtr = &this->airTime;
    }

    this->FUNC_18F290();

    if (al::isOnGround(this, 0) || (*airTimePtr)++ >= 4) {
        al::setNerve(this, &nrvTogezoWander);
    } else {
        if (!al::isActionEnd(this))
            return;
        al::setNerve(this, &nrvTogezoFall);
    }
}
*/