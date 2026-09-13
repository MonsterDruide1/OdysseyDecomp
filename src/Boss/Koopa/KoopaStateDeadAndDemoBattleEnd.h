#pragma once

#include "Library/Nerve/NerveStateBase.h"

#include "Demo/IUseDemoSkip.h"

namespace al {
class LiveActor;
}

class KoopaCap;
class KoopaDemoExecutor;
class KoopaWeaponHolder;

class KoopaStateDeadAndDemoBattleEnd : public al::ActorStateBase, public IUseDemoSkip {
public:
    KoopaStateDeadAndDemoBattleEnd(al::LiveActor* actor, KoopaDemoExecutor* demoExecutor,
                                   KoopaCap* cap, KoopaWeaponHolder* weaponHolder,
                                   al::LiveActor* demoActor, bool isKoopaLv2);

    void appear() override;
    void kill() override;
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;

    void exeDead();
    void exeStart();
    void exeDemo();
    void exeSkip();

private:
    KoopaDemoExecutor* mDemoExecutor = nullptr;
    KoopaCap* mCap = nullptr;
    KoopaWeaponHolder* mWeaponHolder = nullptr;
    al::LiveActor* mDemoActor = nullptr;
    bool mIsKoopaLv2 = false;
};

static_assert(sizeof(KoopaStateDeadAndDemoBattleEnd) == 0x50);
