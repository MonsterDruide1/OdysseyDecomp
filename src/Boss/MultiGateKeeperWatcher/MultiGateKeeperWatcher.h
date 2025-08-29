#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class LiveActor;
struct ActorInitInfo;
}  // namespace al

class MultiGateKeeperWatcher : public al::LiveActor {
public:
    MultiGateKeeperWatcher(const char*);
    void init(const al::ActorInitInfo&) override;
    bool isExistAliveGK() const;
    bool isEnableShowPlayer() const;
    bool isEndDemo() const;
    void addDemoKoopaStatue();
    void noticeGKDead(const al::LiveActor*, al::LiveActor*);
    void exeWatch();
    void exeBonFire();
    void exeShineAppear();
    void exeEntranceCamera();
};
