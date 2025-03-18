#pragma once

#include <container/seadOffsetList.h>
#include <container/seadPtrArray.h>
#include <container/seadStrTreeMap.h>
#include <gfx/seadCamera.h>
#include <gfx/seadFrameBuffer.h>
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

namespace nn::g3d {
class ViewVolume {
private:
    char filler[0xE8];  // TODO: Move this to nn::g3d
};
}  // namespace nn::g3d

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
    u8 getAtmosScatterViewNum() const;

    s32 dword_0;
    bool field_4;
    u8 atmosScatterViewNum;
    bool field_6;
    s32 displayWidth;
    s32 displayHeight;
    s32 dword_10;
    s32 dword_14;
    s32 dword_18;
    s32 dword_1c;
    bool byte_20;
    s32 dword_24;
    bool byte_28;
    s32 dword_2c;
    s32 dword_30;
    s32 dword_34;
    s32 dword_38;
    s32 dword_3c;
    agl::DrawContext* field_40;
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

    void setField2f4(s32 set) { field_2f4 = set; }

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
    GraphicsParamRequesterImpl* field_d0;
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
    void* field_208;
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
    sead::PtrArrayImpl* field_2a8;
    ViewRenderer* mViewRenderer;
    SubCameraRenderer* mSubCameraRenderer;
    TemporalInterlace* mTemporalInterface;
    PeripheryRendering* mPeripheryRendering;
    PostProcessingFilter* mPostProcessingFilter;
    GBufferArray* mDrawEnvGBufferArray;
    const sead::Camera* mDrawEnvCamera;
    const Projection* mDrawEnvProjection;
    s32 mDrawEnvViewIndex;
    s32 field_2f4;
    agl::pfx::FilterAA* mFilterAA;
    AtmosScatter* mAtmosScatter;
    AtmosScatterDrawer* mAtmosScatterDrawer;
    GraphicsParamFilePath* mParamFilePath;
    // agl::utl::IParameterIO field_318;
    // agl::utl::IParameterObj field_4e8;
    // agl::utl::ParameterOfInt mAreaFindMode;
    // agl::utl::ParameterOfInt mAtmosScatterType;
    // agl::utl::ParameterOfBool mIsUsingUpdateAtmosCubeMap;
    // agl::utl::ParameterOfBool mIsUsingOceanWave;
    // agl::utl::ParameterOfInt mOccGroupNum;
    // agl::utl::IParameterIO field_5b8;
    // agl::utl::IParameterObj field_788;
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
    UniformBlock* field_918;
    Resource* field_920;
    AreaObjDirector* mAreaObjDirector;
    ExecuteDirector* mExecuteDirector;
    EffectSystem* mEffectSystem;
    SceneCameraInfo* mSceneCameraInfo;
    const char* field_948;
    void* field_950[3];  // sead::OffsetList
    VastGridMeshDirector* mVastGridMeshDirector;
    FullScreenTriangle* mFullScreenTriangle;
    s32 field_978;
    ReducedBufferRenderer* mReducedBufferRenderer;
    ModelOcclusionCullingDirector* mModelOcclusionCullingDirector;
    ModelLodAllCtrl* mModelLodAllCtrl;
    ModelShaderHolder* mModelShaderHolder;
    PrepassTriangleCulling* mPrepassTriangleCulling;
    bool field_9a8;
    ApplicationMessageReceiver* mApplicationMessageReceiver;
    void* field_9b8;
};

static_assert(sizeof(GraphicsSystemInfo) == 0x9c0);

}  // namespace al
