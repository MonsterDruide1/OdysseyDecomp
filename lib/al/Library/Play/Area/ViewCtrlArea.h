#pragma once

#include "Library/Area/AreaObj.h"

namespace al {
class PlacementId;

class ViewCtrlArea : public AreaObj {
public:
    ViewCtrlArea(const char* name);

    void init(const AreaInitInfo& info) override;

    PlacementId* getPlacementId() const { return mClippingViewId; }

private:
    PlacementId* mClippingViewId;
};
}  // namespace al
