#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

class WorldResourceLoader;

namespace al {
class AsyncFunctorThread;
class Scene;
class Sequence;
}  // namespace al

class HakoniwaStateDeleteScene : public al::HostStateBase<al::Sequence> {
public:
    HakoniwaStateDeleteScene(al::Sequence* sequence, WorldResourceLoader* resourceLoader);
    ~HakoniwaStateDeleteScene() override;

    void appear() override;
    void kill() override;

    void start(al::Scene* scene, bool isDestroySceneHeap, bool isFinalizeAudio, s32 stopSeIndex);
    void deleteScene();
    void exePrepare();
    void exeFinalizeAudio();
    void exeDeleteScene();

private:
    al::Scene* mScene = nullptr;
    al::AsyncFunctorThread* mDeleteSceneThread = nullptr;
    bool mIsDestroySceneHeap = false;
    bool mIsFinalizeAudio = false;
    s32 mStopSeIndex = 0;
    WorldResourceLoader* mResourceLoader = nullptr;
};

static_assert(sizeof(HakoniwaStateDeleteScene) == 0x40);
