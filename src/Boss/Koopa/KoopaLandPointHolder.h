#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
}  // namespace al

class KoopaLandPointHolder {
public:
    KoopaLandPointHolder(const al::ActorInitInfo& initInfo);

    void decidePointFarFrom(const sead::Vector3f& pos);
    void invalidatePoint(s32 index);
    void decidePointEitherFarSide(const sead::Vector3f& pos);
    void decidePointNearFrom(const sead::Vector3f& pos);
    void reset();
    const sead::Vector3f& findNearestPointTrans(const sead::Vector3f& pos) const;

private:
    sead::Vector3f mTrans = {0.0f, 0.0f, 0.0f};
    sead::Quatf mQuat = sead::Quatf::unit;
    s32 mCurrentLandPoint = -1;
    s32 mLandPoints = 0;
    sead::Quatf* mPointsQuat = nullptr;
    sead::Vector3f* mPointsTrans = nullptr;
    bool* mInvalidPoints = nullptr;
};

static_assert(sizeof(KoopaLandPointHolder) == 0x40);
