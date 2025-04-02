#include "Library/Area/SwitchKeepOnAreaGroup.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/SwitchAreaTargetInfo.h"

namespace al {
SwitchKeepOnAreaGroup::SwitchKeepOnAreaGroup(AreaObjGroup* areaObjGroup)
    : mAreaObjGroup(areaObjGroup) {
    _18 = areaObjGroup->getSize();
    if (_18 > 0)
        _10 = new AreaObj*[_18];
}

// void SwitchKeepOnAreaGroup::update(const SwitchAreaTargetInfo* info) {}

void SwitchKeepOnAreaGroup::update(const sead::Vector3f& trans) {
    sead::Vector3f pos = trans;
    SwitchAreaTargetInfo info = {&pos, 1};

    update(&info);
}
}  // namespace al
