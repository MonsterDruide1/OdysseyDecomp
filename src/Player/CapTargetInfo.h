#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class CapTargetInfo {
public:
    CapTargetInfo();

    void init(const al::LiveActor*, const char*);
    void setFollowLockOnMtx(const char*, const sead::Vector3f&, const sead::Vector3f&);
    void setLockOnStartAnimName(const char*);
    void setLockOnAnimName(const char*);
    void setHackName(const char*);
    void makeLockOnMtx(sead::Matrix34f*) const;
    void calcLockOnFollowTargetScale(sead::Vector3f*) const;

private:
    void* filler[16];
};
