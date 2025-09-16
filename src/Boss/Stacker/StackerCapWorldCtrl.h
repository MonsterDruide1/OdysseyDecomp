#pragma once

#include "Library/LiveActor/LiveActor.h"

class Stacker;

namespace al {
class CameraTicket;
struct ActorInitInfo;
class LiveActorGroup;
}  // namespace al

class ElectricWire;

class StackerCapWorldCtrl : public al::LiveActor {
public:
    StackerCapWorldCtrl(const char* name);
    void init(const al::ActorInitInfo&) override;
    void appear() override;
    void exeBattle();
    void exeDemoCamera();
    void exeObjAppear();

private:
    Stacker* mStacker = nullptr;
    ElectricWire* mElectricWire = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
    al::LiveActorGroup* mActorGroup = nullptr;
};
