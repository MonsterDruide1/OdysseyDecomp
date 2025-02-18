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
struct SceneCameraInfo;
class ShaderHolder;

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

    // incomplete
private:
    void* filler[312];
};

static_assert(sizeof(GraphicsSystemInfo) == 0x9c0);

}  // namespace al
