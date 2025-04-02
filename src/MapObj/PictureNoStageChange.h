#pragma once

#include "Library/LiveActor/LiveActor.h"

class PictureNoStageChange : public al::LiveActor {
public:
    PictureNoStageChange(const char* name, const char* archiveName);

    virtual void init(const al::ActorInitInfo& info) override;
    virtual bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) override;

    void exeWait();

private:
    const char* mArchiveName;
};
