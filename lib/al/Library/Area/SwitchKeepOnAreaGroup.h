#pragma once

#include <math/seadVector.h>

namespace al {
class AreaObjGroup;
class AreaObj;
class SwitchAreaTargetInfo;

class SwitchKeepOnAreaGroup {
public:
    SwitchKeepOnAreaGroup(AreaObjGroup* areaObjGroup);

    void update(const SwitchAreaTargetInfo* info);
    void update(const sead::Vector3f& trans);

    virtual void selectTargetPosArray(bool*, sead::Vector3f** targetPos, s32* count,
                                      const AreaObj* areaObj, const SwitchAreaTargetInfo* info);

    virtual bool isExternalCondition() const { return true; }

private:
    AreaObjGroup* mAreaObjGroup;
    unsigned char padding[0x10];
};

static_assert(sizeof(SwitchKeepOnAreaGroup) == 0x20);
}  // namespace al
