#pragma once

namespace agl {
class DrawContext;
}

namespace al {
class GraphicsInitArg;
class AreaObjDirector;
class ExecuteDirector;
class EffectSystem;
class PlayerHolder;
class SceneCameraInfo;
class ShaderHolder;
class GraphicsQualityController;
class ModelLodAllCtrl;
class MaterialCategoryKeeper;

struct GraphicsSystemInfo {
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

    void* size0[24];
    GraphicsQualityController* graphicsQualityController;
    void* size1[46];
    MaterialCategoryKeeper* materialCategoryKeeper;
    void* size2[234];
    ModelLodAllCtrl* modelLodAllCtrl;
    void* size3[5];
};

static_assert(sizeof(GraphicsSystemInfo) == 0x9c0);

}  // namespace al
