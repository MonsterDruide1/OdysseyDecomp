#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;
class CameraTicket;
}  // namespace al

class BossForest;

class BossForestCameraController : public al::NerveExecutor {
public:
    BossForestCameraController(BossForest* boss, const al::ActorInitInfo& initInfo,
                               const sead::Vector3f* targetPos);
    ~BossForestCameraController() override;

    sead::Vector3f calcCameraTargetPos() const;
    bool tryStartCamera();
    bool tryStartCameraOld();
    bool tryEndCamera();
    bool tryEndCameraOld();
    bool tryRestartCamera();
    bool tryRestartCameraOld();
    void allowNearCamera();
    void allowNearCameraOld();
    void disallowNearCamera();
    void disallowNearCameraOld();
    void control();

    void exeOld();
    void endOld();
    void exeTower();
    void endTower();

private:
    BossForest* mBoss = nullptr;
    al::CameraTicket* mBossBattleCameraTicket = nullptr;
    al::CameraTicket* mTowerCameraTicket = nullptr;
    const sead::Vector3f* mTargetPos = nullptr;
    bool mIsNearCameraAllowed = false;
    sead::Vector3f mTowerCameraOffset = sead::Vector3f::zero;
};

static_assert(sizeof(BossForestCameraController) == 0x40);
