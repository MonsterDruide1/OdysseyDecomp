#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class PlacementInfo;
}  // namespace al

class TRexRouteInfoBase {
public:
    TRexRouteInfoBase(const al::PlacementInfo& placementInfo);

    const sead::Vector3f& getTrans() const { return mTrans; }

private:
    sead::Vector3f mTrans;
    s32 mPlacementIdHash;
};

static_assert(sizeof(TRexRouteInfoBase) == 0x10);

class TRexPatrolRouteBuilder {
public:
    TRexPatrolRouteBuilder();

    void init(const al::ActorInitInfo& initInfo);
    void calcFirstRoute(const TRexRouteInfoBase** route, const TRexRouteInfoBase** goalRoute) const;
    void calcNearestRoute(const TRexRouteInfoBase** route, const TRexRouteInfoBase** goalRoute,
                          const sead::Vector3f& trans) const;
    void calcRecoveryRoute(const TRexRouteInfoBase** route, const TRexRouteInfoBase** goalRoute,
                           const sead::Vector3f& trans) const;
    void calcNextRouteGoal(const TRexRouteInfoBase** goalRoute,
                           const TRexRouteInfoBase* route) const;

private:
    s32 mMainRouteInfoCount;
    TRexRouteInfoBase** mMainRouteInfos;
    s32 mSubRouteInfoCount;
    TRexRouteInfoBase** mSubRouteInfos;
};

static_assert(sizeof(TRexPatrolRouteBuilder) == 0x20);
