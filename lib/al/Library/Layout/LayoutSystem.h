#pragma once

#include <basis/seadTypes.h>

namespace agl {
class DrawContext;
}

namespace al {
class FontHolder;
class LayoutSystem;
class EffectSystem;

class LayoutKit {
public:
    LayoutKit(FontHolder*);
    ~LayoutKit();

    void createCameraParamForIcon();
    void createExecuteDirector(s32);
    void createEffectSystem();
    void endInit();
    void update();
    void draw(const char*) const;
    void drawList(const char*, const char*) const;
    void setLayoutSystem(LayoutSystem*);
    void setDrawContext(agl::DrawContext*);

    void setEffectSystem(EffectSystem* effectSystem) { mEffectSystem = effectSystem; }

private:
    void* _0[2];
    EffectSystem* mEffectSystem;
    void* _18[3];
};

static_assert(sizeof(LayoutKit) == 0x30);

}  // namespace al
