#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class PlayerHolder;
class LayoutActor;
class LayoutInitInfo;
class SubCameraRenderer;
}  // namespace al

class CoinCounter;
class CounterLifeCtrl;
class ShineCounter;
class ShineChipLayoutParts;
class PlayGuideCamera;
class PlayGuideBgm;
class MapMini;
class SimpleLayoutAppearWaitEnd;
class GamePadPlayStyleInfo;

class StageSceneLayout : public al::NerveStateBase {
public:
    StageSceneLayout(const char*, const al::LayoutInitInfo&, const al::PlayerHolder*,
                     const al::SubCameraRenderer*);
    ~StageSceneLayout();

    void startActionAll(const char*);
    void control() override;
    void updatePlayGuideMenuText();
    void setDirtyFlagForPlayGuideMenu();
    void start();
    void updateCounterParts();
    void tryAppearCoinCollectCounter();
    void startOnlyCoin(bool);
    void endWithoutCoin(bool);
    void end();
    bool isEnd() const;
    bool isWait() const;
    bool isActive() const;
    bool isEndLifeDemo() const;
    bool tryStartLifeDemo();
    void startCoinCountAnim(s32);
    void startCoinCollectCountAnim(s32);
    void appearCoinCounterForDemo();
    bool isEndCoinCountAnim() const;
    bool isEndShineChipCompleteAnim() const;
    void startShineChipCompleteAnim();
    void endShineChipCompleteAnim();
    bool tryStartDemoGetLifeMaxUpItem(bool);
    bool isEndDemoGetLifeMaxUpItem() const;
    void killShineCount();
    void appearShineCountWait();
    void startCloset();
    void endCloset();
    void missEnd();
    void appearPlayGuideCamera();
    void exeAppear();
    bool isActionEndAll();
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
    bool _60;
    SimpleLayoutAppearWaitEnd* mPlayGuideMenu;
    GamePadPlayStyleInfo* mGamePadPlayStyleInfo;
    al::LayoutActor* mLayoutActorKidsMode;
};

static_assert(sizeof(StageSceneLayout) == 0x80);
