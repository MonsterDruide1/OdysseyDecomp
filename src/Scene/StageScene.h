#pragma once

#include <math/seadQuat.h>

#include "Library/Scene/Scene.h"

#include "Layout/StageSceneLayout.h"

namespace al {
class LayoutActor;
}
class Shine;
class GameDataHolderAccessor;
class GameDataHolder;
class MapLayout;
class ProjectItemDirector;
class PlayerActorHakoniwa;
class ProjectDemoDirector;
class StageSceneLayout;

class StageScene : public al::Scene {
public:
    StageScene();
    ~StageScene() override;
    void init(const al::SceneInitInfo&) override;
    void appear() override;
    void kill() override;

    void control() override;
    void drawMain() const override;

    bool isEnableSave() const;

    // somewhere here at 0xE0: stageName

    unsigned char padding_d8[0x1F8];
    GameDataHolderAccessor* mHolder;
    unsigned char padding_2f8[0x20];
    StageSceneLayout* stageSceneLayout;
};

namespace rs {
void tryStartRaceStage(al::Scene*, GameDataHolder*);
bool updateNormalState(al::Scene*);
void updateEffectSystemEnv(al::Scene*);
bool updateNormalStateExcludeGraphics(al::Scene*);
bool updateBirdCarryMeat(al::Scene*);
void updateKitListExcludePlayer(al::Scene*);
void updateKitListExcludeCamera(al::Scene*);
void updateKitListExcludeCameraDemoChangeWorld(al::Scene*);
void updateKitListPlayerGroupNormal(al::Scene*);
void updateKitListDemoPlayerNoPauseEffect(al::Scene*);
void updateKitListDemoPlayerWithPauseEffect(al::Scene*);
void updateKitListDemoPlayerWithPauseEffectAndCalcAtEndClip(al::Scene*);
void updateKitListDemoNormalNoPauseEffect(al::Scene*);
void updateKitListDemoNormalWithPauseEffect(al::Scene*);
void updateKitListDemoCapWorldOpeninglWithPauseEffect(al::Scene*);
void updateKitListLayout(al::Scene*);
void updateKitListPostNoViewRenderer(al::Scene*);
void updateKitListLayoutIgnorePause(al::Scene*);
void updateKitListLayoutOnlyLayoutScene(al::Scene*);
void updateKitListPostWithPauseNormalEffectAndPlayerEffect(al::Scene*);
void updateKitListPostSnapShot(al::Scene*);
void setScenarioName(al::LayoutActor*, const al::Scene*);
void startTalkMsgTimeBalloonLayout(const al::Scene*);
void endPlayTalkMsgTimeBalloonLayout(const al::Scene*);
void endForceTalkMsgTimeBalloonLayout(const al::Scene*);
void changeDemoCommonProc(al::Scene*, ProjectItemDirector*);
void endDemoCommonProc(al::Scene*, ProjectItemDirector*);
void endDemoNormalShineGet(al::Scene*, ProjectItemDirector*);
bool tryOpenMap(al::IUseNerve*, const al::Scene*, const MapLayout*, const al::Nerve*);
bool tryChangeNextStage(GameDataHolder*, al::Scene*);
al::LiveActor* getPlayerActor(const al::Scene*);
void registerGraphicsPreset(const al::Scene*, const char*, const char*, const char*, bool);
void requestGraphicsPreset(const al::Scene*, const char*, const sead::Vector3f&);
void requestGraphicsCubeMap(const al::Scene*, const char*, const char*);
void requestGraphicsPresetAndCubeMap(const al::Scene*, const char*, const char*, const char*);
void requestGraphicsPresetAndCubeMap(const al::Scene*, const char*, const char*, const char*,
                                     const sead::Vector3f&);
void requestGraphicsPresetAndCubeMapFixedAngle(const al::Scene*, const char*, const char*,
                                               const char*);
void registerGraphicsPresetWorldMap(const al::Scene*);
void registerGraphicsPresetShop(const al::Scene*);
void registerGraphicsPresetMoonGet(const al::Scene*);
void registerGraphicsPresetCloset(const al::Scene*);
void registerGraphicsPresetPause(const al::Scene*);
void requestGraphicsPresetAndCubeMapWorldMap(const al::Scene*);
void requestGraphicsPresetAndCubeMapShop(const al::Scene*);
void requestGraphicsPresetAndCubeMapShopNoFixedAngle(const al::Scene*);
void requestGraphicsPresetAndCubeMapMoonGet(const al::Scene*);
void requestGraphicsPresetAndCubeMapMoonGetNoFixedAngle(const al::Scene*);
void requestGraphicsPresetAndCubeMapCloset(const al::Scene*);
void requestGraphicsPresetAndCubeMapClosetNoFixedAngle(const al::Scene*);
void requestGraphicsPresetAndCubeMapPause(const al::Scene*);
void requestGraphicsPresetAndCubeMapPauseNoFixedAngle(const al::Scene*);
void calcQuatPoseCheckpointWarpCap(sead::Quatf*, const sead::Vector3f&, const al::IUseCamera*);
ProjectDemoDirector* getDemoDirector(const al::Scene*);
Shine* getDemoShineActor(const al::Scene*);
bool isExistDemoShine(const al::Scene*);
}  // namespace rs
