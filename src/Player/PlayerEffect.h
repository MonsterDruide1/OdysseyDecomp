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
    bool tryEmitInvincibleEffect();
    void suspendInvincibleEffect();
    bool tryDeleteInvincibleEffect();
    void restartInvincibleEffect();
    void updateInvincibleEffect(const IPlayerModelChanger*, bool);
    void clearRunEffect();
    bool isRunEffectDashFast() const;
    bool tryStartRunEffectRunStart();
    bool tryStartRunEffectRun();
    bool tryStartRunEffectDash();
    bool tryStartRunEffectDashFast();
    bool tryStartRunEffectDashWaterSurface();
    bool tryEmitRollingEffect();
    bool tryDeleteRollingEffect();
    void clearStainEffect();
    bool tryDeleteWetEffect();
    bool tryDeleteStainPoisonEffect();
    bool tryDeleteStainFireEffect();
    bool tryDeleteStainIceEffect();
    bool tryEmitWetEffect();
    bool tryEmitStainPoisonEffect();
    bool tryEmitStainFireEffect();
    bool tryEmitStainIceEffect();
    bool tryEmitSandSinkEffect(const sead::Vector3f&, const sead::Vector3f&, bool);
    bool tryDeleteSandSinkEffect();
    bool tryDeleteDamageFireRunEffect();

private:
    char filler[0xc0];
};

static_assert(sizeof(PlayerEffect) == 0xc0);
