#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al
class IUsePlayerPuppet;

class CollectionList : public al::LiveActor {
public:
    CollectionList(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool isOpen() const;
    void close();
    void exeWait();
    void exeOpen();
    void exeAfterClose();

private:
    IUsePlayerPuppet* mPlayerPuppet = nullptr;
};

static_assert(sizeof(CollectionList) == 0x110);
