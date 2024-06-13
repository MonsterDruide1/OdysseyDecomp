#pragma once

#include <heap/seadHeap.h>

namespace agl {
class DrawContext;
}

namespace al {
class CameraDirector;
class GraphicsSystemInfo;
class ExecuteDirector;

class EffectSystem {
public:
    static EffectSystem* initializeSystem(agl::DrawContext*, sead::Heap*);

    EffectSystem();

    void initScene();
    void startScene(ExecuteDirector*);
    void endScene();
    void endInit();
    void setCameraDirector(CameraDirector*);
    void setGraphicsSystemInfo(const GraphicsSystemInfo*);

    // incomplete
private:
    // missing
};

}  // namespace al
