#include "Player/PlayerActorBase.h"

#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Se/SeFunction.h"

#include "Util/ObjUtil.h"
#include "Util/SensorMsgFunction.h"

PlayerActorBase::PlayerActorBase(const char* name) : LiveActor(name) {}

u32 PlayerActorBase::getPortNo() const {
    return mPortNo;
}

sead::Matrix34f* PlayerActorBase::getViewMtx() const {
    return mViewMtx;
}

void PlayerActorBase::movement() {
    al::LiveActor::movement();
    if (!checkDeathArea())
        sendCollisionMsg();
}

bool PlayerActorBase::checkDeathArea() {
    if (!al::isInDeathArea(this))
        return false;

    al::tryStartSe(this, "FallDown");
    al::tryStartSe(this, "vDeadFallDown");
    kill();
    return true;
}

void PlayerActorBase::sendCollisionMsg() {
    if (!al::isExistActorCollider(this))
        return;

    al::HitSensor* body = al::getHitSensor(this, "Body");
    if (!body)
        return;

    rs::sendPlayerCollisionTouchMsg(this, body, getPlayerCollision());
}

bool PlayerActorBase::receivePushMsg(const al::SensorMsg* msg, al::HitSensor* other,
                                     al::HitSensor* self, f32 maxPush) {
    if (al::isMsgPush(msg) || al::isMsgPushStrong(msg) || al::isMsgPushVeryStrong(msg) ||
        rs::isMsgPushToPlayer(msg)) {
        f32 combinedRadius = al::getSensorRadius(self) + al::getSensorRadius(other);
        sead::Vector3f diff = al::getSensorPos(self) - al::getSensorPos(other);
        if (diff.squaredLength() > sead::Mathf::square(combinedRadius))
            return true;

        al::verticalizeVec(&diff, al::getGravity(this), diff);
        f32 push = sead::Mathf::clampMax(combinedRadius - diff.length(), maxPush);

        if (!al::tryNormalizeOrZero(&diff)) {
            al::calcFrontDir(&diff, this);
            diff = -diff;
        }
        diff = push * diff;
        al::setTrans(this, al::getTrans(this) + diff);
        return true;
    }
    return false;
}
