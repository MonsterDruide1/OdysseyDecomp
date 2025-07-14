#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class ParameterObj;

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
    void getTargetRadius() const;
    const char* getTargetName() const;
    void setTargetName(const char*);
    void setTargetRadius(f32);
    void setTargetFollowPosOffset(const sead::Vector3f&);
    void setJointName(const char*);

    ParameterObj* getParameterObj() { return mParameterObj; }

private:
    void* filler;
    ParameterObj* mParameterObj;
    char filler2[0x60];
};
}  // namespace al
