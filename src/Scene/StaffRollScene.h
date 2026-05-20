#pragma once

#include "Library/Message/IUseMessageSystem.h"
#include "Library/Scene/Scene.h"

namespace al {
class WipeHolder;
class StageInfo;
}  // namespace al

class StaffRollScene : public al::Scene, public al::IUseMessageSystem {
public:
    StaffRollScene(al::WipeHolder* wipeHolder = nullptr);
    ~StaffRollScene() override;

    void init(const al::SceneInitInfo& info) override;
    void initRollLineData();
    void appear() override;
    void control() override;
    void kill() override;
    void drawMain() const override;

    void exeTitleLogo();
    void exeRollLine();
    void initCurrentRollLines();
    void initSlideShowFrame();
    void updateRollLine();
    void updateSlideShow();
    bool tryChangeToSkipConfirm(const al::Nerve*);
    void exeThank();
    void dropAllRollLine();
    void exeShowPicture();
    void exeRight();
    void exeConfirm();
    void exeSkipProc();
    void exeSkipWipe();
    void killRollLine();
    u32 getMyNerveStep() const;
    virtual void initLayout(const al::LayoutInitInfo&);
    f32 calcNameRollTimeRate(f32);
    void dropCurrentToStock();
    void initPlacement(const al::ActorInitInfo&);
    void initPlacementSky(const al::StageInfo*, const al::ActorInitInfo&);
    void initPlacementObject(const al::StageInfo*, const al::ActorInitInfo&, const char*);
    void initPlacementDemo(const al::StageInfo*, const al::ActorInitInfo&);
    bool isCanSkip();

    const al::MessageSystem* getMessageSystem() const override;

private:
    s8 filler_e0[0x6250 - sizeof(al::Scene) - sizeof(al::IUseMessageSystem)];
};
