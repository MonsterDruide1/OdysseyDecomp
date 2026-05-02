#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"
#include "Library/Scene/Scene.h"
#include "Library/Sequence/Sequence.h"

class WorldResourceLoader;

namespace al {
class AsyncFunctorThread;
}  // namespace al

class HakoniwaStateDeleteScene : public al::HostStateBase<al::Sequence> {
public:
    HakoniwaStateDeleteScene(al::Sequence* sequence, WorldResourceLoader* resourceLoader);
    ~HakoniwaStateDeleteScene() override;

    void appear() override;
    void kill() override;

    void deleteScene();
    void start(al::Scene* scene, bool destroyResource, bool finalizeAudio, s32 stopSeFadeFrames);
    void exePrepare();
    void exeFinalizeAudio();
    void exeDeleteScene();

private:
    al::Scene* mScene = nullptr;
    al::AsyncFunctorThread* mDeleteThread = nullptr;
    bool mIsDestroyResource = false;
    bool mIsFinalizeAudio = false;
    u8 _32[2] = {};
    s32 mStopSeFadeFrames = 0;
    WorldResourceLoader* mResourceLoader = nullptr;
};

static_assert(sizeof(HakoniwaStateDeleteScene) == 0x40);
