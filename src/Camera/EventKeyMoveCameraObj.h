#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class KeyMoveCameraObj;
}  // namespace al

class EventKeyMoveCameraObjNoDemo : public al::LiveActor {
public:
    EventKeyMoveCameraObjNoDemo(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void makeActorAlive() override;
    void kill() override;
    void control() override;

    al::KeyMoveCameraObj* getKeyMoveCameraObj() const { return mKeyMoveCameraObj; }

    s32 getStep() const { return mStep; }

    void setStep(s32 step) { mStep = step; }

    bool isPlayed() const { return mIsPlayed; }

    void setIsPlayed(bool isPlayed) { mIsPlayed = isPlayed; }

    bool isPlayManyTimes() const { return mIsPlayManyTimes; }

private:
    al::KeyMoveCameraObj* mKeyMoveCameraObj = nullptr;
    s32 mStep = 0;
    bool mIsPlayed = false;
    bool mIsPlayManyTimes = false;
};

static_assert(sizeof(EventKeyMoveCameraObjNoDemo) == 0x118);

class EventKeyMoveCameraObjWithDemo : public EventKeyMoveCameraObjNoDemo {
public:
    EventKeyMoveCameraObjWithDemo(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void kill() override;

private:
    const char* mDemoName = nullptr;
};

static_assert(sizeof(EventKeyMoveCameraObjWithDemo) == 0x120);
