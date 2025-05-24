#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace agl {
class TextureData;
}  // namespace agl

namespace agl::sdw {
class DepthShadow;
}  // namespace agl::sdw

namespace sead {
class Viewport;
}  // namespace sead

namespace al {
class Effect;
class EffectUserInfo;
class MtxPtrHolder;
class EffectSystemInfo;
class EffectPrefixType;
class EffectSystem;
class EffectSystemInfo;
class EffectResourceInfo;
class IUseEffectKeeper;
class ModelKeeper;
class IUseCamera;

class EffectKeeper {
public:
    EffectKeeper(EffectSystemInfo* systemInfo, const char*, const sead::Vector3f*,
                 const sead::Vector3f*, const sead::Matrix34f*);
    void update();
    void tryUpdateMaterial(const char*);
    void updatePrefix(const EffectPrefixType&, bool);
    void emitEffectCurrentPos(const char*);
    void findEffect(const char*);
    void emitEffect(const char*, const sead::Vector3f*);
    void tryEmitEffect(const char*, const sead::Vector3f*);
    void deleteEffect(const char*);
    void tryDeleteEffect(const char*);
    void tryKillEmitterAndParticleAll();
    void deleteEffectAll();
    void deleteAndClearEffectAll();
    void onCalcAndDraw();
    void offCalcAndDraw();
    void forceSetStopCalcAndDraw(bool);
    void setEmitRatio(const char*, f32);
    void setEmitterAllScale(const char*, const sead::Vector3f&);
    void setEmitterVolumeScale(const char*, const sead::Vector3f&);
    void setParticleScale(const char*, f32);
    void setParticleScale(const char*, const sead::Vector3f&);
    void setParticleAlpha(const char*, f32);
    void setParticleColor(const char*, const sead::Color4f&);
    void setParticleLifeScale(const char*, f32);
    void findMtxPtr(const char*);
    void tryFindEffect(const char*);

    bool get_21() const { return field_21; }

private:
    const char* mName;
    u32 mEffectCount;
    Effect** mEffects;
    const char* mMaterialName;
    bool mHasEmittedEffect;
    bool field_21;
    bool field_22;
    EffectUserInfo* mEffectUserInfo;
    MtxPtrHolder* mMtxPtrHolder;
};
}  // namespace al

namespace alEffectFunction {
bool tryFindEffectUser(const al::EffectSystemInfo*, const char*);
void initResourceInfo(const al::EffectSystemInfo*, al::EffectResourceInfo*);
bool tryFindEffectResouceInfo(const al::EffectSystemInfo*, const char*);
void emitEffectIfExist(al::IUseEffectKeeper* effectKeeperHolder, const char* effectName,
                       const sead::Vector3f* pos);
}  // namespace alEffectFunction

namespace alEffectKeeperInitFunction {
void setupModelToEffectKeeper(al::EffectKeeper* effectKeeper, const al::ModelKeeper* modelKeeper);
void setupCameraToEffectKeeper(al::EffectKeeper* effectKeeper, const al::IUseCamera* iUseCamera);
}  // namespace alEffectKeeperInitFunction

namespace alEffectSystemFunction {
void setDrawPathRenderStateSetDrawDepthShadow(const al::EffectSystem*, bool);
void calcEffectCompute(const al::EffectSystem*);
void drawEffectDeferred(const al::EffectSystem*, const sead::Matrix44f&, const sead::Matrix34f&,
                        f32, f32, f32);
void drawEffectForward(const al::EffectSystem*, const sead::Matrix44f&, const sead::Matrix34f&, f32,
                       f32, f32);
void drawEffectForwardReduceBuffer(const al::EffectSystem*, const sead::Matrix44f&,
                                   const sead::Matrix34f&, f32, f32, f32);
void drawEffectForwardReduceBufferHdr(const al::EffectSystem*, const sead::Matrix44f&,
                                      const sead::Matrix34f&, f32, f32, f32);
void drawEffectForwardAfterFog(const al::EffectSystem*, const sead::Matrix44f&,
                               const sead::Matrix34f&, f32, f32, f32);
void drawEffectIndirect(const al::EffectSystem*, const sead::Matrix44f&, const sead::Matrix34f&,
                        f32, f32, f32, const sead::Viewport*);
void drawEffectPostEffect(const al::EffectSystem*, const sead::Matrix44f&, const sead::Matrix34f&,
                          f32, f32, f32);
void drawEffectChromakey(const al::EffectSystem*, const sead::Matrix44f&, const sead::Matrix34f&,
                         f32, f32, f32);
void drawEffectShadowCaster(const al::EffectSystem*, const sead::Matrix44f&, const sead::Matrix34f&,
                            f32, f32, f32);
void drawEffectShadowReceiver(const al::EffectSystem*, const sead::Matrix44f&,
                              const sead::Matrix34f&, f32, f32, f32);
void drawEffectShadowCasterWithSilhouette(const al::EffectSystem*, const sead::Matrix44f&,
                                          const sead::Matrix34f&, f32, f32, f32);
void drawEffectShadowReceiverWithSilhouette(const al::EffectSystem*, const sead::Matrix44f&,
                                            const sead::Matrix34f&, f32, f32, f32);
void drawEffectShadowCasterForShadowMap(const al::EffectSystem*, const sead::Matrix44f&,
                                        const sead::Matrix34f&, f32, f32, f32,
                                        const sead::Vector3f*);
void drawEffectShadowCasterWithSilhouetteForShadowMap(const al::EffectSystem*,
                                                      const sead::Matrix44f&,
                                                      const sead::Matrix34f&, f32, f32, f32,
                                                      const sead::Vector3f*);
void drawEffect2D(const al::EffectSystem*, const sead::Viewport*);
void updateEffect2D(al::EffectSystem*);
void calcShadowClipVolume(const al::EffectSystem*, agl::sdw::DepthShadow*);
void tryDeleteEmitterAndParticleOneTime(const al::EffectSystemInfo*);
void setDepthTexture(const al::EffectSystem*, const agl::TextureData*);
bool isHasRenderingEmitterInReduceBuffer(const al::EffectSystem*);
bool isHasRenderingEmitterInReduceBufferHdr(const al::EffectSystem*);
}  // namespace alEffectSystemFunction
