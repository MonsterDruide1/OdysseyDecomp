#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class IUseEffectKeeper;

void emitEffectCurrentPos(IUseEffectKeeper*, const char*);
void emitEffect(IUseEffectKeeper*, const char*, const sead::Vector3<float>*);
bool tryEmitEffect(IUseEffectKeeper*, const char*, const sead::Vector3<float>*);
void deleteEffect(IUseEffectKeeper*, const char*);
bool tryDeleteEffect(IUseEffectKeeper*, const char*);
void deleteEffectAll(IUseEffectKeeper*);
void tryKillEmitterAndParticleAll(IUseEffectKeeper*);
void onCalcAndDrawEffect(IUseEffectKeeper*);
void offCalcAndDrawEffect(IUseEffectKeeper*);
void forceSetStopCalcAndDraw(IUseEffectKeeper*, bool);
bool isEffectEmitting(IUseEffectKeeper const*, const char*);
void setEffectEmitRatio(IUseEffectKeeper*, const char*, float);
void setEffectAllScale(IUseEffectKeeper*, const char*, const sead::Vector3<float>&);
void setEffectEmitterVolumeScale(IUseEffectKeeper*, const char*, const sead::Vector3<float>&);
void setEffectParticleScale(IUseEffectKeeper*, const char*, float);
void setEffectParticleScale(IUseEffectKeeper*, const char*, const sead::Vector3<float>&);
void setEffectParticleAlpha(IUseEffectKeeper*, const char*, float);
void setEffectParticleColor(IUseEffectKeeper*, const char*, const sead::Color4f&);
void setParticleLifeScale(IUseEffectKeeper*, const char*, float);
void setEffectParticleDirectionalVel(IUseEffectKeeper*, const char*, float);
void setEffectFollowPosPtr(IUseEffectKeeper*, const char*, const sead::Vector3<float>*);
void setEffectFollowMtxPtr(IUseEffectKeeper*, const char*, const sead::Matrix34<float>*);
void setEffectNamedMtxPtr(IUseEffectKeeper*, const char*, const sead::Matrix34<float>*);
bool trySetEffectNamedMtxPtr(IUseEffectKeeper*, const char*, const sead::Matrix34<float>*);
bool tryUpdateEffectMaterialCode(IUseEffectKeeper*, const char*);
void resetEffectMaterialCode(IUseEffectKeeper*);
void updateEffectMaterialWater(IUseEffectKeeper*, bool);
void updateEffectMaterialWet(IUseEffectKeeper*, bool);
void updateEffectMaterialPuddle(IUseEffectKeeper*, bool);
}  // namespace al
