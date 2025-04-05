#pragma once

#include "Library/Area/AreaObj.h"

namespace al {
class SeBarrierArea : public AreaObj {
public:
    SeBarrierArea(const char* name);

    void init(const AreaInitInfo& areaInitInfo) override;

private:
    sead::Vector3f mTrans = {0.0, 0.0, 0.0};
    bool mHasData = false;
};
}  // namespace al
