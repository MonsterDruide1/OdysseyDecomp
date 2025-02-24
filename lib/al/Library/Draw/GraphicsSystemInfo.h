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

    // being accessed directly in a lot of places
public:
    void* size0[24];
    GraphicsQualityController* mGraphicsQualityController;
    void* size1[46];
    MaterialCategoryKeeper* mMaterialCategoryKeeper;
    void* size2[234];
    ModelLodAllCtrl* mModelLodAllCtrl;
    void* size3[5];
};

static_assert(sizeof(GraphicsSystemInfo) == 0x9c0);

}  // namespace al
