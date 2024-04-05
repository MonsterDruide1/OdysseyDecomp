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

private:
    nn::g3d::ModelObj mModelObj;
    int _8;
    GpuMemAllocator* mGpuMemAllocator;
    void* _18;
    int mBlockBufferSize;
    // note: member list not complete
};
}  // namespace al
