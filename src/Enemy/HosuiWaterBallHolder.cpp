#include "Enemy/HosuiWaterBallHolder.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Enemy/HosuiWaterBall.h"

HosuiWaterBallHolder::HosuiWaterBallHolder(const al::ActorInitInfo& initInfo) {
    for (s32 i = 0; i < mWaterBalls.capacity(); i++) {
        HosuiWaterBall* waterBall = new HosuiWaterBall();
        waterBall->init(initInfo);
        mWaterBalls.pushBack(waterBall);
    }
}

HosuiWaterBall* HosuiWaterBallHolder::get() {
    s32 capacity = mWaterBalls.capacity();
    if (capacity < 1)
        return nullptr;

    for (s32 i = 0; i < capacity; i++) {
        HosuiWaterBall* waterBall = mWaterBalls[mIndex];
        mIndex = al::modi(capacity + 1 + mIndex, capacity);
        if (al::isDead(waterBall)) {
            waterBall->clearIgnoreActors();
            return waterBall;
        }
    }

    return nullptr;
}

void HosuiWaterBallHolder::killAll() {
    for (s32 i = 0; i < mWaterBalls.size(); i++) {
        if (al::isAlive(mWaterBalls[i])) {
            al::tryKillEmitterAndParticleAll(mWaterBalls[i]);
            mWaterBalls[i]->makeActorDead();
        }
    }

    mIndex = 0;
}

namespace HosuiUtil {
void tryCreateHosuiWaterBallHolder(const al::IUseSceneObjHolder* objHolder,
                                   const al::ActorInitInfo& initInfo) {
    if (al::isExistSceneObj<HosuiWaterBallHolder>(objHolder))
        return;

    HosuiWaterBallHolder* waterBallHolder = new HosuiWaterBallHolder(initInfo);
    al::setSceneObj(objHolder, waterBallHolder);
}

HosuiWaterBall* tryGetHosuiWaterBall(const al::IUseSceneObjHolder* objHolder) {
    HosuiWaterBallHolder* waterBallHolder = al::tryGetSceneObj<HosuiWaterBallHolder>(objHolder);
    if (!waterBallHolder)
        return nullptr;

    return waterBallHolder->get();
}

void killAllHosuiWaterBall(const al::IUseSceneObjHolder* objHolder) {
    HosuiWaterBallHolder* waterBallHolder = al::tryGetSceneObj<HosuiWaterBallHolder>(objHolder);
    if (waterBallHolder)
        waterBallHolder->killAll();
}
}  // namespace HosuiUtil
