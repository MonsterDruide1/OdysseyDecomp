#include "Boss/BossRaid/BossRaidElectricLine.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Boss/BossRaid/BossRaidElectric.h"

namespace {
NERVE_ACTION_IMPL(BossRaidElectricLine, Move);
NERVE_ACTIONS_MAKE_STRUCT(BossRaidElectricLine, Move);
}  // namespace

BossRaidElectricLine::BossRaidElectricLine(const char* name) : al::LiveActor(name) {
    mActors = new al::DeriveActorGroup<BossRaidElectric>("電撃アクティブリスト", 64);
}

void BossRaidElectricLine::init(const al::ActorInitInfo& info) {
    al::initNerveAction(this, "Move", &NrvBossRaidElectricLine.collector, 0);
    al::initActorWithArchiveName(this, info, "BossRaidElectricLine", nullptr);
    makeActorDead();
}

void BossRaidElectricLine::setBulletList(al::DeriveActorGroup<BossRaidElectric>* bulletList) {
    mBulletList = bulletList;
}

void BossRaidElectricLine::shot(const sead::Vector3f& pos, const sead::Vector3f& dir) {
    if (mBulletList == nullptr)
        return;

    BossRaidElectric* bullet = mBulletList->tryFindDeadDeriveActor();
    if (bullet == nullptr)
        return;

    bullet->shot(pos, dir, mPrevBullet, mActors);
    mPrevBullet = bullet;
    if (al::isDead(this)) {
        al::startNerveAction(this, "Move");
        makeActorAlive();
    }
}

void BossRaidElectricLine::exeMove() {
    s32 actorCount = mActors->getActorCount();
    if (actorCount == 0) {
        mPrevBullet = nullptr;
        makeActorDead();
        return;
    }

    sead::Vector3f nearPos = al::getTrans(this);
    f32 minDist = sead::Mathf::maxNumber();

    for (s32 i = 0; i < actorCount; i++) {
        BossRaidElectric* electric = mActors->getDeriveActor(i);

        sead::Vector3f calcPos;
        electric->calcNearPos(&calcPos, al::getPlayerPos(this, 0));
        const sead::Vector3f& playerPos = al::getPlayerPos(this, 0);

        f32 dist = sqrtf((playerPos - calcPos).squaredLength());
        if (dist < minDist) {
            nearPos.set(calcPos);
            minDist = dist;
        }
    }

    al::setTrans(this, nearPos);
}

void BossRaidElectricLine::killForce() {
    mActors->removeActorAll();
    mPrevBullet = nullptr;
    makeActorDead();
}
