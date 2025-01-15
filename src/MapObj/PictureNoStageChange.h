#pragma once

#include "Library/LiveActor/LiveActor.h"

class PictureNoStageChange : public al::LiveActor {
public:
    PictureNoStageChange(const char* name, const char* textureName);

    virtual void init(const al::ActorInitInfo& info) override;
    virtual bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                            al::HitSensor* target) override;

    void exeWait();

private:
    const char* mTextureName;
};
