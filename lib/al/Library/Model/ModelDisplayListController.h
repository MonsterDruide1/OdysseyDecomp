#pragma once

namespace al {
class ModelGroup;
class GraphicsSystemInfo;

class ModelDisplayListController {
public:
    ModelDisplayListController(ModelGroup*);
    void update();

    void setGraphicsSystemInfo(GraphicsSystemInfo* graphicsSystemInfo) {
        mGraphicsSystemInfo = graphicsSystemInfo;
    }

private:
    ModelGroup* mModelGroup;
    GraphicsSystemInfo* mGraphicsSystemInfo;
    void* filler[4];
};
static_assert(sizeof(ModelDisplayListController) == 0x30);

}  // namespace al
