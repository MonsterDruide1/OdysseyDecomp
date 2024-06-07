#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class IUseEffectKeeper;
class PtclSystem;

struct EffectSystemInfo {
    s32 _0;
    PtclSystem* mPtclSystem;
    void* _10;
    s32 _18;
};

static_assert(sizeof(EffectSystemInfo) == 0x20);

void emitEffectCurrentPos(IUseEffectKeeper*, const char*);
void emitEffect(IUseEffectKeeper*, const char*, const sead::Vector3f*);
bool tryEmitEffect(IUseEffectKeeper*, const char*, const sead::Vector3f*);
void deleteEffect(IUseEffectKeeper*, const char*);
bool tryDeleteEffect(IUseEffectKeeper*, const char*);
void deleteEffectAll(IUseEffectKeeper*);
void tryKillEmitterAndParticleAll(IUseEffectKeeper*);
void onCalcAndDrawEffect(IUseEffectKeeper*);
void offCalcAndDrawEffect(IUseEffectKeeper*);
void forceSetStopCalcAndDraw(IUseEffectKeeper*, bool);
bool isEffectEmitting(const IUseEffectKeeper*, const char*);
void setEffectEmitRatio(IUseEffectKeeper*, const char*, f32);
void setEffectAllScale(IUseEffectKeeper*, const char*, const sead::Vector3f&);
void setEffectEmitterVolumeScale(IUseEffectKeeper*, const char*, const sead::Vector3f&);
void setEffectParticleScale(IUseEffectKeeper*, const char*, f32);
void setEffectParticleScale(IUseEffectKeeper*, const char*, const sead::Vector3f&);
void setEffectParticleAlpha(IUseEffectKeeper*, const char*, f32);
void setEffectParticleColor(IUseEffectKeeper*, const char*, const sead::Color4f&);
void setParticleLifeScale(IUseEffectKeeper*, const char*, f32);
void setEffectParticleDirectionalVel(IUseEffectKeeper*, const char*, f32);
void setEffectFollowPosPtr(IUseEffectKeeper*, const char*, const sead::Vector3f*);
void setEffectFollowMtxPtr(IUseEffectKeeper*, const char*, const sead::Matrix34f*);
void setEffectNamedMtxPtr(IUseEffectKeeper*, const char*, const sead::Matrix34f*);
bool trySetEffectNamedMtxPtr(IUseEffectKeeper*, const char*, const sead::Matrix34f*);
bool tryUpdateEffectMaterialCode(IUseEffectKeeper*, const char*);
void resetEffectMaterialCode(IUseEffectKeeper*);
void updateEffectMaterialWater(IUseEffectKeeper*, bool);
void updateEffectMaterialWet(IUseEffectKeeper*, bool);
void updateEffectMaterialPuddle(IUseEffectKeeper*, bool);
}  // namespace al
