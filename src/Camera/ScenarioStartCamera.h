#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Demo/IUseDemoSkip.h"

class ScenarioStartCamera : public al::LiveActor, public IUseDemoSkip {
public:
    ScenarioStartCamera(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void appear() override;
    void kill() override;
    const sead::Matrix34f* getBaseMtx() const override;

    bool isEnableStart() const;
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;

    void end();
    void exePlay();
    void* getPoser(s32 index) const;  // TODO unknown return type
    void exePlayAnim();
    void exeEnd();
    s32 getMaxPlayStep();

private:
    void* _padding[0x8];
};

static_assert(sizeof(ScenarioStartCamera) == 0x150);
