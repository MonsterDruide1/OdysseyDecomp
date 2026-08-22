#include "Enemy/TRexPatrolRouteRider.h"

#include <cmath>

#include "Library/Math/MathUtil.h"

#include "Enemy/TRexPatrolRouteBuilder.h"

TRexPatrolRouteRider::TRexPatrolRouteRider() {
    mRouteDistance = 0.0f;
    mRoute = nullptr;
    mGoalRoute = nullptr;
    mBuilder = nullptr;
}

void TRexPatrolRouteRider::init(const TRexPatrolRouteBuilder* builder) {
    mBuilder = builder;
    mBuilder->calcFirstRoute(&mRoute, &mGoalRoute);
    mRouteDistance = 0.0f;
}

void TRexPatrolRouteRider::resetRoute() {
    mBuilder->calcFirstRoute(&mRoute, &mGoalRoute);
    mRouteDistance = 0.0f;
}

void TRexPatrolRouteRider::move(f32 distance) {
    while (true) {
        const sead::Vector3f routeTrans = mRoute->getTrans();
        const sead::Vector3f goalTrans = mGoalRoute->getTrans();
        const f32 routeLength = (routeTrans - goalTrans).length();
        const f32 nextDistance = distance + mRouteDistance;
        if (routeLength < nextDistance) {
            distance -= routeLength - mRouteDistance;
            mRouteDistance = 0.0f;
            mRoute = mGoalRoute;
            mBuilder->calcNextRouteGoal(&mGoalRoute, mGoalRoute);
        } else {
            mRouteDistance = nextDistance;
            return;
        }
    }
}

void TRexPatrolRouteRider::recoveryRoute(const sead::Vector3f& trans) {
    mBuilder->calcRecoveryRoute(&mRoute, &mGoalRoute, trans);
    mRouteDistance = 0.0f;
}

void TRexPatrolRouteRider::rebuildRoute(const sead::Vector3f& trans) {
    mBuilder->calcNearestRoute(&mRoute, &mGoalRoute, trans);

    sead::Vector3f routeDir = mGoalRoute->getTrans() - mRoute->getTrans();
    al::normalize(&routeDir);

    sead::Vector3f routeDiff = trans - mRoute->getTrans();
    al::parallelizeVec(&routeDiff, routeDir, routeDiff);
    mRouteDistance = routeDiff.length();
}

void TRexPatrolRouteRider::calcTrans(sead::Vector3f* out, f32 distance) const {
    out->set(mRoute->getTrans());

    const sead::Vector3f& goalTrans = mGoalRoute->getTrans();
    distance += mRouteDistance;

    const sead::Vector3f diff = goalTrans - *out;
    const f32 routeLength = diff.length();

    if (routeLength <= distance) {
        out->set(goalTrans);
        return;
    }

    const f32 invRouteLength = 1.0f / routeLength;
    out->setScaleAdd(distance, diff * invRouteLength, *out);
}

bool TRexPatrolRouteRider::calcMoveDirH(sead::Vector3f* out, const sead::Vector3f& vertical) const {
    sead::Vector3f routeDir = mGoalRoute->getTrans() - mRoute->getTrans();
    al::verticalizeVec(out, vertical, routeDir);
    return al::tryNormalizeOrZero(out);
}
