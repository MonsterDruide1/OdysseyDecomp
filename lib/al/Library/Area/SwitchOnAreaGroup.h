#pragma once

#include <math/seadVector.h>

namespace al {
class AreaObjGroup;
class PlayerHolder;

class SwitchOnAreaGroup {
public:
    SwitchOnAreaGroup(AreaObjGroup* areaObjGroup);

    void update(const sead::Vector3f* trans, s32);
    void update(const sead::Vector3f& trans);

    virtual bool isExternalCondition() const;

private:
    AreaObjGroup* mAreaObjGroup;
};

static_assert(sizeof(SwitchOnAreaGroup) == 0x10);
}  // namespace al
