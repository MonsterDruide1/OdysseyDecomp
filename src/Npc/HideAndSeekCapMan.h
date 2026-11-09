#pragma once

#include <basis/seadTypes.h>

#include "Library/Obj/PartsModel.h"

namespace al {
class EventFlowExecutor;
class HitSensor;
class SensorMsg;
}  // namespace al

class HideAndSeekCapMan : public al::PartsModel {
public:
    HideAndSeekCapMan(const char* name);

    static HideAndSeekCapMan* tryCreateLinkNpcIfClearGame(al::LiveActor* actor,
                                                           const al::ActorInitInfo& info,
                                                           const char* arcSuffix,
                                                           const char* suffix);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeAppear();
    void exeWait();
    void exeDisappear();
    void exeHide();
    void exeScared();

private:
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    s32 _150 = -1;
    s32 _154 = 0;
};

static_assert(sizeof(HideAndSeekCapMan) == 0x158);
