#pragma once

#include <gfx/seadColor.h>

#include "Library/Fluid/IUseFluidSurface.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Play/Graphics/PartsGraphics.h"

namespace al {

class GraphicsSystemInfo;
class ExecuteDirector;
class OceanWaveTexture;
class FluidSimulateWave;
class OrthoDepthTexture;
class ApertureMesh;
class UniformBlock;
class EffectKeeper;
class FogParam;
class YFogParam;
class ShaderProgram;
class VastGridMesh;

class OceanWave : public PartsGraphics, public IUseFluidSurface, public HioNode {
public:
    OceanWave(GraphicsSystemInfo*, ExecuteDirector*);
    virtual ~OceanWave();

    void init();
    void addRipple(const sead::Vector3f&, f32, f32);
    void calcWindDir(sead::Vector2f* windDir) const;
    void drawMesh(const GraphicsRenderInfo&, const RenderVariables*, bool, bool) const;
    void initRippleGaussian(s32);

    void setApertureCameraHeightLodLen(f32);
    void setApertureDisplacementScaleBase(f32);
    void setApertureIsDoubleLod(bool);
    void setApertureLayerNum(s32);
    void setApertureLodOffset(s32);
    void setApertureMinGridDivLevel(s32);
    void setAperturePatchLength(f32);
    void setApertureQuadLayerLevel(s32);
    void setCloudSeaParameter();
    void setGridMeshParam(f32, f32, f32, f32, s32);
    void setHeight(f32 height);
    void setOceanTextureCoefSmallL(f32);
    void setOceanTextureInitParam(f32, f32, f32, f32, f32);
    void setOceanTextureParam(f32, f32, f32);
    void setOceanTextureTexLenScale(f32);

    bool tryCalcDepthRate(f32*, const sead::Vector3f&) const;
    bool tryCalcHeightDepthAdd(f32*, const sead::Vector3f&) const;
    bool tryCalcRippleHeight(f32*, const sead::Vector3f&) const;
    bool tryCalcWaveDisplacement(sead::Vector3f*, const sead::Vector3f&) const;
    bool tryCalcWaveHeight(f32*, const sead::Vector3f&) const;

    void updateRippleCenterPos(const sead::Vector3f&);

    void finalize() override;
    void update(const GraphicsUpdateInfo&) override;
    void calcGpu(const GraphicsCalcGpuInfo&) override;
    void drawGBufferAfterSky(const GraphicsRenderInfo&) const override;
    void drawForward(const GraphicsRenderInfo&, const RenderVariables&) const override;
    void drawDeferred(const GraphicsRenderInfo&) const override;
    void drawIndirect(const GraphicsRenderInfo&, const RenderVariables&) const override;
    const char* getName() const override;

    bool calcIsInArea(const sead::Vector3f&) const override;
    void calcPos(sead::Vector3f*, const sead::Vector3f&) const override;
    void calcPosFlat(sead::Vector3f*, const sead::Vector3f&) const override;
    void calcDisplacementPos(sead::Vector3f*, const sead::Vector3f&) const override;
    void calcNormal(sead::Vector3f*, const sead::Vector3f&) const override;
    bool tryAddRipple(const sead::Vector3f&, f32, f32) override;
    bool tryAddRippleWithRange(const sead::Vector3f&, f32, f32, f32, f32) override;
    bool tryAddQuadRipple(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                          const sead::Vector3f&, f32) override;
    void setFieldScale(f32) override;
    const char* getTypeName() const override;

private:
    GraphicsSystemInfo* mGfxSysInfo;
    OceanWaveTexture* mWaveTexture;
    FluidSimulateWave* mFluidSimulateWave;
    OrthoDepthTexture* mDepthTex;
    OrthoDepthTexture* mBeachDepthTex;
    OrthoDepthTexture* mHeightDepthTex;
    OrthoDepthTexture* mAoDepthTex;
    ExecuteDirector* mExecuteDirector;
    VastGridMesh* mVastGridMesh;
    ApertureMesh* mApertureMesh;
    s32 _80;
    s32 _84;
    s32 _88;
    f32 _8c;
    f32 _90;
    f32 mTimeScale;
    f32 _98;
    f32 _9c;
    UniformBlock* mUBOOceanParam;
    s32 mTranslucentType;
    f32 mRefractEta;
    f32 mRefractRate;
    f32 mRoughness;
    f32 mMetalness;
    f32 mScatterScale;
    sead::Color4f mBaseColor;
    sead::Color4f mRefractColor;
    f32 mPhaseK;
    f32 mPhaseBackK;
    f32 mPhaseRate;
    f32 mF0;
    f32 mHighFreqNormalUVScale;
    f32 mHighFreqNormalScale;
    f32 _f8;
    sead::Color4f mColorDamp;
    f32 mLinearDepthScale;
    bool _110;
    bool mIsOffSnapshotMode;
    sead::Vector3f _114;
    f32 mWaveHeightScale;
    f32 mRippleNormalScale;
    f32 _128;
    f32 _12c;
    f32 _130;
    f32 _134;
    f32 mRippleHeightClamp;
    bool _13c;
    EffectKeeper* _140;
    const char* _148;
    f32 mCameraFar;
    f32 _154;
    f32 mDepthRateMin;
    bool _15c;
    f32 mHeightCheckDist;
    f32 mFarFlatDistNear;
    f32 mFarFlatDistFar;
    f32 mFarFlatDistNrmNear;
    f32 mFarFlatDistNrmFar;
    f32 mFarFlatDistHighNrmNear;
    f32 mFarFlatDistHighNrmFar;
    f32 mFarNrmMinRate;
    f32 mFarRoughness;
    f32 _184;
    FogParam* mFogParam;
    YFogParam* mYFogParam;
    sead::Vector3f _198;
    f32 _1a4;
    f32 _1a8;
    f32 _1ac;
    f32 mShoreDepthGradScale;
    f32 mShoreRippleGradScale;
    f32 mShoreColorScale;
    bool _1bc;
    f32 mCloudBacklightIntensity;
    f32 mCloudTransparencyDistance;
    bool mIsEnableRippleAlpha;
    f32 mNoiseTexCrdScale;
    sead::Vector3f _1d0;
    sead::Vector3f mNoiseTexCrdAdd;
    bool mIsEnableNoiseTex;
    s32 _1ec;
    sead::Vector4f mBubbleParam;   // these two should be separated into their
    sead::Vector4f mBubbleParam2;  // individual params when figured out
    sead::Color4f mBubbleColor;
    f32 _220;
    sead::Vector3f _224;
    sead::Vector3f _230;
    f32 _23c;
    f32 _240;
    f32 _244;
    f32 _248;
    f32 _24c;
    f32 _250;
    s32 _254;
    f32 _258;
    f32 _25c;
    bool _260;
    f32 mSphereCurveTonePower;
    f32 mSphereCurveSlope;
    f32 mSphereCurvePeakPos;
    f32 mSphereCurvePeakPower;
    f32 mSphereCurvePeakIntensity;
    f32 mSphereCurveFrontK;
    f32 mSphereCurveBackK;
    f32 mSphereCurveFrontBackRate;
    bool mIsEnableSphereCurve;
    bool mIsEnableShadow;
    bool mIsEnableAoShadow;
    sead::Color4f mAoShadowColor;
    bool _298;
    bool mIsDisableShore;
    bool mIsRipple;
    bool _29b;
    u8 mDepthFetchType;
    s32 _2a0;
    bool _2a4;
    bool _2a5;
    bool _2a6;
    ShaderProgram* mShaderRenderOcean;
    ShaderProgram* mShaderRenderCloudSea;
    f32 mHeight;
    const char* mTypeName;
    bool _2c8;
};

static_assert(sizeof(OceanWave) == 0x2d0);

}  // namespace al
