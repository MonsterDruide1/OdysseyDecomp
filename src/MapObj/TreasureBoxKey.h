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
    TreasureBoxKey(const char*);
    void init(const al::ActorInitInfo&) override;
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
    bool mIsStopUpdateWhileCameraMoving;
    s32 mCameraStartInterpFrame;
    Shine* mShine;
    TreasureBoxKeyOpener* mTreasureBoxKeyOpener;
    al::CameraTicket* mCameraTicket;
};

static_assert(sizeof(TreasureBoxKey) == 0x128);
