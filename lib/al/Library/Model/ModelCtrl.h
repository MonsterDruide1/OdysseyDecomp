#pragma once

#include <basis/seadTypes.h>

namespace nn::g3d {
class ModelObj;
}

namespace al {
class GpuMemAllocator;
class ModelShaderHolder;
class Resource;
class ActorDitherAnimator;

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

private:
    nn::g3d::ModelObj* mModelObj;
    s32 _8;
    GpuMemAllocator* mGpuMemAllocator;
    ModelShaderHolder* mShaderHolder;
    s32 mBlockBufferSize;
    unsigned char padding1[0x354];
    ActorDitherAnimator* mActorDitherAnimator;
    unsigned char padding2[0xc8];
};

static_assert(sizeof(ModelCtrl) == 0x448);
}  // namespace al
