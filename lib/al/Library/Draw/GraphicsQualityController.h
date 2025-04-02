#pragma once

namespace al {
class GraphicsQualityInfo;

class GraphicsQualityController {
public:
    // missing

    GraphicsQualityInfo* getGraphicsQualityInfo() const { return mGraphicsQualityInfo; }

private:
    void* _0[3];
    GraphicsQualityInfo* mGraphicsQualityInfo;
    void* _20[29];
};

static_assert(sizeof(GraphicsQualityController) == 0x108);

}  // namespace al
