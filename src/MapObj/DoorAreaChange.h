#pragma once

#include "Library/LiveActor/LiveActor.h"

class SaveObjInfo;

class DoorAreaChange : public al::LiveActor {
public:
    DoorAreaChange(const char* name);
    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void switchCloseAgain();
    void start();
    void setNoStart();
    void enableStart();
    void appear() override;

    bool isOpen() const;

    void setHomeDoor(bool b);

    void exeWait();
    void exeStart();
    void exeOpen();
    void exeOpenWait();
    void exeCloseWait();
    void exeNoStart();
    void exeCloseBefore();
    void exeClose();
    void exeNoStartWithMessage();

private:
    SaveObjInfo* mSaveObjInfo = nullptr;
    bool mIsHomeDoorSet = false;
    bool mIsDoorClosed = false;
};
