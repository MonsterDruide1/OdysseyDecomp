#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Library/Model/JointMtxPtr.h"
#include "Project/Effect/EffectEmitter.h"

namespace al {
class EffectCameraHolder;
class EffectInfo;
class EffectSystem;
class EffectSystemInfo;

class Effect {
public:
    Effect(const EffectSystemInfo*, const EffectInfo*, const sead::Vector3f*, const sead::Vector3f*,
           const sead::Matrix34f*, u64);

    void setCameraHolder(EffectCameraHolder*);
    void setPosPtr(const sead::Vector3f*);
    void setMtxPtr(const sead::Matrix34f*);
    bool update();
    void setFarClip(bool);
    bool tryUpdateMaterial(const char*, const bool (&)[3]);
    void emitEmitter(EffectEmitter*, const sead::Vector3f*);
    void emitEmitters(const sead::Vector3f*, bool);
    bool tryEmitEmitters(const sead::Vector3f*, bool);
    bool tryEmitEmitter(EffectEmitter*, const sead::Vector3f*);
    void tryDeleteEmitters();
    void deleteAndClearEmitter();
    bool isOneTimeFade() const;
    void tryKillEmitterAndParticleAll();
    void forceSetStopCalcAndDraw(bool);
    void setStopCalcAndDraw(bool);
    void setActorClip(bool);
    void setEmitRatio(f32);
    void setEmitterAllScale(const sead::Vector3f&);
    void setEmitterVolumeScale(const sead::Vector3f&);
    void setParticleScale(f32);
    void setParticleScale(const sead::Vector3f&);
    void setParticleAlpha(f32);
    void setParticleColor(const sead::Color4f&);
    void setParticleLifeScale(f32);
    const sead::Matrix34f* getViewMtxPtr() const;
    bool isLoopOrInfinity() const;
    bool isEmitterActive() const;

    EffectEmitter* const* getEmitters() const { return mEmitters; }

    s32 getEmitterCount() const { return mEmitterCount; }

private:
    void* _0 = nullptr;
    EffectEmitter** mEmitters = nullptr;
    s32 mEmitterCount = 0;
    const EffectSystem* mEffectSystem = nullptr;
    const EffectInfo* mEffectInfo = nullptr;
    const sead::Vector3f* mPosPtr = nullptr;
    const sead::Vector3f* _30 = nullptr;
    JointMtxPtr mJointMtxPtr;
    const sead::Matrix34f* mViewMtxPtr = nullptr;
    EffectCameraHolder* mCameraHolder = nullptr;
    u8 _58[8] = {};
    sead::FixedSafeString<64> _60;
    u8 _b8[0x10] = {};
};

static_assert(sizeof(Effect) == 0xc8);
}  // namespace al
