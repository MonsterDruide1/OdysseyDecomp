#include "Project/LiveActor/ConveyerKeyKeeper.h"

#include "Library/LiveActor/ActorInitInfo.h"

namespace al {
ConveyerKeyKeeper::ConveyerKeyKeeper() {}

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

        f32 distance = sead::Mathf::abs(nextKey->_0 - key->_0);
        mTotalMoveDistance += distance;
        nextKey->_4 = mTotalMoveDistance;
    }
}

// Mismatch: Same as calcPosAndQuatByKeyIndex
// https://decomp.me/scratch/kmQvB
void ConveyerKeyKeeper::calcPosAndQuat(sead::Vector3f* pos, sead::Quatf* quat, s32* index,
                                       f32 coord) const {
    if (coord <= 0.0f) {
        if (pos != nullptr)
            pos->set(mTrans);

        if (quat != nullptr)
            quat->set(mQuat);

        if (index)
            *index = -1;
        return;
    }

    if (coord >= mTotalMoveDistance) {
        // Pretty sure this should be a call to calcPosAndQuatByKeyIndex, but the match becomes
        // worse
        const ConveyerKey& key = getConveyerKey(mConveyerKeyCount - 1);
        if (pos != nullptr)
            pos->set(key._18 + key._0 * mMoveDirection + mTrans);

        if (quat != nullptr)
            quat->set(getConveyerKey(mConveyerKeyCount - 1)._8);

        if (index)
            *index = -1;
        return;
    }

    s32 keyIndex = 0;
    for (s32 i = 0; i < mConveyerKeyCount; i++) {
        if (coord < getConveyerKey(i)._4) {
            keyIndex = i;
            break;
        }
    }

    sead::Vector3f v = sead::Vector3f::zero;
    sead::Quatf q = sead::Quatf::unit;
    f32 value;
    if (keyIndex < 1) {
        const ConveyerKey& key = getConveyerKey(0);
        v.set(key._18);
        q.set(key._8);
        value = 0.0f;
    } else {
        const ConveyerKey& key = getConveyerKey(keyIndex);
        const ConveyerKey& prevKey = getConveyerKey(keyIndex - 1);
        sead::Vector3f prevKeyVec = prevKey._18;
        sead::Vector3f keyVec = key._18;
        f32 t = key._4 - prevKey._4;
        if (isNearZero(t, 0.001f))
            t = 0.0f;
        else
            t = (coord - getConveyerKey(keyIndex - 1)._4) / t;

        f32 ease = easeByType(t, getConveyerKey(keyIndex - 1).interpolateType);
        lerpVec(&v, prevKeyVec, keyVec, ease);
        value = lerpValue(getConveyerKey(keyIndex - 1)._0, getConveyerKey(keyIndex)._0, t);

        sead::Quatf prevKeyQuat = getConveyerKey(keyIndex - 1)._8;
        sead::Quatf keyQuat = getConveyerKey(keyIndex)._8;
        slerpQuat(&q, prevKeyQuat, keyQuat, ease);
    }

    if (pos != nullptr)
        pos->set(value * mMoveDirection + mTrans + v);

    if (quat)
        quat->set(q);

    if (index)
        *index = keyIndex - 1;
    return;
}

// Mismatch: The math is right, but the registers don't match
// https://decomp.me/scratch/kmQvB
void ConveyerKeyKeeper::calcPosAndQuatByKeyIndex(sead::Vector3f* pos, sead::Quatf* quat,
                                                 s32 index) const {
    const ConveyerKey* key = &getConveyerKey(index);
    if (pos != nullptr)
        pos->set(key->_0 * mMoveDirection + mTrans + key->_18);

    if (quat)
        quat->set(getConveyerKey(index)._8);
}

// Mismatch: Same as calcPosAndQuatByKeyIndex
// https://decomp.me/scratch/kmQvB
void ConveyerKeyKeeper::calcClippingSphere(sead::Vector3f* clippingTrans, f32* clippingRadius,
                                           f32 offset) const {
    if (clippingTrans != nullptr) {
        const ConveyerKey& key = getConveyerKey(0);
        clippingTrans->set(key._0 * mMoveDirection + mTrans + key._18);
    }
    *clippingRadius = offset;

    f32 radiusOffset = offset;
    for (s32 i = 1; i < mConveyerKeyCount; i++) {
        const ConveyerKey& key = getConveyerKey(i);
        sead::Vector3f pos = key._0 * mMoveDirection + mTrans + key._18;
        calcSphereMargeSpheres(clippingTrans, clippingRadius, *clippingTrans, radiusOffset, pos,
                               offset);
        radiusOffset = *clippingRadius;
    }
}

const ConveyerKey& ConveyerKeyKeeper::getConveyerKey(s32 index) const {
    return mConveyerKeys[index];
}
}  // namespace al
