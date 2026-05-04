#include "Enemy/SenobiLocalFunction.h"

#include <math/seadVector.h>

#include "Library/Base/StringUtil.h"
#include "Library/Collision/Collider.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Enemy/Senobi.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/IUsePlayerHack.h"
#include "Player/PlayerCollisionCheckSphereMove.h"
#include "Util/Hack.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {

bool isHitWallSensor(al::LiveActor* actor, PlayerCollisionCheckSphereMove* collisionCheck,
                     const sead::Vector3f& move, const char* sensorName) {
    al::HitSensor* sensor = al::getHitSensor(actor, sensorName);
    if (!al::isSensorValid(actor, sensorName))
        return false;

    sead::Vector3f sensorPos;
    sensorPos.set(al::getSensorPos(sensor));
    collisionCheck->checkSphereMove(sensorPos, move, al::getSensorRadius(sensor) * 0.9f);

    f32 minT;
    if (collisionCheck->getNum() != 0) {
        minT = 1.0f;
        u32 i = 0;
        while (true) {
            if (al::isWallPolygon(collisionCheck->getNormal(i), al::getGravity(actor))) {
                f32 t = collisionCheck->getTValue(i);
                if (minT > t)
                    minT = t;
            }
            i++;
            if (i >= collisionCheck->getNum())
                break;
        }
    } else {
        minT = 1.0f;
    }

    return minT < 0.99f;
}

}  // namespace

namespace SenobiLocalFunction {

void setTransToStretchPoint(Senobi* senobi) {
    f32 stretchLength = senobi->getStretchLength();
    if (!(stretchLength <= 0.0f)) {
        sead::Vector3f stretchPoint;
        al::calcJointPos(&stretchPoint, senobi, Senobi::getStretchJointName());

        const sead::Vector3f& localOffset = Senobi::getStretchJointLocalOffset();
        f32 offsetX = localOffset.x;
        f32 offsetY = localOffset.y;
        f32 offsetZ = localOffset.z;
        stretchPoint.x -= offsetX;
        f32 stretchPointY = stretchPoint.y;
        f32 offsetYClamped = offsetY < stretchLength ? offsetY : stretchLength;
        stretchPoint.y = stretchPointY - offsetYClamped;
        stretchPoint.z -= offsetZ;

        al::resetPosition(senobi, stretchPoint);

        sead::Vector3f afterStretchPoint;
        al::calcJointPos(&afterStretchPoint, senobi, Senobi::getStretchJointName());

        f32 diffX = afterStretchPoint.x - stretchPoint.x;
        f32 diffY = afterStretchPoint.y - stretchPoint.y;
        f32 diffZ = afterStretchPoint.z - stretchPoint.z;
        sead::Vector3f trans;
        trans.x = stretchPoint.x - diffX;
        trans.y = stretchPoint.y - diffY;
        trans.z = stretchPoint.z - diffZ;
        al::resetPosition(senobi, trans);
        al::setVelocityZero(senobi);
    }
}

void shrinkBody(Senobi* senobi, f32 rate) {
    const sead::Vector3f& currentTrans = al::getTrans(senobi);
    f32 currentX = currentTrans.x;
    f32 currentY = currentTrans.y;
    f32 currentZ = currentTrans.z;

    sead::Vector3f stretchPoint;
    al::calcJointPos(&stretchPoint, senobi, Senobi::getStretchJointName());

    const sead::Vector3f& localOffset = Senobi::getStretchJointLocalOffset();
    f32 stretchX = stretchPoint.x;
    f32 offsetX = localOffset.x;
    stretchX -= offsetX;
    stretchPoint.x = stretchX;

    f32 stretchY = stretchPoint.y;
    f32 offsetY = localOffset.y;
    stretchY -= offsetY;
    stretchPoint.y = stretchY;

    f32 stretchZ = stretchPoint.z;
    f32 offsetZ = localOffset.z;
    stretchZ -= offsetZ;
    stretchPoint.z = stretchZ;

    sead::Vector3f trans(currentX + (stretchX - currentX) * rate,
                         currentY + (stretchY - currentY) * rate,
                         currentZ + (stretchZ - currentZ) * rate);
    al::resetPosition(senobi, trans);
}

void calcBodyPos(sead::Vector3f* bodyPos, const al::LiveActor* actor) {
    al::calcJointPos(bodyPos, actor, Senobi::getStretchJointName());
}

bool isHitCeil(sead::Vector3f* hitPos, const al::LiveActor* actor,
               PlayerCollisionCheckSphereMove* collisionCheck, const sead::Vector3f& start,
               const sead::Vector3f& end, f32 radius) {
    collisionCheck->checkSphereMove(end, start, radius);

    f32 minT;
    if (collisionCheck->getNum() != 0) {
        minT = 1.0f;
        u32 i = 0;
        while (true) {
            if (al::isCeilingPolygon(collisionCheck->getNormal(i), al::getGravity(actor))) {
                f32 t = collisionCheck->getTValue(i);
                if (minT > t) {
                    hitPos->set(collisionCheck->getPos(i));
                    minT = t;
                }
            }
            i++;
            if (i >= collisionCheck->getNum())
                break;
        }
    } else {
        minT = 1.0f;
    }

    return minT < 0.99f;
}

bool isHitWall(sead::Vector3f* normal, al::LiveActor* actor,
               PlayerCollisionCheckSphereMove* collisionCheck, const sead::Vector3f& move) {
    if (al::isNearZero(move, 0.001f))
        return false;

    if (!isHitWallSensor(actor, collisionCheck, move, "Body")) {
        if (Senobi::getStretchSensorNum() < 1)
            return false;

        s32 i = 0;
        bool isHit = false;
        while (true) {
            if (i != 0) {
                al::StringTmp<32> sensorName("Stretch%02d", i);
                bool isHitSensor = isHitWallSensor(actor, collisionCheck, move, sensorName.cstr());
                isHit |= isHitSensor;
                if (isHitSensor)
                    break;
            }
            i++;
            if (i >= Senobi::getStretchSensorNum())
                break;
        }

        if (!isHit)
            return false;
    }

    if (normal != nullptr)
        normal->set(collisionCheck->getNormal(0));

    return true;
}

void invalidateAllStretchSensors(al::LiveActor* actor) {
    for (s32 i = 0; i < Senobi::getStretchSensorNum(); i++) {
        al::StringTmp<32> sensorName("Stretch%02d", i);
        al::invalidateHitSensor(actor, sensorName.cstr());
    }
}

void updateStretchSensors(al::LiveActor* actor, f32 stretchLength) {
    for (s32 i = 0; i < Senobi::getStretchSensorNum();) {
        al::StringTmp<32> sensorName("Stretch%02d", i);
        f32 radius = al::getSensorRadius(actor, sensorName.cstr());

        i++;
        f32 sensorLength = i * radius;
        sensorLength += sensorLength;

        bool isValid = al::isSensorValid(actor, sensorName.cstr());
        if (sensorLength < stretchLength) {
            if (!isValid)
                al::validateHitSensor(actor, sensorName.cstr());
        } else {
            if (isValid)
                al::invalidateHitSensor(actor, sensorName.cstr());
        }
    }
}

bool isHoldStretchButton(const IUsePlayerHack* playerHack) {
    return rs::isHoldHackAnyButton(playerHack);
}

bool trySendMsgToLandingGround(al::LiveActor* actor, IUsePlayerCollision* collision) {
    if (!rs::isOnGround(actor, collision))
        return false;

    al::HitSensor* groundSensor = rs::tryGetCollidedGroundSensor(collision);
    if (groundSensor == nullptr)
        return false;

    return rs::sendMsgSenobiTrample(groundSensor, al::getHitSensor(actor, "PlayerFoot"));
}

al::LiveActor* getLeaf(const al::LiveActor* actor, s32 index) {
    return al::getSubActor(actor, index + 1);
}

s32 calcLeavesNum(const al::LiveActor* actor) {
    if (al::getSubActorNum(actor) < 1)
        return 0;

    s32 i = 0;
    s32 numLeaves = 0;
    while (true) {
        numLeaves += al::isEqualString(al::getModelName(al::getSubActor(actor, i)), "SenobiLeaf");
        i++;
        if (i >= al::getSubActorNum(actor))
            break;
    }

    return numLeaves;
}

bool tryTurnToPlayer(al::LiveActor* actor, f32 turnDegree) {
    sead::Vector3f playerPos = sead::Vector3f::zero;
    al::tryFindNearestPlayerPos(&playerPos, actor);
    return al::turnToTarget(actor, playerPos, turnDegree);
}

void hideSenobi(Senobi* senobi) {
    al::hideModelIfShow(senobi);
    al::hideSilhouetteModelIfShow(senobi);

    IUsePlayerHack* playerHack = senobi->getPlayerHack();
    if (playerHack != nullptr)
        rs::hideHackCap(playerHack);
}

void showSenobi(Senobi* senobi) {
    al::showModelIfHide(senobi);
    al::showSilhouetteModelIfHide(senobi);

    IUsePlayerHack* playerHack = senobi->getPlayerHack();
    if (playerHack != nullptr)
        rs::showHackCap(playerHack);
}

f32 getSenobiStretchLength(const Senobi* senobi) {
    return senobi->getStretchLength();
}

void setSenobiStretchLength(Senobi* senobi, f32 stretchLength) {
    senobi->setStretchLength(stretchLength);
}

}  // namespace SenobiLocalFunction
