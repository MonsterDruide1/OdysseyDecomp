#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class IUseAudioKeeper;
class MtxConnector;

using JudgeFuncPtr = bool (*)(const SensorMsg* message, HitSensor* other, HitSensor* self);

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
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    virtual JudgeFuncPtr getJudgeFunction(const char* name) const;

private:
    JudgeFuncPtr mJudgeFunction = nullptr;
    MtxConnector* mMtxConnector = nullptr;
    sead::Vector3f mItemOffset = {0.0f, 0.0f, 0.0f};
    bool mIsExistHitReactionBreak = false;
    IUseAudioKeeper* mAudioKeeper = nullptr;
};
}  // namespace al
