#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
template <class T>
class DeriveActorGroup;
}

class BossRaidElectric;

class BossRaidElectricLine : public al::LiveActor {
public:
    BossRaidElectricLine(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void setBulletList(al::DeriveActorGroup<BossRaidElectric>* bulletList);
    void shot(const sead::Vector3f& pos, const sead::Vector3f& dir);
    void exeMove();
    void killForce();

    void resetPrevBullet() { mPrevBullet = nullptr; }

private:
    BossRaidElectric* mPrevBullet = nullptr;
    al::DeriveActorGroup<BossRaidElectric>* mActors = nullptr;
    al::DeriveActorGroup<BossRaidElectric>* mBulletList = nullptr;
};

static_assert(sizeof(BossRaidElectricLine) == 0x120);
