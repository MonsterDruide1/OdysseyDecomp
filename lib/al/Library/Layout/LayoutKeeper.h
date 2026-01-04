#pragma once

#include <basis/seadTypes.h>

namespace nn::ui2d {
class Layout;
class DrawInfo;
}  // namespace nn::ui2d

namespace eui {
class Screen;
}

namespace al {
class LayoutResource;
class CustomTagProcessor;

class LayoutKeeper {
public:
    LayoutKeeper();

    void initScreen(eui::Screen*);
    void initLayout(nn::ui2d::Layout* layout, LayoutResource* resource);
    void initDrawInfo(nn::ui2d::DrawInfo*);
    void initTagProcessor(CustomTagProcessor*);
    void getGroup(const char*) const;
    void getGroup(s32) const;
    void getGroupNum() const;
    void calcAnim(bool);
    void draw();
};
}  // namespace al
