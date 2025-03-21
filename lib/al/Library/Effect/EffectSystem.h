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

    void set_69(bool val) { _69 = val; }

    // incomplete
private:
    void* filler[13];
    bool _68;
    bool _69;
};

}  // namespace al
