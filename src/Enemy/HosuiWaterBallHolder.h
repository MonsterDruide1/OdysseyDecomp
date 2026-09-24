#pragma once

#include <container/seadPtrArray.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

class HosuiWaterBall;

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
}  // namespace al

class HosuiWaterBallHolder : public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_HosuiWaterBallHolder;

    HosuiWaterBallHolder(const al::ActorInitInfo& initInfo);

    HosuiWaterBall* get();
    void killAll();

private:
    sead::FixedPtrArray<HosuiWaterBall, 128> mWaterBalls;
    s32 mIndex = 0;
};

static_assert(sizeof(HosuiWaterBallHolder) == 0x420);

namespace HosuiUtil {
void tryCreateHosuiWaterBallHolder(const al::IUseSceneObjHolder* objHolder,
                                   const al::ActorInitInfo& initInfo);
HosuiWaterBall* tryGetHosuiWaterBall(const al::IUseSceneObjHolder* objHolder);
void killAllHosuiWaterBall(const al::IUseSceneObjHolder* objHolder);
}  // namespace HosuiUtil
