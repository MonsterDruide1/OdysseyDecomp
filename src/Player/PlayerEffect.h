#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al
class IPlayerModelChanger;
class PlayerModelHolder;

class PlayerEffect {
public:
    PlayerEffect(al::LiveActor*, const PlayerModelHolder*, const sead::Matrix34f*);
    void emitEffectWaterInOut(const sead::Matrix34f&, bool);
    void updateWaterSurfaceMtx(const al::WaterSurfaceFinder*);
    void tryEmitInvincibleEffect();
    void suspendInvincibleEffect();
    void tryDeleteInvincibleEffect();
    void restartInvincibleEffect();
    void updateInvincibleEffect(const IPlayerModelChanger*, bool);
    void clearRunEffect();
    bool isRunEffectDashFast() const;
    void tryStartRunEffectRunStart();
    void tryStartRunEffectRun();
    void tryStartRunEffectDash();
    void tryStartRunEffectDashFast();
    void tryStartRunEffectDashWaterSurface();
    void tryEmitRollingEffect();
    void tryDeleteRollingEffect();
    void clearStainEffect();
    void tryDeleteWetEffect();
    void tryDeleteStainPoisonEffect();
    void tryDeleteStainFireEffect();
    void tryDeleteStainIceEffect();
    void tryEmitWetEffect();
    void tryEmitStainPoisonEffect();
    void tryEmitStainFireEffect();
    void tryEmitStainIceEffect();
    void tryEmitSandSinkEffect(const sead::Vector3f&, const sead::Vector3f&, bool);
    void tryDeleteSandSinkEffect();
    void tryDeleteDamageFireRunEffect();

private:
    char filler[0xc0];
};

static_assert(sizeof(PlayerEffect) == 0xc0);
