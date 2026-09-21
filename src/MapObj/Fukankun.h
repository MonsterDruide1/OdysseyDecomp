#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class FilterFly;

class Fukankun : public al::LiveActor {
public:
    Fukankun(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void switchStart();
    void switchAppear();

    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void cleanupCaptureView();
    void turnHeadToDir(const sead::Vector3f&, f32);
    void endCaptureAndNextNerve();

    void exeStandby();
    void exeRecoverWait();
    void exeWait();
    void exeCaptureWait();
    void exeCaptureStart();
    void exeCaptureDemo();

    f32 calcTargetYUp();
    void updateCamera(const sead::Vector3f&, f32, f32);

    void exeCaptureStartEnd();
    void exeCaptureViewWait();

    void setupBalloon();

    void exeCaptureView();
    void exeCaptureEnd();
    void exeJoin();
    void exeReaction();

    const sead::Matrix34f* getLinkedShineMtx() const { return &mLinkedShineMtx; }

    bool hasLinkedShine() const { return mHasLinkedShine; }

private:
    char _108[0x60];
    FilterFly* mFilterFly;
    char _170[0xa0];
    sead::Matrix34f mLinkedShineMtx;
    bool mHasLinkedShine;
    char _240[0x7];
};

static_assert(sizeof(Fukankun) == 0x248);
