#include "Player/PlayerCeilingCheck.h"

#include "Library/Base/StringUtil.h"
#include "Library/Collision/Collider.h"

#include "Player/PlayerCollisionCheckSphereMove.h"

PlayerCeilingCheck::PlayerCeilingCheck(al::CollisionDirector* director) {
    mCollisionCheckSphereMove = new PlayerCollisionCheckSphereMove(director, 16);
    setupCeilingCheckNormal();
}

void PlayerCeilingCheck::setupCeilingCheckNormal() {
    mIsCeilingCheckNormal = true;
    mIsCeilingCheckGrab = false;
}

void PlayerCeilingCheck::update(const sead::Vector3f& trans, const sead::Vector3f& up, f32 tall,
                                f32 collisionRadiusSquat, f32 headClearance, f32 holdHeight) {
    sead::Vector3f checkStart;
    checkStart.set(trans);

    f32 playerHeight;
    sead::Vector3f newCheckStart;
    if (mIsCeilingCheckGrab) {
        newCheckStart = trans - (collisionRadiusSquat + headClearance) * up;
        playerHeight = 2 * tall;
    } else {
        newCheckStart = trans + (collisionRadiusSquat - headClearance) * up;
        playerHeight = (2 * tall) - (2 * collisionRadiusSquat);
        tall -= 2 * collisionRadiusSquat;
    }
    checkStart = newCheckStart;

    f32 checkHeight = playerHeight + headClearance + holdHeight;
    f32 minTStandUp = (tall + headClearance) / checkHeight;
    mCollisionCheckSphereMove->checkSphereMove(checkStart, checkHeight * up, collisionRadiusSquat);

    sead::Vector3f down = -up;
    f32 minTValue2 = 1.0f;
    f32 minTValue = 1.0f;
    s32 minIndex = -1;
    for (u32 i = 0; i < mCollisionCheckSphereMove->getNum(); i++) {
        if (!al::isCeilingPolygon(mCollisionCheckSphereMove->getNormal(i), down))
            continue;

        f32 tValue = mCollisionCheckSphereMove->getTValue(i);
        if (minTValue > tValue)
            minTValue = tValue;

        if (mIsCeilingCheckNormal &&
            al::isEqualString(mCollisionCheckSphereMove->getMapCodeName(i), "Press"))
            continue;

        if (al::isEqualString(mCollisionCheckSphereMove->getWallCodeName(i), "NoCeilSquat"))
            continue;

        minIndex = minTValue2 > tValue ? i : minIndex;
        minTValue2 = sead::Mathf::clampMax(minTValue2, tValue);
    }

    f32 minTHoldUp = (0.99f * (checkHeight - holdHeight));
    f32 maxTPressedCeil = (0.01f * (checkHeight - holdHeight));

    mIsEnableStandUp = minTValue2 >= minTStandUp;
    mIsEnableHoldUp = minTValue >= minTHoldUp / checkHeight;
    mIsPressedCeil = minTValue2 < maxTPressedCeil / checkHeight;
    mCeilCheckHeight = checkHeight + (checkStart - trans).dot(up);
    mSafetyCeilSpace = mCeilCheckHeight;
    if (minIndex >= 0)
        mSafetyCeilSpace = (mCollisionCheckSphereMove->getPos(minIndex) - trans).dot(up);
}

void PlayerCeilingCheck::setupCeilingCheckGrab() {
    mIsCeilingCheckNormal = false;
    mIsCeilingCheckGrab = true;
}

void PlayerCeilingCheck::setCollisionPartsFilter(const al::CollisionPartsFilterBase* filter) {
    mCollisionCheckSphereMove->setCollisionPartsFilter(filter);
}
