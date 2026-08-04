#include "Library/Effect/EffectSystemInfo.h"

#include "Library/Effect/EffectKeeper.h"
#include "Library/Effect/IUseEffectKeeper.h"
#include "Library/Effect/PtclSystem.h"
#include "Library/Matrix/MatrixPtrHolder.h"
#include "Project/Effect/Effect.h"
#include "Project/Effect/EffectPrefixType.h"

namespace al {
EffectSystemInfo::EffectSystemInfo() : _0(0), mPtclSystem(nullptr), _10(nullptr), _18(0) {}

const EffectSystem& EffectSystemInfo::getEffectSystem() const {
    return mPtclSystem->getEffectSystem();
}

void emitEffectCurrentPos(IUseEffectKeeper* iUseEffectKeeper, const char* effectName) {
    iUseEffectKeeper->getEffectKeeper()->emitEffectCurrentPos(effectName);
}

void emitEffect(IUseEffectKeeper* iUseEffectKeeper, const char* effectName,
                const sead::Vector3f* pos) {
    iUseEffectKeeper->getEffectKeeper()->emitEffect(effectName, pos);
}

bool tryEmitEffect(IUseEffectKeeper* iUseEffectKeeper, const char* effectName,
                   const sead::Vector3f* pos) {
    return iUseEffectKeeper->getEffectKeeper()->tryEmitEffect(effectName, pos);
}

void deleteEffect(IUseEffectKeeper* iUseEffectKeeper, const char* effectName) {
    iUseEffectKeeper->getEffectKeeper()->deleteEffect(effectName);
}

void tryDeleteEffect(IUseEffectKeeper* iUseEffectKeeper, const char* effectName) {
    iUseEffectKeeper->getEffectKeeper()->tryDeleteEffect(effectName);
}

void deleteEffectAll(IUseEffectKeeper* iUseEffectKeeper) {
    iUseEffectKeeper->getEffectKeeper()->deleteEffectAll();
}

void tryKillEmitterAndParticleAll(IUseEffectKeeper* iUseEffectKeeper) {
    iUseEffectKeeper->getEffectKeeper()->tryKillEmitterAndParticleAll();
}

void onCalcAndDrawEffect(IUseEffectKeeper* iUseEffectKeeper) {
    iUseEffectKeeper->getEffectKeeper()->onCalcAndDraw();
}

void offCalcAndDrawEffect(IUseEffectKeeper* iUseEffectKeeper) {
    iUseEffectKeeper->getEffectKeeper()->offCalcAndDraw();
}

void forceSetStopCalcAndDraw(IUseEffectKeeper* iUseEffectKeeper, bool isStop) {
    iUseEffectKeeper->getEffectKeeper()->forceSetStopCalcAndDraw(isStop);
}

bool isEffectEmitting(const IUseEffectKeeper* iUseEffectKeeper, const char* effectName) {
    return iUseEffectKeeper->getEffectKeeper()->findEffect(effectName)->isEmitterActive();
}

void setEffectEmitRatio(IUseEffectKeeper* iUseEffectKeeper, const char* effectName, f32 emitRatio) {
    iUseEffectKeeper->getEffectKeeper()->setEmitRatio(effectName, emitRatio);
}

void setEffectAllScale(IUseEffectKeeper* iUseEffectKeeper, const char* effectName,
                       const sead::Vector3f& scale) {
    iUseEffectKeeper->getEffectKeeper()->setEmitterAllScale(effectName, scale);
}

void setEffectEmitterVolumeScale(IUseEffectKeeper* iUseEffectKeeper, const char* effectName,
                                 const sead::Vector3f& scale) {
    iUseEffectKeeper->getEffectKeeper()->setEmitterVolumeScale(effectName, scale);
}

void setEffectParticleScale(IUseEffectKeeper* iUseEffectKeeper, const char* effectName, f32 scale) {
    iUseEffectKeeper->getEffectKeeper()->setParticleScale(effectName, scale);
}

void setEffectParticleScale(IUseEffectKeeper* iUseEffectKeeper, const char* effectName,
                            const sead::Vector3f& scale) {
    iUseEffectKeeper->getEffectKeeper()->setParticleScale(effectName, scale);
}

void setEffectParticleAlpha(IUseEffectKeeper* iUseEffectKeeper, const char* effectName, f32 alpha) {
    iUseEffectKeeper->getEffectKeeper()->setParticleAlpha(effectName, alpha);
}

void setEffectParticleColor(IUseEffectKeeper* iUseEffectKeeper, const char* effectName,
                            const sead::Color4f& color) {
    iUseEffectKeeper->getEffectKeeper()->setParticleColor(effectName, color);
}

void setParticleLifeScale(IUseEffectKeeper* iUseEffectKeeper, const char* effectName, f32 scale) {
    iUseEffectKeeper->getEffectKeeper()->setParticleLifeScale(effectName, scale);
}

void setEffectParticleDirectionalVel(IUseEffectKeeper* iUseEffectKeeper, const char* effectName,
                                     f32 directionalVel) {
    Effect* effect = iUseEffectKeeper->getEffectKeeper()->findEffect(effectName);
    s32 emitterCount = effect->getEmitterCount();

    if (emitterCount >= 1) {
        for (s32 i = 0; i < emitterCount; i++) {
            nn::vfx::Handle* handle = effect->getEmitters()[i]->getHandle();

            if (handle->isValid())
                handle->GetEmitterSet()->directionalVel = directionalVel;
        }
    }
}

void setEffectFollowPosPtr(IUseEffectKeeper* iUseEffectKeeper, const char* effectName,
                           const sead::Vector3f* pos) {
    iUseEffectKeeper->getEffectKeeper()->findEffect(effectName)->setPosPtr(pos);
}

void setEffectFollowMtxPtr(IUseEffectKeeper* iUseEffectKeeper, const char* effectName,
                           const sead::Matrix34f* mtx) {
    iUseEffectKeeper->getEffectKeeper()->findEffect(effectName)->setMtxPtr(mtx);
}

void setEffectNamedMtxPtr(IUseEffectKeeper* iUseEffectKeeper, const char* mtxName,
                          const sead::Matrix34f* mtx) {
    iUseEffectKeeper->getEffectKeeper()->getMtxPtrHolder()->setMtxPtr(mtxName, mtx);
    alEffectKeeperInitFunction::updateNamedMtxPtr(iUseEffectKeeper->getEffectKeeper(), mtxName);
}

bool trySetEffectNamedMtxPtr(IUseEffectKeeper* iUseEffectKeeper, const char* mtxName,
                             const sead::Matrix34f* mtx) {
    if (iUseEffectKeeper->getEffectKeeper()) {
        iUseEffectKeeper->getEffectKeeper()->getMtxPtrHolder()->setMtxPtr(mtxName, mtx);
        return alEffectKeeperInitFunction::updateNamedMtxPtr(iUseEffectKeeper->getEffectKeeper(),
                                                             mtxName);
    }

    return false;
}

bool tryUpdateEffectMaterialCode(IUseEffectKeeper* iUseEffectKeeper, const char* materialCode) {
    return iUseEffectKeeper->getEffectKeeper()->tryUpdateMaterial(materialCode);
}

void resetEffectMaterialCode(IUseEffectKeeper* iUseEffectKeeper) {
    iUseEffectKeeper->getEffectKeeper()->tryUpdateMaterial("");
}

void updateEffectMaterialWater(IUseEffectKeeper* iUseEffectKeeper, bool isApply) {
    EffectKeeper* effectKeeper = iUseEffectKeeper->getEffectKeeper();
    EffectPrefixType prefixType(0);
    effectKeeper->updatePrefix(prefixType, isApply);
}

void updateEffectMaterialWet(IUseEffectKeeper* iUseEffectKeeper, bool isApply) {
    EffectKeeper* effectKeeper = iUseEffectKeeper->getEffectKeeper();
    EffectPrefixType prefixType(1);
    effectKeeper->updatePrefix(prefixType, isApply);
}

void updateEffectMaterialPuddle(IUseEffectKeeper* iUseEffectKeeper, bool isApply) {
    EffectKeeper* effectKeeper = iUseEffectKeeper->getEffectKeeper();
    EffectPrefixType prefixType(2);
    effectKeeper->updatePrefix(prefixType, isApply);
}
}  // namespace al
