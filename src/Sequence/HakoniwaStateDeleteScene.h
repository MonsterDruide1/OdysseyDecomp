#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"
#include "Library/Sequence/Sequence.h"

class WorldResourceLoader;

namespace al {
class AsyncFunctorThread;
class Scene;
}  // namespace al

class HakoniwaStateDeleteScene : public al::HostStateBase<al::Sequence> {
public:
    HakoniwaStateDeleteScene(al::Sequence* sequence, WorldResourceLoader* resourceLoader);

    void appear() override;
    void kill() override;

    void deleteScene();
    void start(al::Scene* scene, bool destroyResource, bool finalizeAudio, s32 stopSeFrame);
    void exePrepare();
    void exeFinalizeAudio();
    void exeDeleteScene();

private:
    al::Scene* mScene = nullptr;
    al::AsyncFunctorThread* mDeleteSceneThread = nullptr;
    bool mIsDestroyResource = false;
    bool mIsFinalizeAudio = false;
    s32 mStopSeFrame = 0;
    WorldResourceLoader* mResourceLoader = nullptr;
};

static_assert(sizeof(HakoniwaStateDeleteScene) == 0x40, "HakoniwaStateDeleteScene size");
