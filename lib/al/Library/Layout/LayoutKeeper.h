#pragma once

namespace nn::ui2d {
class Layout;
}

namespace al {
class LayoutResource;

class LayoutKeeper {
public:
    LayoutKeeper();

    void initScreen(nn::ui2d::Layout* layout, LayoutResource* resource);
};
}  // namespace al
