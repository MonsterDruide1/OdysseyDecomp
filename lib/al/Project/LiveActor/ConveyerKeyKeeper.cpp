#include "Project/LiveActor/ConveyerKeyKeeper.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

namespace al {
void ConveyerKey::init(const sead::Vector3f& keeperTrans, const sead::Vector3f& keeperDir,
                       const PlacementInfo& placement) {
    sead::Vector3f trans;
    tryGetTrans(&trans, placement);
    moveDistance = (trans - keeperTrans).dot(keeperDir);
    totalMoveDistance = 0.0f;

    interpolateType = 0;
    tryGetArg(&interpolateType, placement, "InterpolateType");

    quat.set(sead::Quatf::unit);
    placementInfo = new PlacementInfo(placement);
    tryGetQuat(&quat, placement);
    verticalizeVec(&moveDistanceVertical, keeperDir, trans - keeperTrans);
}

ConveyerKeyKeeper::ConveyerKeyKeeper() = default;

void ConveyerKeyKeeper::init(const ActorInitInfo& info) {
    tryGetQuat(&mQuat, info);
    tryGetTrans(&mTrans, info);
    s32 moveAxis = 2;
    tryGetArg(&moveAxis, info, "MoveAxis");
    tryGetLocalAxis(&mMoveDirection, info, moveAxis);
    mConveyerKeyCount = calcLinkNestNum(info, "KeyMoveNext") + 1;

    mConveyerKeys = new ConveyerKey[mConveyerKeyCount];

    mConveyerKeys[0].init(mTrans, mMoveDirection, *info.placementInfo);

    PlacementInfo linkPlacementSource = *info.placementInfo;
    PlacementInfo linkPlacement;

    for (s32 i = 0; i < mConveyerKeyCount - 1; i++) {
        getLinksInfo(&linkPlacement, linkPlacementSource, "KeyMoveNext");
        mConveyerKeys[i + 1].init(mTrans, mMoveDirection, linkPlacement);
        linkPlacementSource = linkPlacement;
    }

    mTotalMoveDistance = 0.0f;

    for (s32 i = 1; i < mConveyerKeyCount; i++) {
        ConveyerKey* key = &mConveyerKeys[i - 1];
        ConveyerKey* nextKey = &mConveyerKeys[i];

        f32 distance = sead::Mathf::abs(nextKey->moveDistance - key->moveDistance);
        mTotalMoveDistance += distance;
        nextKey->totalMoveDistance = mTotalMoveDistance;
    }
}

// Mismatch: Same as calcPosAndQuatByKeyIndex
// https://decomp.me/scratch/kmQvB
void ConveyerKeyKeeper::calcPosAndQuat(sead::Vector3f* pos, sead::Quatf* quat, s32* index,
                                       f32 coord) const {
    if (coord <= 0.0f) {
        if (pos)
            pos->set(mTrans);

        if (quat)
            quat->set(mQuat);

        if (index)
            *index = -1;
        return;
    }

    if (coord >= mTotalMoveDistance) {
        // Pretty sure this should be a call to calcPosAndQuatByKeyIndex, but the match becomes
        // worse
        const ConveyerKey& key = getConveyerKey(mConveyerKeyCount - 1);
        if (pos)
            pos->set(key.moveDistanceVertical + key.moveDistance * mMoveDirection + mTrans);

        if (quat)
            quat->set(getConveyerKey(mConveyerKeyCount - 1).quat);

        if (index)
            *index = -1;
        return;
    }

    s32 keyIndex = 0;
    for (s32 i = 0; i < mConveyerKeyCount; i++) {
        if (getConveyerKey(i).totalMoveDistance > coord) {
            keyIndex = i;
            break;
        }
    }

    sead::Vector3f moveDistanceVertical = sead::Vector3f::zero;
    sead::Quatf q = sead::Quatf::unit;
    f32 moveDistance;
    if (keyIndex < 1) {
        const ConveyerKey& key = getConveyerKey(0);
        moveDistanceVertical.set(key.moveDistanceVertical);
        q.set(key.quat);
        moveDistance = 0.0f;
    } else {
        const ConveyerKey& key = getConveyerKey(keyIndex);
        const ConveyerKey& prevKey = getConveyerKey(keyIndex - 1);
        sead::Vector3f prevKeyVec = prevKey.moveDistanceVertical;
        sead::Vector3f keyVec = key.moveDistanceVertical;
        f32 totalMoveDistance = key.totalMoveDistance - prevKey.totalMoveDistance;
        f32 t;

        if (isNearZero(totalMoveDistance))
            t = 0.0f;
        else
            t = (coord - prevKey.totalMoveDistance) / totalMoveDistance;

        f32 ease = easeByType(t, getConveyerKey(keyIndex - 1).interpolateType);
        lerpVec(&moveDistanceVertical, prevKeyVec, keyVec, ease);
        moveDistance = lerpValue(getConveyerKey(keyIndex - 1).moveDistance,
                                 getConveyerKey(keyIndex).moveDistance, t);

        sead::Quatf prevKeyQuat = getConveyerKey(keyIndex - 1).quat;
        sead::Quatf keyQuat = getConveyerKey(keyIndex).quat;
        slerpQuat(&q, prevKeyQuat, keyQuat, ease);
    }

    if (pos)
        pos->set(moveDistance * mMoveDirection + mTrans + moveDistanceVertical);

    if (quat)
        quat->set(q);

    if (index)
        *index = keyIndex - 1;
    return;
}

// Mismatch: The math is right, but the registers don't match
// probably inlined (and optimized) in a few other places
// https://decomp.me/scratch/kmQvB
void ConveyerKeyKeeper::calcPosAndQuatByKeyIndex(sead::Vector3f* pos, sead::Quatf* quat,
                                                 s32 index) const {
    const ConveyerKey& key = getConveyerKey(index);
    if (pos)
        pos->set(key.moveDistance * mMoveDirection + mTrans + key.moveDistanceVertical);

    if (quat)
        quat->set(getConveyerKey(index).quat);
}

// Mismatch: Same as calcPosAndQuatByKeyIndex
// https://decomp.me/scratch/kmQvB
void ConveyerKeyKeeper::calcClippingSphere(sead::Vector3f* clippingTrans, f32* clippingRadius,
                                           f32 offset) const {
    if (clippingTrans) {
        const ConveyerKey& key = getConveyerKey(0);
        clippingTrans->set(key.moveDistance * mMoveDirection + mTrans + key.moveDistanceVertical);
    }
    *clippingRadius = offset;

    for (s32 i = 1; i < mConveyerKeyCount; i++) {
        const ConveyerKey& key = getConveyerKey(i);
        sead::Vector3f pos = key.moveDistance * mMoveDirection + mTrans + key.moveDistanceVertical;
        calcSphereMargeSpheres(clippingTrans, clippingRadius, *clippingTrans, *clippingRadius, pos,
                               offset);
    }
}

const ConveyerKey& ConveyerKeyKeeper::getConveyerKey(s32 index) const {
    return mConveyerKeys[index];
}
}  // namespace al
