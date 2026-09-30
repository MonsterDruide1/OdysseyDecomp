#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

class TRexPatrolRouteBuilder;
class TRexRouteInfoBase;

class TRexPatrolRouteRider {
public:
    TRexPatrolRouteRider();

    void init(const TRexPatrolRouteBuilder* builder);
    void resetRoute();
    void move(f32 distance);
    void recoveryRoute(const sead::Vector3f& trans);
    void rebuildRoute(const sead::Vector3f& trans);
    void calcTrans(sead::Vector3f* out, f32 distance) const;
    bool calcMoveDirH(sead::Vector3f* out, const sead::Vector3f& vertical) const;

private:
    const TRexPatrolRouteBuilder* mBuilder;
    const TRexRouteInfoBase* mRoute;
    const TRexRouteInfoBase* mGoalRoute;
    f32 mRouteDistance;
};

static_assert(sizeof(TRexPatrolRouteRider) == 0x20);
