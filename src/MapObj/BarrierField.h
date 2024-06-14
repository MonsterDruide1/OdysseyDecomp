#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class LiveActor;
class ActorInitInfo;
}  // namespace al

class BarrierField : public al::LiveActor {
public:
    BarrierField(const char*);
    void init(const al::ActorInitInfo&) override;
    void appear() override;
    void kill() override;
    void disappearByShineGet();

    void exeAppear();
    void exeAppearBreedaMoonWorld();
    void exeHide();
    void exeDisappear();

private:
    bool mIsDisappearByShineGet = false;
    bool mIsOnTheMoon = false;
};
