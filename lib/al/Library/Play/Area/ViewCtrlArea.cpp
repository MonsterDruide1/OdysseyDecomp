#include "Library/Play/Area/ViewCtrlArea.h"

#include "Library/Placement/PlacementFunction.h"

namespace al {
ViewCtrlArea::ViewCtrlArea(const char* name) : AreaObj(name) {
    mClippingViewId = nullptr;
}

void ViewCtrlArea::init(const AreaInitInfo& info) {
    AreaObj::init(info);
    mClippingViewId = alPlacementFunction::createClippingViewId(*getPlacementInfo());
}
}  // namespace al
