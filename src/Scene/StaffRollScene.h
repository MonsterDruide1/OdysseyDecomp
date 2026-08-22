#pragma once

#include <basis/seadTypes.h>

#include "Library/Message/IUseMessageSystem.h"
#include "Library/Scene/Scene.h"

namespace al {
struct ActorInitInfo;
class LayoutInitInfo;
class MessageSystem;
class Nerve;
struct SceneInitInfo;
class StageInfo;
class WipeHolder;
}  // namespace al

class StaffRollScene : public al::Scene, public al::IUseMessageSystem {
public:
    explicit StaffRollScene(al::WipeHolder* wipeHolder);

    void init(const al::SceneInitInfo& initInfo) override;
    void appear() override;
    void kill() override;
    void control() override;
    void drawMain() const override;
    const al::MessageSystem* getMessageSystem() const override;

    void initRollLineData();
    void exeTitleLogo();
    void exeRollLine();
    void initCurrentRollLines();
    void initSlideShowFrame();
    void updateRollLine();
    void updateSlideShow();
    void tryChangeToSkipConfirm(const al::Nerve* nextNerve);
    void exeThank();
    void dropAllRollLine();
    void exeShowPicture();
    void exeRight();
    void exeConfirm();
    void exeSkipProc();
    void exeSkipWipe();
    void killRollLine();
    s32 getMyNerveStep() const;
    void initLayout(const al::LayoutInitInfo& layoutInitInfo);
    f32 calcNameRollTimeRate(f32 rate);
    void dropCurrentToStock();
    void initPlacement(const al::ActorInitInfo& actorInitInfo);
    void initPlacementSky(const al::StageInfo* stageInfo, const al::ActorInitInfo& actorInitInfo);
    void initPlacementObject(const al::StageInfo* stageInfo, const al::ActorInitInfo& actorInitInfo,
                             const char* placementKey);
    void initPlacementDemo(const al::StageInfo* stageInfo, const al::ActorInitInfo& actorInitInfo);
    bool isCanSkip();

private:
    const al::MessageSystem* mMessageSystem = nullptr;
    al::WipeHolder* mWipeHolder = nullptr;
    u8 _f0[0x6160];
};

static_assert(sizeof(StaffRollScene) == 0x6250, "StaffRollScene size");
