#pragma once

#include "Library/Area/AreaObj.h"
#include "Library/Placement/PlacementId.h"

namespace al {
class ViewCtrlArea : public AreaObj {
public:
    ViewCtrlArea(const char* name);

    void init(const AreaInitInfo& areaInitInfo) override;

private:
    PlacementId* mPlacementId;
};
}  // namespace al
