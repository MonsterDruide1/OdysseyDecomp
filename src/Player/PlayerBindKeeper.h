#pragma once

#include <basis/seadTypes.h>

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al
class PlayerBindableSensorList;
class IUsePlayerPuppet;

class PlayerBindKeeper {
public:
    PlayerBindKeeper(al::HitSensor*, IUsePlayerPuppet*);
    bool sendStartMsg();
    void clearBindableSensor();
    void cancelBind();
    bool receiveEndMsg(const al::SensorMsg*);
    void clearBindImpl();
    void cancelBindByDemo();
    bool sendMsgBindDamage();
    bool receiveRequestDamage();
    bool sendMsgCollidedGround();
    bool sendMsgEnableMapCheckPointWarp();
    bool sendMsgBindKeepDemoStart();
    bool sendMsgBindKeepDemoExecute();
    bool sendMsgBindKeepDemoEnd();
    bool sendMsgBindRecoveryLife() const;
    bool collectBindableSensor(al::HitSensor*, al::HitSensor*);
    void appendBindRequest(al::HitSensor*);
    void resetInvalidTimer();

    al::HitSensor* getBindSensor() const { return mBindSensor; }

private:
    al::HitSensor* mBodyHitSensor;
    al::HitSensor* mBindSensor;
    PlayerBindableSensorList* mBindableSensorList;
    IUsePlayerPuppet* mPuppet;
    s32 _20;
    bool _24;
};

static_assert(sizeof(PlayerBindKeeper) == 0x28);
