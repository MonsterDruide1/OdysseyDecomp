#pragma once

#include "Library/MapObj/BreakMapPartsBase.h"

class SaveObjInfo;
class Shine;

class BreakablePole : public al::BreakMapPartsBase {
public:
    BreakablePole(const char* name) : al::BreakMapPartsBase(name) {}

    void init(const al::ActorInitInfo& info) override;
    void kill() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    al::JudgeFuncPtr getJudgeFunction(const char* name) const override;

private:
    Shine* mShine = nullptr;
    bool mHasItem = false;
    bool mIsSave = false;
    SaveObjInfo* mSaveObjInfo = nullptr;
};

static_assert(sizeof(BreakablePole) == 0x148);
