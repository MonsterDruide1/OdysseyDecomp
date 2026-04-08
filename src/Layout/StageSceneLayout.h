#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class PlayerHolder;
class LayoutActor;
class LayoutInitInfo;
class SimpleLayoutAppearWaitEnd;
class SubCameraRenderer;
}  // namespace al

class CoinCounter;
class CounterLifeCtrl;
class ShineCounter;
class ShineChipLayoutParts;
class PlayGuideCamera;
class PlayGuideBgm;
class MapMini;
class GamePadPlayStyleInfo;

class StageSceneLayout : public al::NerveStateBase {
public:
    StageSceneLayout(const char* name, const al::LayoutInitInfo& initInfo,
                     const al::PlayerHolder* playerHolder,
                     const al::SubCameraRenderer* subCameraRenderer);

    void startActionAll(const char* actionName);
    void control() override;
    void updatePlayGuideMenuText();
    void setDirtyFlagForPlayGuideMenu();
    void start();
    void updateCounterParts();
    void tryAppearCoinCollectCounter();
    void startOnlyCoin(bool isOnlyCoin);
    void endWithoutCoin(bool isOnlyCoin);
    void end();
    bool isEnd() const;
    bool isWait() const;
    bool isActive() const;
    bool isEndLifeDemo() const;
    bool tryStartLifeDemo();
    void startCoinCountAnim(s32 coinNum);
    void startCoinCollectCountAnim(s32 coinNum);
    void appearCoinCounterForDemo();
    bool isEndCoinCountAnim() const;
    bool isEndShineChipCompleteAnim() const;
    void startShineChipCompleteAnim();
    void endShineChipCompleteAnim();
    bool tryStartDemoGetLifeMaxUpItem(bool isHackKoopa);
    bool isEndDemoGetLifeMaxUpItem() const;
    void killShineCount();
    void appearShineCountWait();
    void startCloset();
    void endCloset();
    void missEnd();
    void appearPlayGuideCamera();
    void exeAppear();
    bool isActionEndAll() const;
    void exeWait();
    void exeEnd();
    void exeEndWithoutCoin();
    void exeCoinCountAnim();
    void exeShineChipComplete();
    void exeShineCountAppear();
    void updateLifeCounter();
    void updateKidsModeLayout();
    void startShineCountAnim(bool isAddTenShines);
    bool isEndShineCountAnim() const;

private:
    CoinCounter* mCoinCounter;
    CounterLifeCtrl* mCounterLifeCtrl;
    ShineCounter* mShineCounter;
    CoinCounter* mCoinCollectCounter;
    ShineChipLayoutParts* mShineChipLayoutParts;
    PlayGuideCamera* mPlayGuideCamera;
    PlayGuideBgm* mPlayGuideBgm;
    MapMini* mMapMini;
    const al::PlayerHolder* mPlayerHolder;
    bool mIsOnlyCoin;
    al::SimpleLayoutAppearWaitEnd* mPlayGuideMenu;
    GamePadPlayStyleInfo* mGamePadPlayStyleInfo;
    al::LayoutActor* mLayoutActorKidsMode;
};

static_assert(sizeof(StageSceneLayout) == 0x80);
