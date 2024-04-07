#pragma once

#include <nn/g3d/ModelObj.h>

namespace al {
class GpuMemAllocator;
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

private:
    nn::g3d::ModelObj* mModelObj;
    s32 _8;
    GpuMemAllocator* mGpuMemAllocator;
    void* _18;
    s32 mBlockBufferSize;
    // note: member list not complete
};
}  // namespace al
