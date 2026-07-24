#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Scene/Scene.h"

namespace al {
struct ActorInitInfo;
class LayoutInitInfo;
struct SceneInitInfo;
class StageInfo;
class WipeHolder;
}  // namespace al

class EndingScene : public al::Scene {
public:
    explicit EndingScene(al::WipeHolder* wipeHolder);

    void init(const al::SceneInitInfo& initInfo) override;
    void appear() override;
    void kill() override;
    void control() override;
    void drawMain() const override;

    void exePlay();
    bool isLoadEnd() const;
    void exeSkipProc();
    void initLayout(const al::LayoutInitInfo& layoutInitInfo);
    void initPlacement(const al::ActorInitInfo& actorInitInfo);
    void initPlacementSky(const al::StageInfo* stageInfo, const al::ActorInitInfo& actorInitInfo);
    void initPlacementDemo(const al::StageInfo* stageInfo, const al::ActorInitInfo& actorInitInfo);
    void initPlacementObject(const al::StageInfo* stageInfo, const al::ActorInitInfo& actorInitInfo,
                             const char* placementKey);

    void setLoadEnd(bool isLoadEnd) { mIsLoadEnd = isLoadEnd; }

private:
    sead::FixedSafeString<64> mStageName;
    u8 _130[0x10];
    bool mIsLoadEnd = false;
    u8 _141[7];
    void* _148 = nullptr;
    s32 _150 = 1;
    u8 _154[4];
    void* _158 = nullptr;
    s32 _160 = 0;
    u8 _164[4];
    void* _168 = nullptr;
    void* _170 = nullptr;
    void* _178 = nullptr;
    void* _180 = nullptr;
    al::WipeHolder* mWipeHolder = nullptr;
};

static_assert(sizeof(EndingScene) == 0x190, "EndingScene size");
