#pragma once

#include "Library/Area/AreaObj.h"

namespace al {
class PlacementId;

class ViewCtrlArea : public AreaObj {
public:
    ViewCtrlArea(const char* name);

    void init(const AreaInitInfo& areaInitInfo) override;
<<<<<<< HEAD

private:
    PlacementId* mClippingViewId;
};
}  // namespace al
=======
    PlacementId* getPlacementId() { return mClippingViewId; }
private:
    PlacementId* mClippingViewId;
};
}  // namespace al
>>>>>>> cae8a58c (uploaded because need to rebase other pr)
