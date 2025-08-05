#pragma once

#include <math/seadVector.h>

namespace al {
class AreaObjGroup;

class SwitchOnAreaGroup {
public:
    SwitchOnAreaGroup(AreaObjGroup* areaObjGroup);

    virtual bool isExternalCondition() const;

    void update(const sead::Vector3f* positions, s32 posCount);
    void update(const sead::Vector3f& position);

private:
    AreaObjGroup* mAreaObjGroup;
};

static_assert(sizeof(SwitchOnAreaGroup) == 0x10);
}  // namespace al
