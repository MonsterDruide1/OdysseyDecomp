#pragma once

#include <basis/seadTypes.h>

namespace agl {
class DrawContext;
}

namespace nn::ui2d {
class DrawInfo;
}

namespace al {
class EffectSystem;
class ExecuteDirector;
class FontHolder;
class LayoutSystem;

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

    ExecuteDirector* getExecuteDirector() { return mExecuteDirector; }

    EffectSystem* getEffectSystem() { return mEffectSystem; }

    nn::ui2d::DrawInfo* getDrawInfo() { return mDrawInfo; }

private:
    FontHolder* mFontHolder;
    ExecuteDirector* mExecuteDirector;
    EffectSystem* mEffectSystem;
    LayoutSystem* mLayoutSystem;
    agl::DrawContext* mDrawContex;
    nn::ui2d::DrawInfo* mDrawInfo;
};

static_assert(sizeof(LayoutKit) == 0x30);

}  // namespace al
