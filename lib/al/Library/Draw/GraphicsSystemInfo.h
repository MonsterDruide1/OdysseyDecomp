#pragma once

#include <basis/seadTypes.h>
#include <container/seadStrTreeMap.h>
#include <nn/g3d/ViewVolume.h>

namespace sead {
class Camera;
class FrameBuffer;
class PtrArrayImpl;
template <typename T>
class PtrArray;
}  // namespace sead

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
class ApplicationMessageReceiver;
class AreaObjDirector;
class AtmosScatter;
class AtmosScatterDrawer;
class BloomDirector;
class CameraBlurController;
class CloudRenderKeeper;
class CubeMapDirector;
class DemoGraphicsController;
class DepthOfFieldDrawer;
class DirectionalLightKeeper;
class EffectSystem;
class ExecuteDirector;
class FlareFilterDirector;
class FogDirector;
class FootPrintTextureKeeper;
class FullScreenTriangle;
class GBufferArray;
class GodRayDirector;
class GpuMemAllocator;
class GraphicsAreaDirector;
class GraphicsParamFilePath;
class GraphicsParamRequesterImpl;
class GraphicsPresetDirector;
class GraphicsQualityController;
class HdrCompose;
class LightStreakDirector;
class MaterialCategoryKeeper;
class MaterialLightDirector;
class ModelLodAllCtrl;
class ModelOcclusionCullingDirector;
class ModelShaderHolder;
class NoiseTextureKeeper;
class OccludedEffectDirector;
class OcclusionCullingJudge;
class OceanWave;
class PeripheryRendering;
class PlayerHolder;
class PointSpriteCursorHolder;
class PostProcessingFilter;
class PrePassLightKeeper;
class PrepassTriangleCulling;
class ProgramTextureKeeper;
class Projection;
class RadialBlurDirector;
class RandomTextureKeeper;
class ReducedBufferRenderer;
class Resource;
class RippleTextureKeeper;
class SSIIKeeper;
class SceneCameraInfo;
class ShaderEnvTextureKeeper;
class ShaderHolder;
class ShaderMirrorDirector;
class ShadowDirector;
class SkyDirector;
class StarrySky;
class SubCameraRenderer;
class TemporalInterlace;
class ThunderRenderKeeper;
class UniformBlock;
class VastGridMeshDirector;
class ViewRenderer;
class VignettingDrawer;
class WorldAODirector;

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

    ModelLodAllCtrl* getModelLodAllCtrl() const { return mModelLodAllCtrl; }

    MaterialCategoryKeeper* getMaterialCategoryKeeper() const { return mMaterialCategoryKeeper; }

    GraphicsQualityController* getGraphicsQualityController() const {
        return mGraphicsQualityController;
    }

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
