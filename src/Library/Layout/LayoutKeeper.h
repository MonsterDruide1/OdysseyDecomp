#pragma once

#include <basis/seadTypes.h>
#include <nn/ui2d/Layout.h>

namespace eui {
class Screen;
}  // namespace eui

namespace nn::ui2d {
class DrawInfo;
}  // namespace nn::ui2d

namespace al {

class LayoutPaneGroup;
class LayoutResource;
class CustomTagProcessor;

class LayoutKeeper {
public:
    LayoutKeeper();

    void initScreen(eui::Screen* screen);
    void initLayout(nn::ui2d::Layout* layout, LayoutResource* resource);
    void initDrawInfo(nn::ui2d::DrawInfo* drawInfo);
    void initTagProcessor(CustomTagProcessor* tagProcessor);
    LayoutPaneGroup* getGroup(const char* groupName) const;
    LayoutPaneGroup* getGroup(s32 groupIndex) const;
    s32 getGroupNum() const;
    void calcAnim(bool isStep);
    void draw();

    nn::ui2d::Layout* getLayout() const { return mLayout; }

private:
    void* _0;
    void* _8;
    nn::ui2d::Layout* mLayout;
};
}  // namespace al
