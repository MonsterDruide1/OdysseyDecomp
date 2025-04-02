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

    virtual void selectTargetPosArray(bool*, sead::Vector3f** outTargetPos, s32* outTargetPosCount,
                                      const AreaObj* areaObj, const SwitchAreaTargetInfo* info);

    virtual bool isExternalCondition() const { return true; }

private:
    AreaObjGroup* mAreaObjGroup;
    AreaObj** _10 = nullptr;
    s32 _18 = 0;
    s32 _1c = 0;
};

static_assert(sizeof(SwitchKeepOnAreaGroup) == 0x20);
}  // namespace al
