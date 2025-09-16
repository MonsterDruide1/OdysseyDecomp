#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Rail/VertexGraph.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class ElectricWireVertex;

class ElectricWireRailKeeper;

class ElectricWire : public al::LiveActor {
public:
    ElectricWire(const char*);
<<<<<<< HEAD
    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void initAfterPlacement() override;
    void control() override;

    bool tryCreateElectricWireLinks(al::LiveActor**, al::LiveActor*, const al::ActorInitInfo&,
                                    const char*, s32);
    void createGraph(const al::ActorInitInfo&);
    void switchStartOn();
=======

>>>>>>> 5281125b (Stashing)
    void tryUpdateDisplayModel();
    ElectricWireVertex* searchAliveKeeperGraphVertexBySensor(const al::HitSensor*);
    void endCapture(const sead::Vector3f*, const sead::Vector3f*, bool, bool);
    void afterSpringMovement();
    void springMovement();
    void exeHackEnd();
    void exeStandby();
    void exeCaptureWait();
    void exeCaptureStart();
    void exeCaptureHackStart();
    bool trySwitchMoveEndCamera(const ElectricWireVertex*);
    void exeSwitchRail();
    void trySetPoseGraphMoveDir(f32);
    const char* getMoveActionName() const;
    const char* getMoveAttackableActionName() const;
    const char* getSparkActionName() const;
    void exeTurnFrontStartDemo();
    void exeMove();
    bool checkMovableInputAndNextNerve();
    f32 checkEnableMoveByGraph(const al::Graph::PosVertex**, const sead::Vector2f&,
                               const al::LiveActor*, const al::Graph::PosVertex*);
    ElectricWireVertex* checkMovableInputNextByGraph(bool*, const al::Graph::PosVertex*,
                                                     const sead::Vector2f&);
    void exeMoveEnd();
    void onMoveEndEnd();
    void exeLightning();
    void exeLightningEnd();
    void exeLightningEndToMove();
    void addDemoActorElectricWirePartsAll();

    bool isElectricWireRadio() { return mIsElectricWireRadio; }

    void addDemoActorElectricWirePartsAll();

    s32 get_110() const { return _110; }

    ElectricWireRailKeeper** getElectricWireRailKeepers() { return mElectricWireRailKeepers; }

private:
<<<<<<< HEAD
<<<<<<< HEAD
    char filler_108[0x1a1];
=======
    void* pad;
    s32 _110;
    ElectricWireRailKeeper** mElectricWireRailKeepers;
<<<<<<< HEAD
    char pad3[0x1e0];
>>>>>>> 5281125b (Stashing)
    bool mIsElectricWireRadio;
    char filler_2aa[0x58];
=======
    char pad2[0x189];
    bool mIsElectricWireRadio;
    char pad3[0x58];
>>>>>>> 3eae8abd (Implemented: StakcerCapWorldCtrl)
=======
    void* mPad;
    s32 _110;
    ElectricWireRailKeeper** mElectricWireRailKeepers;
    char mPad2[0x189];
    bool mIsElectricWireRadio;
    char mPad3[0x58];
>>>>>>> 40f0138c (Fixed format issues)
};

static_assert(sizeof(ElectricWire) == 0x308);
