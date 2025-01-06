#include "Util/ExternalForceKeeper.h"

#include "Util/SensorMsgFunction.h"

ExternalForceKeeper::ExternalForceKeeper() {}

bool ExternalForceKeeper::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                     al::HitSensor* self) {
    if (rs::isMsgByugoBlow(message)) {
        sead::Vector3f force = sead::Vector3f::zero;
        rs::tryGetByugoBlowForce(&force, message);

        mMinForce.x = mMinForce.x < force.x ? mMinForce.x : force.x;
        mMinForce.y = mMinForce.y < force.y ? mMinForce.y : force.y;
        mMinForce.z = mMinForce.z < force.z ? mMinForce.z : force.z;

        mMaxForce.x = mMaxForce.x > force.x ? mMaxForce.x : force.x;
        mMaxForce.y = mMaxForce.y > force.y ? mMaxForce.y : force.y;
        mMaxForce.z = mMaxForce.z > force.z ? mMaxForce.z : force.z;

        return true;
    }

    if (rs::isMsgAirExplosion(message)) {
        sead::Vector3f force = sead::Vector3f::zero;
        rs::tryGetAirExplosionForce(&force, message);

        mMinForce.x = mMinForce.x < force.x ? mMinForce.x : force.x;
        mMinForce.y = mMinForce.y < force.y ? mMinForce.y : force.y;
        mMinForce.z = mMinForce.z < force.z ? mMinForce.z : force.z;

        mMaxForce.x = mMaxForce.x > force.x ? mMaxForce.x : force.x;
        mMaxForce.y = mMaxForce.y > force.y ? mMaxForce.y : force.y;
        mMaxForce.z = mMaxForce.z > force.z ? mMaxForce.z : force.z;

        return true;
    }

    return false;
}

void ExternalForceKeeper::reset() {
    mMinForce.set(sead::Vector3f::zero);
    mMaxForce.set(sead::Vector3f::zero);
}

void ExternalForceKeeper::calcForce(sead::Vector3f* force) const {
    force->x = mMinForce.x + mMaxForce.x;
    force->y = mMinForce.y + mMaxForce.y;
    force->z = mMinForce.z + mMaxForce.z;
}
