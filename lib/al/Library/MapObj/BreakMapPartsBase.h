#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;

class BreakMapPartsBase : public LiveActor {
public:
    BreakMapPartsBase(const char* name);

    void init(const ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void kill() override;
    void movement() override;
    void calcAnim() override;
    void exeWait();
    void exeBreak();
    void startBreakByProgram();
    bool receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) override;
    virtual void* getJudgeFunction(const char* name) const;

private:
    void* mJudgeFunction = nullptr;
    MtxConnector* mMtxConnector = nullptr;
    sead::Vector3f mItemOffset = {0, 0, 0};
    bool mIsExistHitReactionBreak = false;
    IUseAudioKeeper* mAudioKeeper = nullptr;
};
}  // namespace al
