#pragma once

#include <container/seadOffsetList.h>
#include <container/seadPtrArray.h>
#include <container/seadStrTreeMap.h>
#include <gfx/seadCamera.h>
#include <gfx/seadFrameBuffer.h>
#include <nn/g3d/ViewVolume.h>
#include <utility/aglParameterIO.h>

namespace agl {
class DrawContext;

namespace pfx {
class FilterAA;
}

namespace sdw {
class PrimitiveOcclusion;
}  // namespace sdw
}  // namespace agl

namespace al {
class AreaObjDirector;
class ExecuteDirector;
class EffectSystem;
class PlayerHolder;
class SceneCameraInfo;
class ShaderHolder;
class BloomDirector;
class CubeMapDirector;
class DirectionalLightKeeper;
class GraphicsAreaDirector;
class GraphicsPresetDirector;
class DemoGraphicsController;
class RadialBlurDirector;
class PrePassLightKeeper;
class ShaderEnvTextureKeeper;
class ShadowDirector;
class DepthOfFieldDrawer;
class GraphicsQualityController;
class ShaderMirrorDirector;
class GraphicsParamRequesterImpl;
class GraphicsParamRequesterImpl;
class FlareFilterDirector;
class GodRayDirector;
class FogDirector;
class OccludedEffectDirector;
class LightStreakDirector;
class HdrCompose;
class SSIIKeeper;
class OceanWave;
class RandomTextureKeeper;
class WorldAODirector;
class PointSpriteCursorHolder;
class MaterialLightDirector;
class MaterialCategoryKeeper;
class SkyDirector;
class OcclusionCullingJudge;
class VignettingDrawer;
class CameraBlurController;
class ThunderRenderKeeper;
class StarrySky;
class NoiseTextureKeeper;
class CloudRenderKeeper;
class GpuMemAllocator;
class FootPrintTextureKeeper;
class ProgramTextureKeeper;
class RippleTextureKeeper;
class ViewRenderer;
class SubCameraRenderer;
class TemporalInterlace;
class PeripheryRendering;
class PostProcessingFilter;
class GBufferArray;
class Projection;
class UniformBlock;
class AtmosScatter;
class AtmosScatterDrawer;
class GraphicsParamFilePath;
class Resource;
class VastGridMeshDirector;
class FullScreenTriangle;
class ReducedBufferRenderer;
class ModelOcclusionCullingDirector;
class ModelLodAllCtrl;
class ModelShaderHolder;
class PrepassTriangleCulling;
class ApplicationMessageReceiver;

struct GraphicsInitArg {
    GraphicsInitArg(agl::DrawContext*, sead::FrameBuffer*);
    bool isUsingCubeMapAtmosScatter() const;
    u32 getAtmosScatterViewNum() const;

    s32 _0;
    bool _4;
    u8 atmosScatterViewNum;
    bool _6;
    s32 displayWidth;
    s32 displayHeight;
    s32 _10;
    s32 _14;
    s32 _18;
    s32 _1c;
    bool _20;
    s32 _24;
    bool _28;
    s32 _2c;
    s32 _30;
    s32 _34;
    s32 _38;
    s32 _3c;
    agl::DrawContext* drawContext;
};

class GraphicsSystemInfo {
public:
    GraphicsSystemInfo();
    ~GraphicsSystemInfo();

    void init(const GraphicsInitArg&, AreaObjDirector*, ExecuteDirector*, EffectSystem*,
              PlayerHolder*, SceneCameraInfo*, ShaderHolder*);

    agl::DrawContext* getDrawContext() const;
    void endInit();
    void initAfterPlacement();
    void clearGraphicsRequest();
    void updateGraphics();
    void preDrawGraphics(SceneCameraInfo*);

    void set_2f4(s32 set) { _2f4 = set; }

    const ViewRenderer* getViewRenderer() const { return mViewRenderer; }

private:
    sead::StrTreeMap<128, const sead::PtrArray<UniformBlock>*> mViewIndexedUboArrayTree;
    GraphicsInitArg mInitArg;
    BloomDirector* mBloomDirector;
    CubeMapDirector* mCubeMapDirector;
    DirectionalLightKeeper* mDirectionalLightKeeper;
    GraphicsAreaDirector* mGraphicsAreaDirector;
    GraphicsPresetDirector* mGraphicsPresetDirector;
    DemoGraphicsController* mDemoGraphicsController;
    RadialBlurDirector* mRadialBlurDirector;
    PrePassLightKeeper* mPrePassLightKeeper;
    ShaderEnvTextureKeeper* mShaderEnvTextureKeeper;
    ShadowDirector* mShadowDirector;
    DepthOfFieldDrawer* mDepthOfFieldDrawer;
    GraphicsQualityController* mGraphicsQualityController;
    ShaderMirrorDirector* mShaderMirrorDirector;
    GraphicsParamRequesterImpl* _d0;
    GraphicsParamRequesterImpl* mColorCorrection;
    FlareFilterDirector* mFlareFilterDirector;
    GodRayDirector* mGodRayDirector;
    FogDirector* mFogDirector;
    OccludedEffectDirector* mOccludedEffectDirector;
    LightStreakDirector* mLightStreakDirector;
    HdrCompose* mHdrCompose;
    SSIIKeeper* mSSIIKeeper;
    agl::sdw::PrimitiveOcclusion* mPrimitiveOcclusion;
    nn::g3d::ViewVolume mViewVolume;
    void* _208;
    OceanWave* mOceanWave;
    RandomTextureKeeper* mRandomTextureKeeper;
    WorldAODirector* mWorldAODirector;
    PointSpriteCursorHolder* mPointSpriteCursorHolder;
    MaterialLightDirector* mMaterialLightDirector;
    MaterialCategoryKeeper* mMaterialCategoryKeeper;
    SkyDirector* mSkyDirector;
    ShaderHolder* mShaderHolder;
    OcclusionCullingJudge* mOcclusionCullingJudge;
    VignettingDrawer* mVignettingDrawer;
    CameraBlurController* mCameraBlurController;
    ThunderRenderKeeper* mThunderRenderKeeper;
    StarrySky* mStarrySky;
    NoiseTextureKeeper* mNoiseTextureKeeper;
    CloudRenderKeeper* mCloudRenderKeeper;
    GpuMemAllocator* mGpuMemAllocator;
    FootPrintTextureKeeper* mFootPrintTextureKeeper;
    ProgramTextureKeeper* mProgramTextureKeeper;
    RippleTextureKeeper* mRippleTextureKeeper;
    sead::PtrArrayImpl* _2a8;
    ViewRenderer* mViewRenderer;
    SubCameraRenderer* mSubCameraRenderer;
    TemporalInterlace* mTemporalInterface;
    PeripheryRendering* mPeripheryRendering;
    PostProcessingFilter* mPostProcessingFilter;
    GBufferArray* mDrawEnvGBufferArray;
    const sead::Camera* mDrawEnvCamera;
    const Projection* mDrawEnvProjection;
    s32 mDrawEnvViewIndex;
    s32 _2f4;
    agl::pfx::FilterAA* mFilterAA;
    AtmosScatter* mAtmosScatter;
    AtmosScatterDrawer* mAtmosScatterDrawer;
    GraphicsParamFilePath* mParamFilePath;
    // agl::utl::IParameterIO _318;
    // agl::utl::IParameterObj _4e8;
    // agl::utl::ParameterOfInt mAreaFindMode;
    // agl::utl::ParameterOfInt mAtmosScatterType;
    // agl::utl::ParameterOfBool mIsUsingUpdateAtmosCubeMap;
    // agl::utl::ParameterOfBool mIsUsingOceanWave;
    // agl::utl::ParameterOfInt mOccGroupNum;
    // agl::utl::IParameterIO _5b8;
    // agl::utl::IParameterObj _788;
    // agl::utl::ParameterOfBool mIsUsingTemporal;
    // agl::utl::ParameterOfBool mIsUsingPeriphery;
    // agl::utl::ParameterOfBool mIsUsingStarrySky;
    // agl::utl::ParameterOfFloat mCupeMapIntensityPower;
    // agl::utl::ParameterOfFloat mCubeMapIntensityRange;
    // agl::utl::ParameterOfFloat mLineLightAntiArtifact;
    // agl::utl::ParameterOfFloat mMinRoughnessGGX;
    // agl::utl::ParameterOfFloat mSphereLightDiffuseAdd;
    // agl::utl::ParameterOfFloat mSpecularScale;
    // agl::utl::ParameterOfFloat mLightUnitScale;
    // agl::utl::ParameterOfFloat mLightColorScale;
    char filler[0x918 - 0x318];  // TODO: Replace this with the real params once the Parameter types
                                 // are added to agl
    UniformBlock* _918;
    Resource* _920;
    AreaObjDirector* mAreaObjDirector;
    ExecuteDirector* mExecuteDirector;
    EffectSystem* mEffectSystem;
    SceneCameraInfo* mSceneCameraInfo;
    const char* _948;
    void* _950[3];  // sead::OffsetList
    VastGridMeshDirector* mVastGridMeshDirector;
    FullScreenTriangle* mFullScreenTriangle;
    s32 _978;
    ReducedBufferRenderer* mReducedBufferRenderer;
    ModelOcclusionCullingDirector* mModelOcclusionCullingDirector;
    ModelLodAllCtrl* mModelLodAllCtrl;
    ModelShaderHolder* mModelShaderHolder;
    PrepassTriangleCulling* mPrepassTriangleCulling;
    bool _9a8;
    ApplicationMessageReceiver* mApplicationMessageReceiver;
    void* _9b8;
};

static_assert(sizeof(GraphicsSystemInfo) == 0x9c0);

}  // namespace al
