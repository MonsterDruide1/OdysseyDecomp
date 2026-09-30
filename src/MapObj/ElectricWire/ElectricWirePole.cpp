#include "MapObj/ElectricWire/ElectricWirePole.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"

#include "MapObj/ElectricWire/CollisionPartsFilterMultiActor.h"

ElectricWirePole::ElectricWirePole(const char* name) : al::LiveActor(name) {}

void ElectricWirePole::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "ElectricWirePole", nullptr);
    makeActorAlive();

    mBaseActor = new al::LiveActor("");
    al::initActorWithArchiveName(mBaseActor, info, "ElectricWirePoleBase", nullptr);
    mBaseActor->makeActorAlive();
    mBaseActor->getName();
}

bool ElectricWirePole::isExistSelfCollisionBeneath(sead::Vector3f* out, const al::LiveActor* actor,
                                                   const sead::Vector3f& pos, f32 startOffset,
                                                   f32 length) {
    sead::Vector3f up;
    al::calcUpDir(&up, actor);
    CollisionPartsFilterMultiActor filter(this, mBaseActor, nullptr);
    const al::IUseCollision* collisionActor = actor;

    sead::Vector3f start = up * startOffset + pos;
    sead::Vector3f dir = -(up * length);

    return alCollisionUtil::getFirstPolyOnArrow(collisionActor, out, nullptr, start, dir, &filter,
                                                nullptr);
}

void ElectricWirePole::initAfterPlacement() {
    al::copyPose(mBaseActor, this);
    const sead::Vector3f& trans = al::getTrans(this);

    sead::Vector3f up;
    al::calcUpDir(&up, this);
    CollisionPartsFilterMultiActor filter(this, mBaseActor, nullptr);
    const al::LiveActor* collisionActorBase = this;
    const al::IUseCollision* collisionActor = collisionActorBase;

    sead::Vector3f start = trans - up * 150.0f;

    sead::Vector3f dir = -(up * 1000.0f);

    sead::Vector3f hitPos;
    if (alCollisionUtil::getFirstPolyOnArrow(collisionActor, &hitPos, nullptr, start, dir, &filter,
                                             nullptr))
        al::resetPosition(mBaseActor, hitPos);
}

void ElectricWirePole::kill() {
    al::LiveActor::kill();
    mBaseActor->kill();
}

void ElectricWirePole::appear() {
    al::LiveActor::appear();
    mBaseActor->appear();
}
