#pragma once

#include <math/seadBoundBox.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class RollingCubePose;
struct ActorInitInfo;

class RollingCubePoseKeeper {
public:
    RollingCubePoseKeeper();

    void setCubeSize(const sead::BoundBox3f& cubeSize);
    bool isMoveTypeTurn() const;
    bool isMoveTypeLoop() const;
    void init(const ActorInitInfo& initInfo);
    bool nextKey();
    void setStart();
    void setKeyIndex(s32 index);
    const RollingCubePose& getCurrentPose() const;
    const RollingCubePose& getPose(s32 index) const;
    void calcBoundingBoxCenter(sead::Vector3f* center, const sead::Quatf& quat,
                               const sead::Vector3f& trans) const;

    s32 getPoseCount() const { return mPoseCount; }

    s32 getKeyIndex() const { return mKeyIndex; }

    const sead::BoundBox3f& getCubeSize() const { return mCubeSize; }

private:
    enum MoveType : s32 { None, Loop, Turn, All };

    RollingCubePose* mRollingCubePoses = nullptr;
    s32 mPoseCount = 0;
    s32 mKeyIndex = 0;
    sead::BoundBox3f mCubeSize;
    MoveType mMoveType = MoveType::Loop;
};

}  // namespace al
