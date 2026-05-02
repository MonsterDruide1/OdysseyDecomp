#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

class BootLayout;
class DemoSceneWithCinemaCaption;
class FirstSequenceScene;
class HakoniwaSequence;
class HakoniwaStateDeleteScene;
class LoadLayoutCtrl;
class WorldResourceLoader;

namespace al {
class AsyncFunctorThread;
class LayoutInitInfo;
class ScreenCaptureExecutor;
class SimpleLayoutAppearWaitEnd;
class WipeHolder;
}  // namespace al

class HakoniwaStateDemoOpening : public al::HostStateBase<HakoniwaSequence> {
public:
    HakoniwaStateDemoOpening(HakoniwaSequence* sequence, al::WipeHolder* wipeHolder,
                             al::ScreenCaptureExecutor* screenCaptureExecutor,
                             WorldResourceLoader* resourceLoader, BootLayout* bootLayout,
                             const al::LayoutInitInfo& layoutInitInfo,
                             HakoniwaStateDeleteScene* stateDeleteScene,
                             al::AsyncFunctorThread* initThread, LoadLayoutCtrl* loadLayoutCtrl);
    ~HakoniwaStateDemoOpening() override;

    void init() override;
    void appear() override;

    void startSecond();
    void exeBoot();
    void exeLoadFirstSequence();
    void exeFirstSequence();
    void exeDestroyFirstSequence();
    void exeLoad();
    void exeFadeToText();
    void exeTextAppear();
    void exeText();
    void exeFadeToDemo();
    void exeDemoOpening();
    void exeEnd();

private:
    FirstSequenceScene* mFirstSequenceScene;
    DemoSceneWithCinemaCaption* mDemoScene;
    al::WipeHolder* mWipeHolder;
    BootLayout* mBootLayout;
    al::SimpleLayoutAppearWaitEnd* mStartOpeningDemo;
    al::ScreenCaptureExecutor* mScreenCaptureExecutor;
    WorldResourceLoader* mResourceLoader;
    bool mIsSecond;
    HakoniwaStateDeleteScene* mStateDeleteScene;
    al::AsyncFunctorThread* mInitThread;
    LoadLayoutCtrl* mLoadLayoutCtrl;
};

static_assert(sizeof(HakoniwaStateDemoOpening) == 0x78);
