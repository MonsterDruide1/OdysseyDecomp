#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al
class RiseMapParts;

class RiseMapPartsHolder : public al::LiveActor {
public:
    RiseMapPartsHolder(const char*);
    void init(const al::ActorInitInfo&) override;
    void startRise();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeMove();
    void startRiseAllChild();
    void exeMoveEnd();
    void exeStop();
    void exeRequestDemo();
    void exeDelay();
    void startDemo(bool);

    void setSwitchActor(al::LiveActor* actor) { mSwitchActor = actor; }

    void setRiseMapPartsArray(RiseMapParts** riseMapPartsArray, s32 num) {
        mRiseMapPartsArray = riseMapPartsArray;
        mRiseMapPartsNum = num;
    }

private:
    void* filler_170[13];
    al::LiveActor* mSwitchActor;
    RiseMapParts** mRiseMapPartsArray;
    s32 mRiseMapPartsNum;
    void* filler_198[2];
};

static_assert(sizeof(RiseMapPartsHolder) == 0x198);
