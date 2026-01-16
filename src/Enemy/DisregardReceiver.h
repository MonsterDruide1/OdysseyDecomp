#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class DisregardReceiver {
public:
    DisregardReceiver(al::LiveActor*, const char*);

    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* self, al::HitSensor* other);
    bool checkActionDisregardAll() const;
    bool checkActionDisregardHomingAttack() const;
    bool checkActionDisregardTargetMarker() const;

    void setEDC(bool e, bool d, bool c) {
        _e = e;
        _d = d;
        _c = c;
    }

    void set44(const sead::Vector3f& value) {
        _40 = true;
        _44 = value;
    }

private:
    al::LiveActor* mActor;
    s32 _8;
    bool _c;
    bool _d;
    bool _e;
    s32 _10;
    void* _18;
    s32 _20;
    void* _28;
    s32 _30;
    void* _38;
    bool _40;
    sead::Vector3f _44;
    char filler[0x8];
};

static_assert(sizeof(DisregardReceiver) == 0x58);
