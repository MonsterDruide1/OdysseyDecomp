#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Demo/IUseDemoSkip.h"

namespace al {
class AddDemoInfo;
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al
class ChangeStageInfo;
class ChurchDoor;
class KoopaDemoExecutor;
class Peach;

class KoopaChurch : public al::LiveActor, public IUseDemoSkip {
public:
    KoopaChurch(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;

    void exeBeforeEnter();
    void exeDemoEnter();
    void exeWait();
    void exeDemoChurch();
    void exeChangeStage();
    void exeEnd();

private:
    KoopaDemoExecutor* mDemoExecutor = nullptr;
    ChangeStageInfo* mChangeStageInfo = nullptr;
    ChurchDoor* mChurchDoor = nullptr;
    Peach* mPeach = nullptr;
    al::LiveActor* mDemoPeachRing = nullptr;
    void* filler_138 = nullptr;
    al::AddDemoInfo* mAddDemoInfo = nullptr;
};

static_assert(sizeof(KoopaChurch) == 0x148);
