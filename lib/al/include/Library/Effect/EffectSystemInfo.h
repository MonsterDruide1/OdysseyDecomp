#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class IUseEffectKeeper;

void emitEffectCurrentPos(IUseEffectKeeper*, const char*);
void emitEffect(IUseEffectKeeper*, const char*, const sead::Vector3<f32>*);
bool tryEmitEffect(IUseEffectKeeper*, const char*, const sead::Vector3<f32>*);
void deleteEffect(IUseEffectKeeper*, const char*);
bool tryDeleteEffect(IUseEffectKeeper*, const char*);
void deleteEffectAll(IUseEffectKeeper*);
void tryKillEmitterAndParticleAll(IUseEffectKeeper*);
void onCalcAndDrawEffect(IUseEffectKeeper*);
void offCalcAndDrawEffect(IUseEffectKeeper*);
void forceSetStopCalcAndDraw(IUseEffectKeeper*, bool);
bool isEffectEmitting(IUseEffectKeeper const*, const char*);
void setEffectEmitRatio(IUseEffectKeeper*, const char*, f32);
void setEffectAllScale(IUseEffectKeeper*, const char*, const sead::Vector3<f32>&);
void setEffectEmitterVolumeScale(IUseEffectKeeper*, const char*, const sead::Vector3<f32>&);
void setEffectParticleScale(IUseEffectKeeper*, const char*, f32);
void setEffectParticleScale(IUseEffectKeeper*, const char*, const sead::Vector3<f32>&);
void setEffectParticleAlpha(IUseEffectKeeper*, const char*, f32);
void setEffectParticleColor(IUseEffectKeeper*, const char*, const sead::Color4f&);
void setParticleLifeScale(IUseEffectKeeper*, const char*, f32);
void setEffectParticleDirectionalVel(IUseEffectKeeper*, const char*, f32);
void setEffectFollowPosPtr(IUseEffectKeeper*, const char*, const sead::Vector3<f32>*);
void setEffectFollowMtxPtr(IUseEffectKeeper*, const char*, const sead::Matrix34<f32>*);
void setEffectNamedMtxPtr(IUseEffectKeeper*, const char*, const sead::Matrix34<f32>*);
bool trySetEffectNamedMtxPtr(IUseEffectKeeper*, const char*, const sead::Matrix34<f32>*);
bool tryUpdateEffectMaterialCode(IUseEffectKeeper*, const char*);
void resetEffectMaterialCode(IUseEffectKeeper*);
void updateEffectMaterialWater(IUseEffectKeeper*, bool);
void updateEffectMaterialWet(IUseEffectKeeper*, bool);
void updateEffectMaterialPuddle(IUseEffectKeeper*, bool);
}  // namespace al
