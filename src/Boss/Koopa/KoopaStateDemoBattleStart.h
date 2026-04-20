#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

#include "Demo/IUseDemoSkip.h"

namespace al {
struct ActorInitInfo;
class AreaObjGroup;
template <class T>
class DeriveActorGroup;
}  // namespace al

class CapAppearMapParts;
class KoopaDemoExecutor;

class KoopaStateDemoBattleStart : public al::ActorStateBase, public IUseDemoSkip {
public:
    KoopaStateDemoBattleStart(al::LiveActor* actor, KoopaDemoExecutor* demoExecutor, s32 level);

    void initDemoStartAreaGroup(const al::ActorInitInfo& info);
    void setCapAppearMapPartsGroup(al::DeriveActorGroup<CapAppearMapParts>* group);
    void appear() override;
    void kill() override;
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;

    void exePrepare();
    void exeStart();
    void exeDemo();
    void exeSkip();

private:
    KoopaDemoExecutor* mDemoExecutor;
    s32 mBattleLevel;
    al::AreaObjGroup* mDemoStartAreaGroup;
    al::DeriveActorGroup<CapAppearMapParts>* mCapAppearMapPartsGroup;
    bool mIsSkipped;
};

static_assert(sizeof(KoopaStateDemoBattleStart) == 0x50);
