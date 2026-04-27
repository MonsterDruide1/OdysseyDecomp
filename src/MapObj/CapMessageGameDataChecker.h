#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
}  // namespace al

class CapMessageGameDataChecker {
public:
    CapMessageGameDataChecker();

    void initByPlacementInfo(const al::ActorInitInfo& info);
    void invalidateKidsMode();
    void invalidatePlayerSeparate();
    void invalidateGameClear();
    void enableNoCap();
    void enableGiantWanderBoss();
    bool check(const al::IUseSceneObjHolder* sceneObjHolder) const;

private:
    bool mIsInvalidateKidsMode = false;
    bool mIsInvalidatePlayerSeparate = false;
    bool mIsInvalidateGameClear = false;
    bool mIsEnableNoCap = false;
    bool mIsEnableGiantWanderBoss = false;
    u8 _5[3] = {};
};

static_assert(sizeof(CapMessageGameDataChecker) == 0x8, "CapMessageGameDataChecker");
