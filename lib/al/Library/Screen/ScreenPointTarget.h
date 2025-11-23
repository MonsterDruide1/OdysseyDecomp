#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class ParameterObj;
class ScreenPointCheckGroup;

class ScreenPointTarget {
public:
    ScreenPointTarget(LiveActor*, const char*, f32, const sead::Vector3f*, const char*,
                      const sead::Vector3f&);

    void setFollowMtxPtrByJointName(const LiveActor*);
    const char* getJointName() const;
    void update();
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    f32 getTargetRadius() const;
    const char* getTargetName() const;
    void setTargetName(const char*);
    void setTargetRadius(f32);
    void setTargetFollowPosOffset(const sead::Vector3f&);
    void setJointName(const char*);

    ParameterObj* getParameterObj() const { return mParameterObj; }

    sead::Matrix34f* getJointMtx() const { return mJointMtx; }

    LiveActor* getActor() const { return mActor; }

    const sead::Vector3f& getTargetPos() const { return mTargetPos; }

private:
    void* filler;
    ParameterObj* mParameterObj;
    char filler2[0x28];
    sead::Vector3f* _38;
    sead::Matrix34f* mJointMtx;
    sead::Vector3f _48;
    sead::Vector3f mTargetPos;
    LiveActor* mActor;
    ScreenPointCheckGroup* mCheckGroup;
};
}  // namespace al
