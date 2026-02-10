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

class ElectricWire : public al::LiveActor {
public:
    ElectricWire(const char*);
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

private:
    char filler_108[0x1a1];
    bool mIsElectricWireRadio;
    char filler_2aa[0x58];
};

static_assert(sizeof(ElectricWire) == 0x308);
