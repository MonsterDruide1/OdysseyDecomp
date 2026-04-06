#include "Boss/Koopa/KoopaLandPointHolder.h"

#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

KoopaLandPointHolder::KoopaLandPointHolder(const al::ActorInitInfo& initInfo) {
    al::getTrans(&mTrans, initInfo);
    al::getQuat(&mQuat, initInfo);
    mLandPoints = al::calcLinkChildNum(initInfo, "LandPoint");

    if (mLandPoints <= 0)
        return;

    mPointsQuat = new sead::Quatf[mLandPoints];
    mPointsTrans = new sead::Vector3f[mLandPoints];
    mInvalidPoints = new bool[mLandPoints];

    for (s32 i = 0; i < mLandPoints; i++) {
        mInvalidPoints[i] = false;

        al::PlacementInfo placementInfo;
        al::getLinksInfoByIndex(&placementInfo, initInfo, "LandPoint", i);
        al::getQuat(&mPointsQuat[i], placementInfo);
        al::getTrans(&mPointsTrans[i], placementInfo);
    }
}

inline f32 getKoopaLandPointDistance(const sead::Vector3f& posA, const sead::Vector3f& posB) {
    sead::Vector3f distance = posA - posB;
    return sead::Mathf::sqrt(distance.x * distance.x + distance.z * distance.z);
}

void KoopaLandPointHolder::decidePointFarFrom(const sead::Vector3f& pos) {
    s32 selectedPoint = -1;
    f32 maxDistance = 0.0f;

    for (s32 i = 0; i < mLandPoints; i++) {
        if (mInvalidPoints[i])
            continue;

        f32 dist = getKoopaLandPointDistance(mPointsTrans[i], pos);

        if (selectedPoint < 0 || maxDistance < dist) {
            selectedPoint = i;
            maxDistance = dist;
        }
    }

    invalidatePoint(selectedPoint);
    mCurrentLandPoint = selectedPoint;
}

void KoopaLandPointHolder::invalidatePoint(s32 index) {
    mInvalidPoints[index] = true;

    if (mCurrentLandPoint > -1)
        mInvalidPoints[mCurrentLandPoint] = false;
}

void KoopaLandPointHolder::decidePointEitherFarSide(const sead::Vector3f& pos) {
    if (mCurrentLandPoint < 0) {
        decidePointFarFrom(pos);
        return;
    }

    mInvalidPoints[mCurrentLandPoint] = false;
    s32 prevPoint = al::modi(mCurrentLandPoint + mLandPoints - 1, mLandPoints);
    s32 nextPoint = al::modi(mCurrentLandPoint + mLandPoints + 1, mLandPoints);

    f32 prevDist = getKoopaLandPointDistance(mPointsTrans[prevPoint], pos);
    f32 nextDist = getKoopaLandPointDistance(mPointsTrans[nextPoint], pos);

    if (prevDist < nextDist)
        mCurrentLandPoint = nextPoint;
    else
        mCurrentLandPoint = prevPoint;
}

void KoopaLandPointHolder::decidePointNearFrom(const sead::Vector3f& pos) {
    s32 selectedPoint = -1;
    f32 minDistance = 0.0f;

    for (s32 i = 0; i < mLandPoints; i++) {
        if (mInvalidPoints[i])
            continue;

        f32 dist = getKoopaLandPointDistance(mPointsTrans[i], pos);

        if (selectedPoint < 0 || dist < minDistance) {
            selectedPoint = i;
            minDistance = dist;
        }
    }

    invalidatePoint(selectedPoint);
    mCurrentLandPoint = selectedPoint;
}

void KoopaLandPointHolder::reset() {
    for (s32 i = 0; i < mLandPoints; i++)
        mInvalidPoints[i] = false;
}

const sead::Vector3f& KoopaLandPointHolder::findNearestPointTrans(const sead::Vector3f& pos) const {
    f32 minDistance = getKoopaLandPointDistance(mPointsTrans[0], pos);
    s32 selectedPoint = 0;

    for (s32 i = 1; i < mLandPoints; i++) {
        f32 dist = getKoopaLandPointDistance(mPointsTrans[i], pos);

        if (dist < minDistance) {
            selectedPoint = i;
            minDistance = dist;
        }
    }

    return mPointsTrans[selectedPoint];
}
