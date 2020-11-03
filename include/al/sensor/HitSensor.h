#pragma once

#include "al/actor/LiveActor.h"
#include "sead/seadVector.h"

namespace al
{
    class HitSensor
    {
    public:

        void update();
        void addHitSensor(al::HitSensor *);

        const char* mName; // _0
        int _8;
        int _C;
        unsigned long _10;
        float _18;
        unsigned short mMaxSensorCount; // _1C
        short mSensorCount; // _1E
        al::HitSensor** mSensors; // _20
        unsigned long _28;
        unsigned long _30;
        unsigned long _38;
        al::LiveActor* mParentActor; // _40
        unsigned long _48;
        sead::Vector3<float>* _50;
    };
};