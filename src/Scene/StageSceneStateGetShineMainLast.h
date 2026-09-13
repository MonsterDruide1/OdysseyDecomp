#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class CameraTicket;
class LiveActor;
class Scene;
}  // namespace al

class StageSceneStateGetShineMainLast : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateGetShineMainLast(const char* name, al::Scene* scene, al::LiveActor* playerActor,
                                    al::CameraTicket* demoCameraTicket);

    void appear() override;
    void kill() override;

    void exeWait();

private:
    al::LiveActor* mPlayerActor = nullptr;
    al::CameraTicket* mDemoCameraTicket = nullptr;
};

static_assert(sizeof(StageSceneStateGetShineMainLast) == 0x30);
