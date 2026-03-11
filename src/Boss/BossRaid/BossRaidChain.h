#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class BossRaidChain : public al::LiveActor {
public:
    BossRaidChain(const char* name, const char* archiveName, const char* suffix, f32 minDist,
                  f32 maxDist);
    void init(const al::ActorInitInfo& info) override;
    void setPrevChain(BossRaidChain* chain);
    void setNextChain(BossRaidChain* chain);
    void setDemoFollowMtxPtr(const sead::Matrix34f* mtxPtr);
    void setFix();
    void addVelocityChain(const sead::Vector3f& velocity);
    void updateVelocity(BossRaidChain* other);
    void updateDirection(f32 yRate, f32 zRate);
    void resetDirection();
    void startBlowDown();
    void active();
    void deactive();
    void setUpDemo();
    void reset();
    void exeDeactive();
    void exeDemo();
    void exeWait();
    void exeBlowDown();

private:
    const char* mArchiveName = nullptr;
    const char* mSuffix = nullptr;
    const sead::Matrix34f* mDemoFollowMtxPtr = nullptr;
    BossRaidChain* mPrevChain = nullptr;
    BossRaidChain* mNextChain = nullptr;
    sead::Vector3f mBlowAxis = sead::Vector3f::ex;
    f32 mMinDist = 0.0f;
    f32 mMaxDist = 0.0f;
    bool mIsFix = false;
};

static_assert(sizeof(BossRaidChain) == 0x148);
