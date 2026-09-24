#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

class IUsePlayerHack;

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;
}  // namespace al

class DemoHackFirstDirector : public al::LiveActor {
public:
    DemoHackFirstDirector(s32 demoType);

    void init(const al::ActorInitInfo& initInfo) override;
    void appear() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    void registRequestActor(al::LiveActor* actor);
    s32 getDemoStartWaitFrame() const;
    void setHackFirstDemo(al::LiveActor* actor, IUsePlayerHack* playerHack);
    void setHackFirstDemoCore(al::LiveActor* actor, IUsePlayerHack* playerHack);
    void noticeDemoStartToDemoHackFirstDirector();
    void skipDemo();
    void endDemo();
    void updateOnlyDemoGraphics();
    bool tryEndDemo();
    bool isEndHackFirstDemo() const;
    void exeWatch();
    void exeDemo();
    void exeShowMessage();
    void startShowMessageFrog();
    void exeEnd();

private:
    u8 _108[0x178 - sizeof(al::LiveActor)];
};

static_assert(sizeof(DemoHackFirstDirector) == 0x178);
