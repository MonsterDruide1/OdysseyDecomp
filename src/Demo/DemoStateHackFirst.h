#pragma once

#include "Library/Nerve/NerveStateBase.h"

#include "Demo/IUseDemoSkip.h"

class DemoHackFirstDirector;
class IUsePlayerHack;

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

namespace DemoName {
extern const char* cHackStartFirstTimeFrog;
extern const char* cHackStartFirstTimeKoopa;
}  // namespace DemoName

class DemoStateHackFirst : public al::ActorStateBase, public IUseDemoSkip {
public:
    DemoStateHackFirst(al::LiveActor* actor, const al::ActorInitInfo& initInfo);

    void appear() override;
    void kill() override;
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;
    void endDemo();
    void updateOnlyDemoGraphics() override;
    bool tryHackFirstDemoWait(const al::SensorMsg* message);
    bool isEnableShowHackDemo() const;
    bool tryHackFirst(IUsePlayerHack** playerHack, const al::SensorMsg* message,
                      al::HitSensor* other, al::HitSensor* self);
    void exeDemo();
    void exeEnd();

private:
    s32 mDemoType = 0;
    IUsePlayerHack* mPlayerHack = nullptr;
    DemoHackFirstDirector* mDemoHackFirstDirector = nullptr;
    s32 mDemoStartWaitFrame = 0;
};

static_assert(sizeof(DemoStateHackFirst) == 0x48);

namespace rs {
DemoStateHackFirst* tryCreateDemoStateHackFirst(al::LiveActor* actor,
                                                const al::ActorInitInfo& initInfo);
}  // namespace rs
