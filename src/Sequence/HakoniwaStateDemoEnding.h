#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

#include "System/GameDataHolderAccessor.h"

class GameDataHolder;
class HakoniwaSequence;
class HakoniwaStateDeleteScene;
class WorldResourceLoader;

namespace al {
class Scene;
class ScreenCaptureExecutor;
class WipeHolder;
}  // namespace al

class HakoniwaStateDemoEnding : public al::HostStateBase<HakoniwaSequence> {
public:
    HakoniwaStateDemoEnding(HakoniwaSequence* sequence, al::WipeHolder* wipeHolder,
                            al::ScreenCaptureExecutor* screenCaptureExecutor,
                            HakoniwaStateDeleteScene* stateDeleteScene,
                            WorldResourceLoader* resourceLoader, GameDataHolder* gameDataHolder);
    ~HakoniwaStateDemoEnding() override;

    void init() override;
    void appear() override;
    void kill() override;

    void exeLoadWorldResource();
    void exeLoadWorldResourceSecond();
    void exeLoad();
    void exeDemoEnding();
    void exeEndEnding();
    void exeLoadStaffRoll();
    void exeDemoStaffRoll();
    void exeEnd();

private:
    al::Scene* mCurrentScene = nullptr;
    al::WipeHolder* mWipeHolder = nullptr;
    al::ScreenCaptureExecutor* mScreenCaptureExecutor = nullptr;
    HakoniwaStateDeleteScene* mStateDeleteScene = nullptr;
    WorldResourceLoader* mResourceLoader = nullptr;
    GameDataHolderAccessor mGameDataHolder;
};

static_assert(sizeof(HakoniwaStateDemoEnding) == 0x50, "HakoniwaStateDemoEnding size");
