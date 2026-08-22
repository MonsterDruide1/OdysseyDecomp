#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
class WaterSurfaceFinder;
}  // namespace al

class GiantWanderBossLifeUpItemHolder;
class WaterSurfaceShadow;

class GiantWanderBossBullet : public al::LiveActor {
public:
    GiantWanderBossBullet(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void appearAttach(const sead::Matrix34f* attachMtx, const sead::Vector3f* localOffset,
                      bool isWaterSurfaceEffect);
    void startLaunch();
    bool isLaunched() const;
    void exeAppearAttach();
    void exeFly();
    void resetPositionByAnim();
    bool checkCollideAndSendMsg();

    void setLifeUpItemHolder(GiantWanderBossLifeUpItemHolder* lifeUpItemHolder) {
        mLifeUpItemHolder = lifeUpItemHolder;
    }

private:
    sead::Vector3f mScale;
    const sead::Matrix34f* mAttachMtx;
    const sead::Vector3f* mLocalOffset;
    al::WaterSurfaceFinder* mWaterSurfaceFinder;
    WaterSurfaceShadow* mWaterSurfaceShadow;
    bool mIsWaterSurfaceEffect;
    GiantWanderBossLifeUpItemHolder* mLifeUpItemHolder;
    s32 mHitPoint;
    sead::Matrix34f mWaterSurfaceMtx;
};

static_assert(sizeof(GiantWanderBossBullet) == 0x180);
