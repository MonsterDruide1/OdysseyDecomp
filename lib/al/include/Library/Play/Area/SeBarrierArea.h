#include "al/include/Library/Area/AreaObj.h"

namespace al {
class SeBarrierArea : public AreaObj {
private:
    sead::Vector3f mTrans = {0.0, 0.0, 0.0};
    bool mHasData = false;

public:
    SeBarrierArea(const char* name);

    void init(const AreaInitInfo& areaInitInfo) override;
};
}  // namespace al
