#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Scene/Scene.h"

namespace al {
struct ActorInitInfo;
class LayoutInitInfo;
class StageInfo;
class WipeHolder;
}  // namespace al

class DemoScene : public al::Scene {
public:
    DemoScene(al::WipeHolder* wipeHolder);
    ~DemoScene() override;

    void init(const al::SceneInitInfo& info) override;
    void appear() override;
    void control() override;
    void kill() override;
    void drawMain() const override;

    virtual void exePlay();
    virtual void initLayout(const al::LayoutInitInfo& layoutInitInfo);

    void initPlacement(const al::ActorInitInfo& actorInitInfo);
    bool isLoadEnd() const;
    void exeSkipProc();
    void initPlacementSky(const al::StageInfo* stageInfo, const al::ActorInitInfo& actorInitInfo);
    void initPlacementDemo(const al::StageInfo* stageInfo, const al::ActorInitInfo& actorInitInfo);
    void initPlacementObject(const al::StageInfo* stageInfo, const al::ActorInitInfo& actorInitInfo,
                             const char* suffix);

    void setLoadEnd() { mIsLoadEnd = true; }

private:
    sead::FixedSafeString<64> mStageName;
    void* _130 = nullptr;
    void* _138 = nullptr;
    bool mIsLoadEnd = false;
    u8 _141[7] = {};
    void* _148 = nullptr;
    s32 _150 = 1;
    s32 _154 = 0;
    void* _158 = nullptr;
    void* _160 = nullptr;
    void* _168 = nullptr;
    void* _170 = nullptr;
    s32 _178 = 0;
    s32 _17c = 0;
    al::WipeHolder* mWipeHolder = nullptr;
    void* _188 = nullptr;
    void* _190 = nullptr;
};

static_assert(sizeof(DemoScene) == 0x198);
