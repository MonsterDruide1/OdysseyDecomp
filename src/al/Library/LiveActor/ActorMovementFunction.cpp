#include <math/seadVector.h>
#include "al/LiveActor/ActorPoseKeeper.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/MathUtil.h"

namespace al {

float calcDistance(const al::LiveActor* l1, const al::LiveActor* l2) {
    return calcDistance(l1, getTrans(l2));
}

float calcDistance(const al::LiveActor* l1, const sead::Vector3f& vec) {
    sead::Vector3f distance = al::getTrans(l1) - vec;
    return distance.length();
}

float calcSpeed(const al::LiveActor* actor) {
    return actor->mPoseKeeper->getVelocity().length();
}

float calcSpeedH(const al::LiveActor* actor) {
    sead::Vector3f verticalized;
    al::verticalizeVec(&verticalized, al::getGravity(actor), actor->mPoseKeeper->getVelocity());
    return verticalized.length();
}

float calcSpeedV(const al::LiveActor* actor) {
    return -actor->mPoseKeeper->getVelocity().dot(al::getGravity(actor));
}

}  // namespace al
