#pragma once

namespace nn::g3d {
class ModelObj;
}

namespace al {
class ActorDitherAnimator;
class GpuMemAllocator;
class GraphicsQualityInfo;
class ModelOcclusionQuery;
class ModelShaderHolder;
class Resource;

class ModelCtrl {
public:
    ModelCtrl();
    ~ModelCtrl();

    void initResource(Resource*, Resource*);
    void show();
    void hide();
    void recreateDisplayList();

    nn::g3d::ModelObj* getModelObj() const { return mModelObj; }

    ActorDitherAnimator* getActorDitherAnimator() const { return mActorDitherAnimator; }

    void setCalcViewCore(s32 core) { mCalcViewCore = core; }

    void setGraphicsQualityInfo(GraphicsQualityInfo* info) { mGraphicsQualityInfo = info; }

    void setModelOcclusionQuery(ModelOcclusionQuery* query) { mModelOcclusionQuery = query; }

private:
    nn::g3d::ModelObj* mModelObj;
    s32 _8;
    GpuMemAllocator* mGpuMemAllocator;
    ModelShaderHolder* mShaderHolder;
    s32 mBlockBufferSize;
    unsigned char padding1[332];
    GraphicsQualityInfo* mGraphicsQualityInfo;
    unsigned char padding2[514];
    ActorDitherAnimator* mActorDitherAnimator;
    unsigned char padding3[36];
    s32 mCalcViewCore;
    s32 pad_3b0;
    unsigned char padding4[124];
    ModelOcclusionQuery* mModelOcclusionQuery;
    unsigned char padding5[16];
};

static_assert(sizeof(ModelCtrl) == 0x448);
}  // namespace al
