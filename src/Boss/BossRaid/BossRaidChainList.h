#pragma once

#include "Library/LiveActor/LiveActor.h"

class BossRaidChain;

class BossRaidChainList : public al::LiveActor {
public:
    BossRaidChainList(const char* name, const char* modelName, s32 count, f32 minDist, f32 maxDist);
    void init(const al::ActorInitInfo& info) override;
    void makeActorAlive() override;
    void makeActorDead() override;
    void setRootPosPtr(const sead::Vector3f* pos);
    void setTipPosPtr(const sead::Vector3f* pos);
    void calcRootAndTipPos(sead::Vector3f* outRoot, sead::Vector3f* outTip);
    BossRaidChain* getChain(s32 index);
    void registerHostSubActorSyncClipping(al::LiveActor* actor);
    void resetChain();
    void resetChain(const sead::Vector3f& rootPos, const sead::Vector3f& tipPos);
    void startBlowDown();
    void active();
    void deactive();
    void setUpDemo();
    void reset();
    void exeDemo();
    void exeDeactive();
    void exeWait();
    void exeBlowDown();

private:
    BossRaidChain** mChains = nullptr;
    const char* mModelName = nullptr;
    s32 mChainCount = 0;
    const sead::Vector3f* mRootPosPtr = nullptr;
    const sead::Vector3f* mTipPosPtr = nullptr;
    f32 mMinDist = 0.0f;
    f32 mMaxDist = 0.0f;
};

static_assert(sizeof(BossRaidChainList) == 0x138);
