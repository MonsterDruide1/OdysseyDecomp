#include "Scene/StageScene.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneUtil.h"

#include "Layout/KidsModeLayoutAccessor.h"
#include "Layout/StageSceneLayout.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/AmiiboUtil.h"
#include "Util/DemoUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(StageScene, StartStageBgm);
NERVE_IMPL(StageScene, CollectBgm);
NERVE_IMPL(StageScene, CollectionList);
NERVE_IMPL(StageScene, MiniGameRanking);
NERVE_IMPL(StageScene, Shop);
NERVE_IMPL(StageScene, Closet);
NERVE_IMPL(StageScene, DemoShineGet);
NERVE_IMPL(StageScene, DemoTalk);
NERVE_IMPL(StageScene, Radicon);
NERVE_IMPL(StageScene, Warp);
NERVE_IMPL(StageScene, DemoGetLifeMaxUpItem);
NERVE_IMPL(StageScene, DemoWorldIntroCamera);
NERVE_IMPL_(StageScene, DemoWorldIntroCameraBeforeAppearElectricDemo, DemoWorldIntroCamera);
NERVE_IMPL(StageScene, SnapShot);
NERVE_IMPL(StageScene, DemoShineMainGet);
NERVE_IMPL(StageScene, DemoShineGrandGet);
NERVE_IMPL(StageScene, SkipDemo);
NERVE_IMPL(StageScene, Miss);
NERVE_IMPL(StageScene, DemoCarryMeat);
NERVE_IMPL(StageScene, RaceYukimaru);
NERVE_IMPL(StageScene, RaceYukimaruTutorial);
NERVE_IMPL(StageScene, RaceManRace);
NERVE_IMPL(StageScene, DemoTitleLogo);
NERVE_IMPL(StageScene, Pause);
NERVE_IMPL(StageScene, ArriveAtCheckpoint);
NERVE_IMPL(StageScene, TimeBalloon);
NERVE_IMPL(StageScene, DemoScenarioCamera);
NERVE_IMPL(StageScene, AppearFromWorldWarpHole);
NERVE_IMPL_(StageScene, DemoStageStartOpening, DemoStageStart);
NERVE_IMPL(StageScene, DemoStageStart);
NERVE_IMPL(StageScene, DemoPlayerDown);
NERVE_IMPL(StageScene, DemoStageStartCapManHeroTalk);
NERVE_IMPL_(StageScene, DemoCapManHeroTalkMoonRock, DemoStageStartCapManHeroTalk);
NERVE_IMPL(StageScene, DemoReturnToHome);
NERVE_IMPL(StageScene, DemoHackStart);
NERVE_IMPL(StageScene, Play);
NERVE_IMPL_(StageScene, DemoWithPlayerUseCoinNoFirst, DemoWithPlayerUseCoin);
NERVE_IMPL(StageScene, DemoDamageStopPlayer);
NERVE_IMPL(StageScene, DemoSceneStartPlayerWalk);
NERVE_IMPL(StageScene, DemoRiseMapParts);
NERVE_IMPL(StageScene, DemoShineChipComplete);
NERVE_IMPL(StageScene, DemoHackStartFirst);
NERVE_IMPL(StageScene, DemoNormalBazookaElectric);
NERVE_IMPL(StageScene, DemoWithPlayerUseCoin);
NERVE_IMPL(StageScene, DemoWithPlayer);
NERVE_IMPL(StageScene, DemoNormal);
NERVE_IMPL(StageScene, Amiibo);
NERVE_IMPL_(StageScene, DemoStageStartElectricDemo, DemoStageStart);
NERVE_IMPL(StageScene, WaitStartWarpForSession);
NERVE_IMPL(StageScene, DemoCountCoin);
NERVE_IMPL(StageScene, WarpToCheckpoint);
NERVE_IMPL(StageScene, DemoAppearFromHome);
NERVE_IMPL(StageScene, DemoRisePyramid);
NERVE_IMPL(StageScene, WaitWarpToCheckpoint);

NERVES_MAKE_STRUCT(StageScene, StartStageBgm, CollectBgm, CollectionList, MiniGameRanking, Shop,
                   Closet, DemoShineGet, DemoTalk, Radicon, Warp, DemoGetLifeMaxUpItem,
                   DemoWorldIntroCamera, DemoWorldIntroCameraBeforeAppearElectricDemo, SnapShot,
                   DemoShineMainGet, DemoShineGrandGet, SkipDemo, Miss, DemoCarryMeat, RaceYukimaru,
                   RaceYukimaruTutorial, RaceManRace, DemoTitleLogo, Pause, ArriveAtCheckpoint,
                   TimeBalloon, DemoScenarioCamera, AppearFromWorldWarpHole, DemoStageStartOpening,
                   DemoStageStart, DemoPlayerDown, DemoStageStartCapManHeroTalk,
                   DemoCapManHeroTalkMoonRock, DemoReturnToHome, DemoHackStart, Play,
                   DemoWithPlayerUseCoinNoFirst, DemoDamageStopPlayer, DemoSceneStartPlayerWalk,
                   DemoRiseMapParts, DemoShineChipComplete, DemoHackStartFirst,
                   DemoNormalBazookaElectric, DemoWithPlayerUseCoin, DemoWithPlayer, DemoNormal,
                   Amiibo, DemoStageStartElectricDemo, WaitStartWarpForSession, DemoCountCoin,
                   WarpToCheckpoint, DemoAppearFromHome, DemoRisePyramid, WaitWarpToCheckpoint)
}  // namespace

void StageScene::exeDemoGetLifeMaxUpItem() {
    if (mIsUpdateKitAndGraphics) {
        al::updateKit(this);
        al::updateGraphicsPrev(this);
        mIsUpdateKitAndGraphics = false;
        return;
    }

    if (al::updateNerveState(this)) {
        rs::endDemoCommonProc(this, mProjectItemDirector);
        al::updateKitListPostOnNerveEnd(this);
        al::setNerve(this, &NrvStageScene.Play);
        return;
    }

    al::updateKitListPostOnNerveEnd(this);
}

void StageScene::exeDemoHackStart() {
    bool isRadicon =
        !GameDataFunction::isMainStage(this) && rs::isPlayerHackRadiconNpc(this->mDemoShine);

    if (al::isFirstStep(this)) {
        if (isRadicon) {
            mSceneLayout->end();
            rs::setKidsModeLayoutDisable(this);
        }
        rs::killCoinCollectHintEffect(this);
    }

    al::updateKitListPrev(this);
    rs::updateKitListDemoPlayerWithPauseEffect(this);
    if (isRadicon)
        mSceneLayout->update();
    al::updateKitListPostDemoWithPauseNormalEffect(this);

    if (!rs::isActiveDemo(this)) {
        endDemoAndChangeNerve();
        if (isRadicon)
            al::setNerve(this, &NrvStageScene.Radicon);
        return;
    }

    al::updateKitListPostOnNerveEnd(this);
}
