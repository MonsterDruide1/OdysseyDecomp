#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class KoopaCap;

class KoopaWeaponHolder {
public:
    static KoopaWeaponHolder* create(const al::LiveActor*, const al::ActorInitInfo&, s32);

    KoopaWeaponHolder();
    void makeActorDeadAll();
    void killAll();
    void cancelAll();
    void* findDeadDamageBall() const;
    void* findDeadDamageBallBomb() const;
    void disappearAllDummyCap();
    void killAllDummyCapWaitHover();
    bool isAllAliveDummyCapWaitHover() const;
    KoopaCap* getDummyCap(s32) const;
    KoopaCap* findDeadDummyCap() const;
    void resetWeaponItemGenerateLimitByDamage();

private:
    void* mItemHolder = nullptr;
    void* mRingBeamEmitter = nullptr;
    void* mWeaponCapGroup = nullptr;
    void* mDamageBallGroup = nullptr;
    void* mDamageBallBombGroup = nullptr;
};

static_assert(sizeof(KoopaWeaponHolder) == 0x28);
