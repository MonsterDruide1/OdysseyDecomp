#pragma once

namespace al {
class GraphicsQualityInfo;

class GraphicsQualityController {
public:
    // missing

    GraphicsQualityInfo* getGraphicsQualityInfo() const { return mGraphicsQualityInfo; }

private:
    void* size1[3];
    GraphicsQualityInfo* mGraphicsQualityInfo;
    void* size2[29];
};

static_assert(sizeof(GraphicsQualityController) == 0x108);

}  // namespace al
