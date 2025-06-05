#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class CameraTicket;
class HitSensor;
class SensorMsg;
}  // namespace al
class Shine;
class TreasureBoxKeyOpener;

class TreasureBoxKey : public al::LiveActor {
public:
    TreasureBoxKey(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void makeActorDead() override;
    void makeActorAlive() override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;
    void restart();
    void exeCloseWait();
    void exeWaitStartDemo();
    void exeWaitCameraInterpole();
    void exeOpen();
    void exeWaitEndCameraInterpole();
    void exeOpenWait();

    Shine* getShine() { return mShine; }

private:
    bool mIsStopUpdateWhileCameraMoving = false;
    s32 mCameraStartInterpFrame = 45;
    Shine* mShine = nullptr;
    TreasureBoxKeyOpener* mTreasureBoxKeyOpener = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
};

static_assert(sizeof(TreasureBoxKey) == 0x128);
