#pragma once

#include "Library/Nerve/NerveStateBase.h"

#include "Demo/IUseDemoSkip.h"

namespace al {
struct ActorInitInfo;
class AreaObjGroup;
class LiveActor;
}  // namespace al

class KoopaDemoExecutor;

class KoopaStateDemoClashBasement : public al::ActorStateBase, public IUseDemoSkip {
public:
    static KoopaStateDemoClashBasement*
    tryCreate(al::LiveActor* actor, const al::ActorInitInfo& info, KoopaDemoExecutor* demoExecutor);

    KoopaStateDemoClashBasement(al::LiveActor* actor, KoopaDemoExecutor* demoExecutor,
                                const al::AreaObjGroup* areaGroup);

    void appear() override;
    void kill() override;
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;

    void exePrepare();
    void exeStart();
    void exeDemo();
    void exeSkip();
    void exeEnd();

private:
    KoopaDemoExecutor* mDemoExecutor = nullptr;
    const al::AreaObjGroup* mAreaGroup = nullptr;
};

static_assert(sizeof(KoopaStateDemoClashBasement) == 0x38);
