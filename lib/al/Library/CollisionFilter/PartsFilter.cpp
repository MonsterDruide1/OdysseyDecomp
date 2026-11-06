#include "Library/CollisionFilter/PartsFilter.h"

#include "Library/Base/StringUtil.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFunction.h"

namespace al {

CollisionPartsFilterFunc*
createCollisionPartsFilterFunc(CollisionPartsFilterFunc::DelegateFilter filter) {
    return new CollisionPartsFilterFunc(filter);
}

CollisionPartsFilterFuncSendMsg*
createCollisionPartsFilterFuncSendMsg(CollisionPartsFilterFuncSendMsg::DelegateFilter filter,
                                      HitSensor* sensor) {
    return new CollisionPartsFilterFuncSendMsg(filter, sensor);
}

CollisionPartsFilterConnectedSensor*
createCollisionPartsFilterConnectedSensor(const HitSensor* sensor) {
    return new CollisionPartsFilterConnectedSensor(sensor);
}

bool CollisionPartsFilterActor::isInvalidParts(const CollisionParts& parts) const {
    auto* sensor = parts.getConnectedSensor();
    if (!sensor)
        return false;

    if (mIsCompareEqual)
        return getSensorHost(sensor) == mActor;
    else
        return getSensorHost(sensor) != mActor;
}

static bool isEqualSubActors(const LiveActor* a, const LiveActor* b);

bool CollisionPartsFilterSubActor::isInvalidParts(const CollisionParts& parts) const {
    auto* sensor = parts.getConnectedSensor();
    if (!sensor)
        return false;

    if (getSensorHost(sensor) == mActor)
        return true;

    return isEqualSubActors(getSensorHost(sensor), mActor);
}

static bool isEqualSubActors(const LiveActor* actor, const LiveActor* other) {
    s32 num = getSubActorNum(other);
    for (s32 idx = 0; idx < num; idx++) {
        auto* subActor = getSubActor(other, idx);

        if (subActor == actor)
            return true;

        if (isExistSubActorKeeper(subActor) && isEqualSubActors(actor, subActor))
            return true;
    }

    return false;
}

bool CollisionPartsFilterConnectedSensor::isInvalidParts(const CollisionParts& parts) const {
    auto* sensor = parts.getConnectedSensor();
    if (!sensor)
        return false;

    return sensor == mSensor;
}

bool CollisionPartsFilterSpecialPurpose::isInvalidParts(const CollisionParts& parts) const {
    auto* specialPurpose = parts.getSpecialPurposeName();
    if (!specialPurpose)
        return false;

    return !isEqualString(mSpecialPurpose, specialPurpose);
}

bool CollisionPartsFilterOnlySpecialPurpose::isInvalidParts(const CollisionParts& parts) const {
    auto* specialPurpose = parts.getSpecialPurposeName();
    if (!specialPurpose)
        return true;

    return !isEqualString(mSpecialPurpose, specialPurpose);
}

bool CollisionPartsFilterIgnoreSpecialPurpose::isInvalidParts(const CollisionParts& parts) const {
    return parts.getSpecialPurposeName() != nullptr;
}

bool CollisionPartsFilterIgnoreOptionalPurpose::isInvalidParts(const CollisionParts& parts) const {
    auto* optionalPurpose = parts.getOptionalPurposeName();
    if (!optionalPurpose)
        return false;

    return isEqualString(mOptionalPurpose, optionalPurpose);
}

bool CollisionPartsFilterFunc::isInvalidParts(const CollisionParts& parts) const {
    return mFilter(parts);
}

bool CollisionPartsFilterFuncSendMsg::isInvalidParts(const CollisionParts& parts) const {
    return mFilter(parts, mSensor);
}

}  // namespace al
