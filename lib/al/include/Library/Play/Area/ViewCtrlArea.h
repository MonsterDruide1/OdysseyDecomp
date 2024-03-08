#include "al/include/Library/Area/AreaObj.h"
#include "al/include/Library/Placement/PlacementId.h"

namespace al {
class ViewCtrlArea : public AreaObj {
private:
    PlacementId* mPlacementId;

public:
    ViewCtrlArea(const char* name);

    void init(const AreaInitInfo& areaInitInfo) override;
};
}  // namespace al
