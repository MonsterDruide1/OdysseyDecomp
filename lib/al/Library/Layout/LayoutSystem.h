#pragma once

#include <basis/seadTypes.h>

#include <heap/seadHeap.h>

namespace agl {
class DrawContext;
}  // namespace agl

namespace nn::ui2d {
class DrawInfo;
}  // namespace nn::ui2d

namespace nn::font {
class ScalableFont;
}  // namespace nn::font

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

    ExecuteDirector* getExecuteDirector() const { return mExecuteDirector; }

    EffectSystem* getEffectSystem() const { return mEffectSystem; }

    nn::ui2d::DrawInfo* getDrawInfo() const { return mDrawInfo; }

    agl::DrawContext* getDrawContext() const { return mDrawContext; }

private:
    FontHolder* mFontHolder;
    ExecuteDirector* mExecuteDirector;
    EffectSystem* mEffectSystem;
    LayoutSystem* mLayoutSystem;
    agl::DrawContext* mDrawContext;
    nn::ui2d::DrawInfo* mDrawInfo;
};

static_assert(sizeof(LayoutKit) == 0x30);

class LayoutSystem {
public:
    LayoutSystem();

    void init();
    void initGraphicsResource();
    void initFont();
    void initEui();

    nn::font::ScalableFont* tryFindFont(const char*) const;
    nn::font::ScalableFont* getFontNamePair(s32) const;
    void prepareInitFontForChangeLanguage();
    void initFontForChangeLanguage();
    void reinitFont(sead::Heap*);

    void beginDraw();
    void endDraw();

private:
    char filler[0x30];
};

static_assert(sizeof(LayoutSystem) == 0x30);

}  // namespace al
