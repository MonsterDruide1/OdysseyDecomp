#pragma once

#include "Library/LiveActor/LiveActor.h"

class SaveObjInfo;

class ChurchDoor : public al::LiveActor {
public:
    ChurchDoor(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* source,
                    al::HitSensor* target) override;

    bool isOpenWait() const;
    bool isDemoEnterChurch() const;
    void startDemoEnterChurch();
    void endDemoEnterChurch();

    void exeCloseWait1();
    void exeOpen1();
    void exeCloseWait2();
    void exeOpen2();
    void exeCloseWait3();
    void exeOpen3();
    void exeOpenWait();
    void exeDemoEnterChurch();

private:
    SaveObjInfo* mSaveObjInfo = nullptr;
};
