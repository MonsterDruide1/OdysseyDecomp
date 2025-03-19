#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Sequence/Sequence.h"

#include "System/GameDataHolderAccessor.h"

class BootLayout;
class CollectBgmPlayer;
class HakoniwaStateBootLoadData;
class HakoniwaStateDeleteScene;
class HakoniwaStateDemoEnding;
class HakoniwaStateDemoOpening;
class HakoniwaStateDemoWorldWarp;
class HakoniwaStateSimpleDemo;
class LoadLayoutCtrl;
class TimeBalloonSequenceInfo;
class WorldResourceLoader;

namespace sead {
class Heap;
}  // namespace sead

namespace al {
class AsyncFunctorThread;
class AudioBusSendFader;
class EffectSystem;
class GamePadSystem;
class LayoutKit;
class Scene;
class ScreenCaptureExecutor;
class SeadAudioPlayer;
class SimpleAudioUser;
class SimpleLayoutAppearWaitEnd;
class WipeHolder;

}  // namespace al

class HakoniwaSequence : public al::Sequence {
public:
    HakoniwaSequence(const char* name);

    void init(const al::SequenceInitInfo& info) override;
    void update() override;
    void drawMain() const override;

    void updatePadSystem();
    void destroySceneHeap(bool destroyResource);
    void initSystem();
    bool isEnableSave() const;

    void exeBootLoadData();
    void exeDemoOpening();
    void exeLoadWorldResource();
    void exeLoadWorldResourceWithBoot();
    void exeLoadStage();
    void exePlayStage();
    void exeDemoWorldWarp();
    void exeDemoEnding();
    void exeDestroy();
    void exeMiss();
    void exeMissCoinSub();
    void exeMissEnd();
    void exeDemoLava();
    void exeFadeToNewGame();
    void exeChangeLanguage();
    void exeWaitWriteData();
    void exeWaitLoadData();
    void exeWaitWriteDataModeChange();
    void exeWaitLoadDataModeChange();

    bool isDisposable() const override;
    al::Scene* getCurrentScene() const override;

private:
    al::Scene* mCurrentScene;
    GameDataHolderAccessor mGameDataHolderAccessor;
    al::GamePadSystem* mGamePadSystem;
    HakoniwaStateDemoOpening* mStateDemoOpening;
    HakoniwaStateDemoEnding* mStateDemoEnding;
    HakoniwaStateDemoWorldWarp* mStateDemoWorldWarp;
    HakoniwaStateSimpleDemo* mStateSimpleDemo;
    HakoniwaStateBootLoadData* mStateBootLoadData;
    HakoniwaStateDeleteScene* mStateDeleteScene;
    al::LayoutKit* mLayoutKit;
    bool _100;
    sead::FixedSafeString<128> mStageName;
    s32 mNextScenarioNum;
    s32 _1a4;
    al::ScreenCaptureExecutor* mScreenCaptureExecutor;
    al::WipeHolder* mWipeHolder;
    bool mIsMissEnd;
    al::SimpleLayoutAppearWaitEnd* mCounterMiss;
    s32 mCurrentCoins;
    s32 mFinalCoins;
    BootLayout* mBootLayout;
    al::EffectSystem* mEffectSystem;
    al::AsyncFunctorThread* mInitThread;
    bool mIsInitialized;
    al::SeadAudioPlayer* mSeAudioPlayer;
    al::SeadAudioPlayer* mBgmAudioPlayer;
    al::AudioBusSendFader* mAudioBusSendFader;
    WorldResourceLoader* mResourceLoader;
    sead::Heap* mPlayerResourceHeap;
    sead::FixedSafeString<128> mCapName;
    sead::FixedSafeString<128> mCostumeName;
    al::SimpleAudioUser* mPlayerAudioUser;
    bool mIsHackEnd;
    TimeBalloonSequenceInfo* mBalloonSeqInfo;
    CollectBgmPlayer* mCollectBgmPlayer;
    sead::FixedSafeString<128> mLanguage;
    s32 mFileId;
    LoadLayoutCtrl* mLoadLayoutCtrl;
    bool mIsKidsMode;
};

static_assert(sizeof(HakoniwaSequence) == 0x418, "HakoniwaSequnce size");
